/*****************************************************************************************
//	¶ÁÈ¡´ò°üÎÄ¼þ
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-9-16
*****************************************************************************************/
#include "KWin32.h"
#include "XPackFile.h"
#include "ucl/ucl.h"
#include <crtdbg.h>

//Ò»¸öPackÎÄ¼þ¾ßÓÐµÄÍ·½á¹¹:
/*
struct XPackFileHeader
{
	unsigned char cSignature[4];		//ËÄ¸ö×Ö½ÚµÄÎÄ¼þµÄÍ·±êÖ¾£¬¹Ì¶¨Îª×Ö·û´®'PACK'
	unsigned long uCount;				//ÎÄ¼þÊý¾ÝµÄÌõÄ¿Êý
	unsigned long uIndexTableOffset;	//Ë÷ÒýµÄÆ«ÒÆÁ¿
	unsigned long uDataOffset;			//Êý¾ÝµÄÆ«ÒÆÁ¿
	unsigned long uCrc32;				//Ð£ÑéºÍ
	unsigned char cReserved[12];		//±£ÁôµÄ×Ö½Ú
};

#define	XPACKFILE_SIGNATURE_FLAG		0x4b434150	//'PACK'

//PackÖÐ¶ÔÓ¦Ã¿¸ö×ÓÎÄ¼þµÄË÷ÒýÐÅÏ¢Ïî
struct XPackIndexInfo
{
	unsigned long	uId;				//×ÓÎÄ¼þid
	unsigned long	uOffset;			//×ÓÎÄ¼þÔÚ°üÖÐµÄÆ«ÒÆÎ»ÖÃ
	long			lSize;				//×ÓÎÄ¼þµÄÔ­Ê¼´óÐ¡
	long			lCompressSizeFlag;	//×ÓÎÄ¼þÑ¹ËõºóµÄ´óÐ¡ºÍÑ¹Ëõ·½·¨
										//×î¸ß×Ö½Ú±íÊ¾Ñ¹Ëõ·½·¨£¬¼ûXPACK_METHOD
										//µÍµÄÈý¸ö×Ö½Ú±íÊ¾×ÓÎÄ¼þÑ¹ËõºóµÄ´óÐ¡
};

//°üÎÄ¼þµÄÑ¹Ëõ·½Ê½
enum XPACK_METHOD
{
	TYPE_NONE	= 0x00000000,			//Ã»ÓÐÑ¹Ëõ
	TYPE_UCL	= 0x01000000,			//UCLÑ¹Ëõ
	TYPE_BZIP2	= 0x02000000,			//bzip2Ñ¹Ëõ
	TYPE_FRAME	= 0x10000000,			//Ê¹ÓÃÁË¶ÀÁ¢Ö¡Ñ¹Ëõ,×ÓÎÄ¼þÎªsprÀàÐÍÊ±²Å¿ÉÄÜÓÃµ½
	TYPE_METHOD_FILTER = 0x0f000000,	//¹ýÂË±ê¼Ç
	TYPE_FILTER = 0xff000000,			//¹ýÂË±ê¼Ç
};

//pak°üÖÐ±£´æµÄsprÖ¡ÐÅÏ¢Ïî
struct XPackSprFrameInfo
{
	long lCompressSize;
	long lSize;
} ;
*/
XPackFile::XPackElemFileCache	XPackFile::ms_ElemFileCache[MAX_XPACKFILE_CACHE];
int								XPackFile::ms_nNumElemFileCache = 0;

XPackFile::XPackFile()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_uFileSize = 0;
	m_pIndexList = NULL;
	m_nElemFileCount = 0;  // ×ÓÎÄ¼þÊýÁ¿
	InitializeCriticalSection(&m_ReadCritical);
}

XPackFile::~XPackFile()
{
	Close();
	DeleteCriticalSection(&m_ReadCritical);
}

//--------------------------------------------------
//Function: Open package file
//Return: success or failure
//--------------------------------------------------
bool XPackFile::Open(const char* pszPackFileName, int nSelfIndex)
{
	bool bResult = false;
	Close();
	EnterCriticalSection(&m_ReadCritical);
	m_nSelfIndex = nSelfIndex;
	m_hFile = ::CreateFile(pszPackFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	while (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_uFileSize = ::GetFileSize(m_hFile, NULL);
		if (m_uFileSize == 0 || m_uFileSize == INVALID_FILE_SIZE || m_uFileSize <= sizeof(XPackFileHeader))
		{
			break;
		}
		XPackFileHeader Header;
		//		XPackFileHeaderX xHeader;
		DWORD dwListSize, dwReaded;
		//--读取包文件头--
		if (::ReadFile(m_hFile, &Header, sizeof(Header), &dwReaded, NULL) == FALSE)
		{
			//           if (::ReadFile(m_hFile, &xHeader, sizeof(xHeader), &dwReaded, NULL) == FALSE)
			break;
		}
		// Judgment of legality of package file mark and content
		if (dwReaded != sizeof(Header) ||
			*(int*)(&Header.cSignature) != XPACKFILE_SIGNATURE_FLAG ||
			Header.uCount == 0 ||
			Header.uIndexTableOffset < sizeof(XPackFileHeader) ||
			Header.uIndexTableOffset >= m_uFileSize ||
			Header.uDataOffset < sizeof(XPackFileHeader) ||
			Header.uDataOffset >= m_uFileSize)
		{
			break;
		}
		//--读取索引子信息表--
		dwListSize = sizeof(XPackIndexInfo) * Header.uCount;
		m_pIndexList = (XPackIndexInfo*)malloc(dwListSize); // Allocate memory. . . .
		// printf("---PAK(%s) allocates memory: %u Mb, total %d files--\n",pszPackFileName,dwListSize/(1024*1024),Header.uCount);
		if (m_pIndexList == NULL ||
			::SetFilePointer(m_hFile, Header.uIndexTableOffset, NULL, FILE_BEGIN) != Header.uIndexTableOffset)
		{
			break;
		}

		if (::ReadFile(m_hFile, m_pIndexList, dwListSize, &dwReaded, NULL) == FALSE)
			break;
		if (dwReaded != dwListSize)
			break;
		m_nElemFileCount = Header.uCount; // 子文件数量
		bResult = true;
		break;
	};

	if (bResult == false)
		Close();
	LeaveCriticalSection(&m_ReadCritical);
	return bResult;
}
//-------------------------------------------------
//¹¦ÄÜ£º´ò¿ªÐÂ°üÎÄ¼þ
//·µ»Ø£º³É¹¦Óë·ñ
//-------------------------------------------------
/*bool XPackFile::OpenX(const char* pszPackFileName, int nSelfIndex)
{
	bool bResult = false;
	Close();
	EnterCriticalSection(&m_ReadCritical);
	m_nSelfIndex = nSelfIndex;
	m_hFile = ::CreateFile(pszPackFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	while (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_uFileSize = ::GetFileSize(m_hFile, NULL);
		if (m_uFileSize == 0 || m_uFileSize == INVALID_FILE_SIZE || m_uFileSize <= sizeof(XPackFileHeaderX))
		{
			break;
		}
		XPackFileHeaderX xHeader;
		DWORD dwListSize, dwReaded;
		//--¶ÁÈ¡°üÎÄ¼þÍ·--
		if (::ReadFile(m_hFile, &xHeader, sizeof(xHeader), &dwReaded, NULL) == FALSE)
		{
			break;
		}
		//--°üÎÄ¼þ±ê¼ÇÓëÄÚÈÝµÄºÏ·¨ÐÔÅÐ¶Ï--
		if (dwReaded != sizeof(xHeader) ||
			*(int*)(&xHeader.cSignature) != XPACKFILE_SIGNATURE_FLAG ||
			xHeader.uCount == 0 ||
			xHeader.SubHeader[0].uIndexTableOffset < sizeof(XPackFileHeaderX) ||
			xHeader.SubHeader[0].uIndexTableOffset >= m_uFileSize ||
			xHeader.SubHeader[0].uDataOffset < sizeof(XPackFileHeaderX) ||
			xHeader.SubHeader[0].uDataOffset >= m_uFileSize)
		{
			break;
		}

		//--¶ÁÈ¡Ë÷Òý×ÓÐÅÏ¢±í--
		dwListSize = sizeof(XPackIndexInfoX) * xHeader.SubHeader[0].uCount;
		m_pIndexListX = (XPackIndexInfoX*)malloc(dwListSize);   //·ÖÅäÄÚ´æ¡£¡£¡£¡£
		if (m_pIndexListX == NULL ||
			::SetFilePointer(m_hFile, xHeader.SubHeader[0].uIndexTableOffset, NULL, FILE_BEGIN) != xHeader.SubHeader[0].uIndexTableOffset)
		{//ÉèÖÃµ±Ç°ÎÄ¼þ¶ÁÈ¡Î»ÖÃ
			break;
		}
		//Ö±½Ó¶ÁÈ¡×ÓÎÄ¼þ
		if (::ReadFile(m_hFile, m_pIndexListX, dwListSize, &dwReaded, NULL) == FALSE)
			break;
		if (dwReaded != dwListSize)
			break;
		m_nElemFileCount= xHeader.SubHeader[0].uCount;  //×ÓÎÄ¼þÊýÁ¿
		bResult = true;
		break;
	};
	if (bResult == false)
		Close();
	LeaveCriticalSection(&m_ReadCritical);
	return bResult;
}	*/
//-------------------------------------------------
//¹¦ÄÜ£º¹Ø±Õ°üÎÄ¼þ
//-------------------------------------------------
void XPackFile::Close()
{
	EnterCriticalSection(&m_ReadCritical);

	if (m_pIndexList)
	{
		// Clear the cached sub-files in the cache (which may be) in this package
		for (int i = ms_nNumElemFileCache - 1; i >= 0; i--)
		{
			if (ms_ElemFileCache[i].nPackIndex == m_nSelfIndex)
			{
				FreeElemCache(i);
				ms_nNumElemFileCache--;
				for (int j = i; j < ms_nNumElemFileCache; ++j)
					ms_ElemFileCache[j] = ms_ElemFileCache[j + 1];
			}
		}
		free(m_pIndexList);
		m_pIndexList = NULL;
	}
	m_nElemFileCount = 0;

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	m_uFileSize = 0;

	LeaveCriticalSection(&m_ReadCritical);
}

//--------------------------------------------------
//Function: Release the contents of a cache node
//Return: success or failure
//--------------------------------------------------
void XPackFile::FreeElemCache(int nCacheIndex)
{
	_ASSERT(nCacheIndex >= 0 && nCacheIndex < ms_nNumElemFileCache);
	if (ms_ElemFileCache[nCacheIndex].pBuffer)
	{
		free(ms_ElemFileCache[nCacheIndex].pBuffer);
		ms_ElemFileCache[nCacheIndex].pBuffer = NULL;
	}
	ms_ElemFileCache[nCacheIndex].uId = 0;
	ms_ElemFileCache[nCacheIndex].lSize = 0;
	ms_ElemFileCache[nCacheIndex].uRefFlag = 0;
	ms_ElemFileCache[nCacheIndex].nPackIndex = -1;
}

//--------------------------------------------------
// Function: Directly read the data in the package file data into the buffer
// Return: success or failure
//--------------------------------------------------
bool XPackFile::DirectRead(void* pBuffer, unsigned int uOffset, unsigned int uSize) const
{
	bool bResult = false;
	DWORD dwReaded;
	//_ASSERT(pBuffer && m_hFile != INVALID_HANDLE_VALUE);
	if (pBuffer == NULL || m_hFile == INVALID_HANDLE_VALUE)
		return bResult;

	if (uOffset + uSize <= m_uFileSize && ::SetFilePointer(m_hFile, uOffset, 0, FILE_BEGIN) == uOffset) // Set the current read and write position
	{
		if (ReadFile(m_hFile, pBuffer, uSize, &dwReaded, NULL))
		{
			if (dwReaded == uSize)
				bResult = true;
		}
	}
	return bResult;
}

//--------------------------------------------------
// Function: Read package files into buffer with decompression
// Parameters: pBuffer --> buffer pointer
// uExtractSize --> The size of the data (expected) after decompression, the size of the pBuffer buffer is not less than this number
// lCompressType --> Get the original (/compressed) size directly from the package
// uOffset --> Start reading from this offset position in the package
// uSize --> The size of the (compressed) data read directly from the package
// Return: success or failure
//--------------------------------------------------
bool XPackFile::ExtractRead(void* pBuffer, unsigned int uExtractSize, long lCompressType, unsigned int uOffset, unsigned int uSize) const
{
	_ASSERT(pBuffer);
	bool bResult = false;
	if (lCompressType == TYPE_NONE)
	{ // 没有压缩  直接读取
		if (uExtractSize == uSize)
			bResult = DirectRead(pBuffer, uOffset, uSize);
	}
	else
	{
		void* pReadBuffer = malloc(uSize); // 分配内存
		//|| lCompressType == TYPE_BZIP2
		if (pReadBuffer)
		{
			if ((lCompressType == TYPE_UCL || lCompressType == TYPE_UCL_OLD || lCompressType == TYPE_BZIP2_OLD || lCompressType == TYPE_BZIP2) && DirectRead(pReadBuffer, uOffset, uSize))
			{
				unsigned int uDestLength;
				ucl_nrv2b_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL); // 解压读取
				bResult = (uDestLength == uExtractSize);
			}
			else if ((lCompressType == TYPE_FRAGMENT || lCompressType == TYPE_FRAGMENT_OLD) && DirectRead(pReadBuffer, uOffset, uSize))
			{
				unsigned int uDestLength;
				ucl_nrv2d_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL); // 解压读取
				bResult = (uDestLength == uExtractSize);
			}
			else if ((lCompressType == TYPE_FRAGMENTA || lCompressType == TYPE_FRAGMENTA_OLD) && DirectRead(pReadBuffer, uOffset, uSize))
			{
				unsigned int uDestLength;
				ucl_nrv2e_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL); // 解压读取
				bResult = (uDestLength == uExtractSize);
			}
			else
			{	// new package
				if (DirectRead(pReadBuffer, uOffset, uSize))
				{
					unsigned int uDestLength;
					//ucl_nrv2e_decompress_8_fs((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength);
					ucl_nrv2e_decompress_8((BYTE*)pReadBuffer, uSize, (BYTE*)pBuffer, &uDestLength, NULL); //Decompress and read
					bResult = (uDestLength == uExtractSize); //Decompress data
				}
			}
			free(pReadBuffer);
		}
	}
	return bResult;
}

//½âÑ¹º¯Êý
int XPackFile::ucl_nrv2e_decompress_8_fs(unsigned char* s_buf, int s_size, unsigned char* d_buf, unsigned int* d_size)  const
{
	unsigned int ch = 0;
	unsigned int repeat_pri = 1;		    //ÉÏÒ»´ÎµÄ REPEAT VALUE
	unsigned int repeat_cur;			//±¾´ÎµÄ REPEAT VALUE
	unsigned int repeat_cnt;			// repeat count
	unsigned int d_cnt = 0;
	unsigned char* bak_s_buf;		    //±¸·ÝµÄÊý¾Ý

	bak_s_buf = s_buf;
	do
	{
		do
		{
			if ((ch & 0x7f) != 0)
				ch <<= 1;
			else
				ch = ((*(s_buf++)) << 1) + 1;
			if ((ch & 0x100) == 0) break;
			*(d_buf + d_cnt++) = *(s_buf++);
		} while (1);
		repeat_cur = 1;

		do
		{
			if ((ch & 0x7f) != 0)
				ch <<= 1;
			else
				ch = ((*(s_buf++)) << 1) + 1;
			repeat_cur <<= 1;
			repeat_cur += (ch & 0x100) >> 8;
			if ((ch & 0x7f) != 0)
				ch <<= 1;
			else
				ch = ((*(s_buf++)) << 1) + 1;
		} while ((ch & 0x100) == 0);

		if (repeat_cur == 2)
			repeat_cur = repeat_pri;
		else
		{
			repeat_cur += 0x00fffffd;
			repeat_cur <<= 8;
			repeat_cur += *(s_buf++);
			if (repeat_cur == 0xffffffff) break;
			repeat_cur++;
			repeat_pri = repeat_cur;
		}
		if ((ch & 0x7f) != 0)
			ch <<= 1;
		else
			ch = ((*(s_buf++)) << 1) + 1;
		repeat_cnt = (ch & 0x100) >> 8;
		if ((ch & 0x7f) != 0)
			ch <<= 1;
		else
			ch = ((*(s_buf++)) << 1) + 1;
		repeat_cnt <<= 1;
		repeat_cnt += (ch & 0x100) >> 8;
		if (repeat_cnt == 0)
		{
			repeat_cnt = 1;
			do
			{
				if ((ch & 0x7f) != 0)
					ch <<= 1;
				else
					ch = ((*(s_buf++)) << 1) + 1;
				repeat_cnt <<= 1;
				repeat_cnt += (ch & 0x100) >> 8;
				if ((ch & 0x7f) != 0)
					ch <<= 1;
				else
					ch = ((*(s_buf++)) << 1) + 1;
			} while ((ch & 0x100) == 0);
			repeat_cnt += 2;
		}
		if (repeat_cur > 0x0d00) repeat_cnt++;
		repeat_cur = d_cnt - repeat_cur;
		*(d_buf + d_cnt++) = *(d_buf + repeat_cur++);
		do
		{
			*(d_buf + d_cnt++) = *(d_buf + repeat_cur++);
			repeat_cnt--;
		} while (repeat_cnt != 0);

	} while (1);
	*d_size = d_cnt;
	if (s_size > (s_buf - bak_s_buf))
		return 1;
	if (s_size == (s_buf - bak_s_buf))
		return 0;
	if (s_size < (s_buf - bak_s_buf))
		return -1;

	return 0;
}

//--------------------------------------------------
//Function: Find sub-file items in the index table (dichotomy search)
//Return: If found, return the position in the index table (>=0); if not found, return -1
//--------------------------------------------------
int XPackFile::XFindElemFileA(unsigned long ulId) const
{
	int nBegin, nEnd, nMid;
	nBegin = 0;
	nEnd = m_nElemFileCount - 1;
	while (nBegin <= nEnd)
	{
		nMid = (nBegin + nEnd) / 2;
		if (ulId < m_pIndexList[nMid].uId)
			nEnd = nMid - 1;
		else if (ulId > m_pIndexList[nMid].uId)
			nBegin = nMid + 1;
		else
			break;
	}
	return ((nBegin <= nEnd) ? nMid : -1);
}

//--------------------------------------------------
//Function: Find subfiles in the package
//Parameters: uId --> id of sub-file
//ElemRef -->If found, fill in the relevant information of the sub-file in this structure
//Return: whether found
//--------------------------------------------------
bool XPackFile::XFindElemFile(unsigned long uId, XPackElemFileRef& ElemRef)
{
	ElemRef.nElemIndex = -1;
	if (uId)
	{
		EnterCriticalSection(&m_ReadCritical);
		ElemRef.nCacheIndex = FindElemFileInCache(uId, -1);  //²éÕÒÊÇ·ñÔÚ»º´æÖÐ
		if (ElemRef.nCacheIndex >= 0)
		{
			ElemRef.uId = uId;
			ElemRef.nPackIndex = ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex;
			ElemRef.nElemIndex = ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex;
			ElemRef.nSize = ms_ElemFileCache[ElemRef.nCacheIndex].lSize;
			ElemRef.nOffset = 0;
		}
		else
		{
			ElemRef.nElemIndex = XFindElemFileA(uId);
			if (ElemRef.nElemIndex >= 0)
			{
				ElemRef.uId = uId;
				ElemRef.nPackIndex = m_nSelfIndex;
				ElemRef.nOffset = 0;
				ElemRef.nSize = m_pIndexList[ElemRef.nElemIndex].lSize;
			}
		}
		LeaveCriticalSection(&m_ReadCritical);
	}
	return (ElemRef.nElemIndex >= 0);
}

//-------------------------------------------------
//¹¦ÄÜ£º·ÖÅä»º³åÇø£¬²¢¶Á°üÄÚµÄ×ÓÎÄ¼þµÄÄÚÈÝµ½ÆäÖÐ
//²ÎÊý£º×ÓÎÄ¼þÔÚ°üÄÚµÄË÷Òý
//·µ»Ø£º³É¹¦Ôò·µ»Ø»º³åÇøµÄÖ¸Õë£¬·ñÔò·µ»Ø¿ÕÖ¸Õë
//-------------------------------------------------
void* XPackFile::ReadElemFile(int nElemIndex) const
{
	_ASSERT(nElemIndex >= 0 && nElemIndex < m_nElemFileCount);
	void* pDataBuffer = malloc(m_pIndexList[nElemIndex].lSize);
	if (pDataBuffer)
	{//´øÑ¹Ëõ¶ÁÈ¡ÎÄ¼þ
		long lCompressType = m_pIndexList[nElemIndex].lCompressSizeFlag & TYPE_FILTER_OLD;
		unsigned int uSize = m_pIndexList[nElemIndex].lCompressSizeFlag & (~TYPE_FILTER_OLD);

		if (ExtractRead(pDataBuffer,
			m_pIndexList[nElemIndex].lSize,
			lCompressType,  //²»µÈÓÚ¹ýÂË±êÖ¾
			m_pIndexList[nElemIndex].uOffset,
			uSize) == false)
		{
			free(pDataBuffer);
			pDataBuffer = NULL;
		}
	}
	return pDataBuffer;
}

//-------------------------------------------------
//¹¦ÄÜ£ºÔÚcacheÀï²éÕÒ×ÓÎÄ¼þ
//²ÎÊý£ºuId --> ×ÓÎÄ¼þid
//		nDesireIndex --> ÔÚcacheÖÐµÄ¿ÉÄÜÎ»ÖÃ
//·µ»Ø£º³É¹¦Ôò·µ»Øcache½ÚµãË÷Òý(>=0),Ê§°ÜÔò·µ»Ø-1
//-------------------------------------------------
int XPackFile::FindElemFileInCache(unsigned int uId, int nDesireIndex)
{
	if (nDesireIndex >= 0 && nDesireIndex < ms_nNumElemFileCache &&
		uId == ms_ElemFileCache[nDesireIndex].uId)
	{
		ms_ElemFileCache[nDesireIndex].uRefFlag = 0xffffffff;
		return nDesireIndex;
	}

	nDesireIndex = -1;
	for (int i = 0; i < ms_nNumElemFileCache; ++i)
	{
		if (uId == ms_ElemFileCache[i].uId)
		{
			ms_ElemFileCache[i].uRefFlag = 0xffffffff;
			nDesireIndex = i;
			break;
		}
	}
	return nDesireIndex;
}

//-------------------------------------------------
//¹¦ÄÜ£º°Ñ×ÓÎÄ¼þÊý¾ÝÌí¼Óµ½cache
//²ÎÊý£ºpBuffer --> ´æÓÐ×ÓÎÄ¼þÊý¾ÝµÄ»º³åÇø
//		nElemIndex --> ×ÓÎÄ¼þÔÚ°üÖÐµÄË÷Òý
//·µ»Ø£ºÌí¼Óµ½cacheµÄË÷ÒýÎ»ÖÃ
//-------------------------------------------------
int XPackFile::AddElemFileToCache(void* pBuffer, int nElemIndex)
{
	_ASSERT(pBuffer && nElemIndex >= 0 && nElemIndex < m_nElemFileCount);
	int nCacheIndex;
	if (ms_nNumElemFileCache < MAX_XPACKFILE_CACHE)
	{	//ÕÒµ½Ò»¸ö¿ÕÎ»ÖÃ
		nCacheIndex = ms_nNumElemFileCache++;
	}
	else
	{	//ÊÍ·ÅÒ»¸ö¾ÉµÄcache½Úµã
		nCacheIndex = 0;
		if (ms_ElemFileCache[0].uRefFlag)
			ms_ElemFileCache[0].uRefFlag--;
		for (int i = 1; i < MAX_XPACKFILE_CACHE; ++i)
		{
			if (ms_ElemFileCache[i].uRefFlag)
				ms_ElemFileCache[i].uRefFlag--;
			if (ms_ElemFileCache[i].uRefFlag < ms_ElemFileCache[nCacheIndex].uRefFlag)
				nCacheIndex = i;

		}
		FreeElemCache(nCacheIndex);
	}
	ms_ElemFileCache[nCacheIndex].pBuffer = pBuffer;
	ms_ElemFileCache[nCacheIndex].uId = m_pIndexList[nElemIndex].uId;
	ms_ElemFileCache[nCacheIndex].lSize = m_pIndexList[nElemIndex].lSize;
	ms_ElemFileCache[nCacheIndex].nPackIndex = m_nSelfIndex;
	ms_ElemFileCache[nCacheIndex].nElemIndex = nElemIndex;
	ms_ElemFileCache[nCacheIndex].uRefFlag = 0xffffffff;
	return nCacheIndex;
}

//-------------------------------------------------
//¹¦ÄÜ£º¶ÁÈ¡×ÓÎÄ¼þÒ»¶¨³¤¶ÈµÄÊý¾Ýµ½»º³åÇø
//²ÎÊý£ºpBuffer --> ÓÃÀ´¶ÁÈ¡Êý¾ÝµÄ»º³åÇø
//		uSize --> Òª¶ÁÈ¡µÄÊý¾ÝµÄ³¤¶È
//·µ»Ø£º³É¹¦¶ÁÈ¡µÃ×Ö½ÚÊý
//-------------------------------------------------
int XPackFile::XElemFileRead(XPackElemFileRef& ElemRef, void* pBuffer, unsigned uSize)
{
	int nResult = 0;
	if (pBuffer && ElemRef.uId && ElemRef.nElemIndex >= 0)
	{
		EnterCriticalSection(&m_ReadCritical);

		//--ÏÈ¿´ÊÇ·ñÒÑ¾­ÔÚcacheÀïÁË---
		ElemRef.nCacheIndex = FindElemFileInCache(ElemRef.uId, ElemRef.nCacheIndex);

		if (ElemRef.nCacheIndex < 0 &&								//ÔÚcacheÀïÎ´ÕÒµ½
			ElemRef.nElemIndex < m_nElemFileCount &&
			m_pIndexList[ElemRef.nElemIndex].uId == ElemRef.uId)
		{
			void* pDataBuffer = ReadElemFile(ElemRef.nElemIndex);
			if (pDataBuffer) //Ôö¼ÓÎÄ¼þ»º´æ£¬·µ»ØË÷Òý£¬ÒÔ±ãµ÷ÓÃ·ÃÎÊ
				ElemRef.nCacheIndex = AddElemFileToCache(pDataBuffer, ElemRef.nElemIndex);
		}

		if (ElemRef.nCacheIndex >= 0 &&
			//´ËÏÂÃæÈýÏîÓ¦¸ÃÕ¹¿ª¼ì²é£¬·ÀÖ¹±»Ä£¿éÍâ²¿¸Ä±ä£¬ÒýÆð´íÎó¡£
			//ÎªÐ§ÂÊ¿É¿¼ÂÇÊ¡ÂÔ£¬µ«ÐèÍâ²¿°´ÕÕ¹æÔòËæ±ã¸Ä±äElemRefµÄÄÚÈÝ¡£
			ElemRef.nPackIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex &&
			ElemRef.nElemIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex &&
			ElemRef.nSize == ms_ElemFileCache[ElemRef.nCacheIndex].lSize
			)
		{
			//_ASSERT(ElemRef.nPackIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nPackIndex);
			//_ASSERT(ElemRef.nElemIndex == ms_ElemFileCache[ElemRef.nCacheIndex].nElemIndex);
			//_ASSERT(ElemRef.nSize == ms_ElemFileCache[ElemRef.nCacheIndex].lSize);

			if (ElemRef.nOffset < 0)
				ElemRef.nOffset = 0;
			if (ElemRef.nOffset < ElemRef.nSize)
			{
				if (ElemRef.nOffset + (int)uSize <= ElemRef.nSize)
					nResult = uSize;
				else
					nResult = ElemRef.nSize - ElemRef.nOffset;
				memcpy(pBuffer, (char*)ms_ElemFileCache[ElemRef.nCacheIndex].pBuffer + ElemRef.nOffset, nResult);
				ElemRef.nOffset += nResult;
			}
			else
			{
				ElemRef.nOffset = ElemRef.nSize;
			}
		}
		LeaveCriticalSection(&m_ReadCritical);
	}
	return nResult;
}

#define	NODE_INDEX_STORE_IN_RESERVED	2

SPRHEAD* XPackFile::GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable)
{
	SPRHEAD* pSpr = NULL;
	bool			bOk = false;

	pOffsetTable = NULL;
	if (ElemRef.uId == 0 || ElemRef.nElemIndex < 0)
		return NULL;

	EnterCriticalSection(&m_ReadCritical);
	if (ElemRef.nElemIndex < m_nElemFileCount && m_pIndexList[ElemRef.nElemIndex].uId == ElemRef.uId)
	{
		//Ê×ÏÈ¼ì²éÕâ¸öidÊÇÊ²Ã´ÀàÐÍÑ¹Ëõ·½Ê½
		if ((m_pIndexList[ElemRef.nElemIndex].lCompressSizeFlag & TYPE_FRAME) == 0)//TYPE_FRAME TYPE_UCL
		{//Ñ¹Ëõ°üÄÚSPR¶ÁÈ¡
			pSpr = (SPRHEAD*)ReadElemFile(ElemRef.nElemIndex);
			if (pSpr)
			{
				if ((*(int*)&pSpr->Comment[0]) == SPR_COMMENT_FLAG)
				{
					pOffsetTable = (SPROFFS*)(((char*)pSpr) + sizeof(SPRHEAD) + pSpr->Colors * 3);
					bOk = true;
				}
			}
		}
		/*	    else if ((m_pIndexList[ElemRef.nElemIndex].lCompressSizeFlag & TYPE_UCL) == 0)//TYPE_FRAME TYPE_UCL
				{//Ñ¹Ëõ°üÄÚSPR¶ÁÈ¡
					pSpr = (SPRHEAD*)ReadElemFile(ElemRef.nElemIndex);
					if (pSpr)
					{
						if ((*(int*)&pSpr->Comment[0]) == SPR_COMMENT_FLAG)
						{
							pOffsetTable = (SPROFFS*)(((char*)pSpr) + sizeof(SPRHEAD) + pSpr->Colors * 3);
							bOk = true;
						}
					}
				}
				else if ((m_pIndexList[ElemRef.nElemIndex].lCompressSizeFlag & TYPE_FRAGMENT) == 0)//TYPE_FRAME TYPE_UCL
				{//Ñ¹Ëõ°üÄÚSPR¶ÁÈ¡
					pSpr = (SPRHEAD*)ReadElemFile(ElemRef.nElemIndex);
					if (pSpr)
					{
						if ((*(int*)&pSpr->Comment[0]) == SPR_COMMENT_FLAG)
						{
							pOffsetTable = (SPROFFS*)(((char*)pSpr) + sizeof(SPRHEAD) + pSpr->Colors * 3);
							bOk = true;
						}
					}
				}
				else if ((m_pIndexList[ElemRef.nElemIndex].lCompressSizeFlag & TYPE_FRAGMENTA) == 0)//TYPE_FRAME TYPE_UCL
				{//Ñ¹Ëõ°üÄÚSPR¶ÁÈ¡
					pSpr = (SPRHEAD*)ReadElemFile(ElemRef.nElemIndex);
					if (pSpr)
					{
						if ((*(int*)&pSpr->Comment[0]) == SPR_COMMENT_FLAG)
						{
							pOffsetTable = (SPROFFS*)(((char*)pSpr) + sizeof(SPRHEAD) + pSpr->Colors * 3);
							bOk = true;
						}
					}
				}*/
		else
		{//Ö±½Ó¶ÁÈ¡ --°üÍâµÄSPR
			SPRHEAD Header;
			if (DirectRead(&Header, m_pIndexList[ElemRef.nElemIndex].uOffset, sizeof(SPRHEAD)))
			{
				if (*(int*)&(Header.Comment[0]) == SPR_COMMENT_FLAG)  //spr
				{
					unsigned int	u2ListSize = Header.Colors * 3 + Header.Frames * sizeof(XPackSprFrameInfo);
					pSpr = (SPRHEAD*)malloc(sizeof(SPRHEAD) + u2ListSize);
					if (pSpr)
					{
						if (DirectRead((char*)(&pSpr[1]), m_pIndexList[ElemRef.nElemIndex].uOffset + sizeof(SPRHEAD), u2ListSize))
						{
							memcpy(pSpr, &Header, sizeof(SPRHEAD));
							bOk = true;
						}
					}
				}
			}
		}

		if (pSpr)
		{
			if (bOk)
			{
				*((int*)&pSpr->Reserved[NODE_INDEX_STORE_IN_RESERVED]) = ElemRef.nElemIndex;
			}
			else
			{
				free(pSpr);
				pSpr = NULL;
			}
		}
	}
	LeaveCriticalSection(&m_ReadCritical);
	return pSpr;
}

SPRFRAME* XPackFile::GetSprFrame(SPRHEAD* pSprHeader, int nFrame)
{
	SPRFRAME* pFrame = NULL;
	if (pSprHeader && nFrame >= 0 && nFrame < pSprHeader->Frames)
	{
		EnterCriticalSection(&m_ReadCritical);
		int nNodeIndex = *((int*)&pSprHeader->Reserved[NODE_INDEX_STORE_IN_RESERVED]);
		if (nNodeIndex >= 0 && nNodeIndex < m_nElemFileCount)
		{
			long lCompressType = m_pIndexList[nNodeIndex].lCompressSizeFlag;
			if ((lCompressType & TYPE_FRAME) != 0) //Èç¹û²»ÊÇ¶ÀÁ¢Ö¡Ñ¹ËõµÄ
			{
				bool bOk = false;
				lCompressType &= TYPE_METHOD_FILTER_OLD;
				long	lTempValue = sizeof(SPRHEAD) + pSprHeader->Colors * 3;
				//¶Á³öÖ¸¶¨Ö¡µÄÐÅÏ¢
				XPackSprFrameInfo* pFrameList = (XPackSprFrameInfo*)((char*)pSprHeader + lTempValue);
				unsigned long	uSrcOffset = m_pIndexList[nNodeIndex].uOffset + lTempValue + pSprHeader->Frames * sizeof(XPackSprFrameInfo);
				while (nFrame > 0)
				{
					uSrcOffset += (pFrameList++)->lCompressSize;
					nFrame--;
				};
				lTempValue = pFrameList->lSize;
				if (lTempValue < 0)
				{
					lTempValue = -lTempValue;
					if (pFrame = (SPRFRAME*)malloc(lTempValue))
						bOk = DirectRead(pFrame, uSrcOffset, lTempValue);
				}
				else
				{//½âÑ¹¶ÁÈ¡
					if (pFrame = (SPRFRAME*)malloc(lTempValue))
					{
						bOk = ExtractRead(pFrame, lTempValue, lCompressType, uSrcOffset, pFrameList->lCompressSize);
					}
				}

				if (bOk == false && pFrame != NULL)
				{
					free(pFrame);
					pFrame = NULL;
				}
			}
		}
		LeaveCriticalSection(&m_ReadCritical);
	}
	return pFrame;
}
