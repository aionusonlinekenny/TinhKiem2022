//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KIniFile.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Ini File Operation Class
//---------------------------------------------------------------------------
#include "KWin32.h"
#include "KDebug.h"
#include "KMemBase.h"
#include "KStrBase.h"
#include "KFile.h"
#include "KFilePath.h"
#include "KPakFile.h"
#ifndef _SERVER
#include "KCodec.h"
#endif
#include "KIniFile.h"
#include <string.h>
#include "KSG_StringProcess.h"

//---------------------------------------------------------------------------
typedef struct {
	DWORD		Id;			// ÎÄ¼þ±êÊ¶ = 0x4b434150 ("PACK")
	DWORD		DataLen;	// ÎÄ¼þÔ­Ê¼µÄ³¤¶È
	DWORD		PackLen;	// ÎÄ¼þÑ¹Ëõºó³¤¶È
	DWORD		Method;		// Ê¹ÓÃµÄÑ¹ËõËã·¨
} TPackHead;
//---------------------------------------------------------------------------
static int32_t  g_nCodec = 0;// codec method = 0,1,2,3,
//---------------------------------------------------------------------------
// º¯Êý:	SetCodec
// ¹¦ÄÜ:	set codec method
// ²ÎÊý:	void
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void g_SetIniCodec(int32_t nCodec)
{
	g_nCodec = nCodec;
}
//---------------------------------------------------------------------------
// º¯Êý:	KIniFile
// ¹¦ÄÜ:	¹ºÔìº¯Êý
// ²ÎÊý:	void
// ·µ»Ø:	void
//---------------------------------------------------------------------------
KIniFile::KIniFile()
{
	g_MemZero(&m_Header, sizeof(SECNODE));
	dwFileIdx = 0;
}
//---------------------------------------------------------------------------
// º¯Êý:	~KIniFile
// ¹¦ÄÜ:	ÎöÔìº¯Êý
// ²ÎÊý:	void
// ·µ»Ø:	void
//---------------------------------------------------------------------------
KIniFile::~KIniFile()
{
	Clear();
}
//---------------------------------------------------------------------------
// º¯Êý:	Load
// ¹¦ÄÜ:	¼ÓÔØÒ»¸öINIÎÄ¼þ
// ²ÎÊý:	FileName	ÎÄ¼þÃû
// ·µ»Ø:	TRUE		³É¹¦
//			FALSE		Ê§°Ü
//---------------------------------------------------------------------------
BOOL KIniFile::Load(LPCSTR FileName, BOOL nIsCreate)
{
	KFile File;
	KPakFile pFile;
	DWORD dwSize;
	PVOID Buffer;
	TPackHead* pHeader;

	// check file name
	if (FileName[0] == 0)
		return FALSE;

	if (m_Header.pNextNode)
		Clear();

	if (!pFile.Open((char*)FileName)) // Open the file in the package
	{
		//		//g_DebugLog("Can't open ini file : %s", FileName);
		if (nIsCreate)
		{
			if (File.Create((char*)FileName)) // Create one if it doesn't exist?
			{
				if (!pFile.Open((char*)FileName))
				{
					File.Close();
					return FALSE;
				}
			}
			File.Close();
		}
		else
			return FALSE;
	}

#pragma message("KIniFile 当前不支持 KeyName 为中文! by Romandou")

	dwSize = pFile.Size();
	char nFilePath[218] = { 0 };
	sprintf(nFilePath, FileName);
	// g_StrLower(nFilePath);
	_strlwr(nFilePath);
	dwFileIdx = g_FileName2Id(nFilePath);

	/*if (dwSize<=0)
		return FALSE;*/

	Buffer = m_MemStack.Push(dwSize + 4); // 分配内存

	pFile.Read(Buffer, dwSize); // 读取包中文件

	pHeader = (TPackHead*)Buffer;

	if (pHeader->Id == 0x4b434150) // "PACK"
	{
		pFile.Close();
		return LoadPack(FileName);
	}

	CreateIniLink(Buffer, dwSize);
	pFile.Close();

	return TRUE;
}

//---------------------------------------------------------------------------
// º¯Êý:	LoadPack
// ¹¦ÄÜ:	´ò¿ªÒ»¸öÑ¹ËõµÄINIÎÄ¼þ
// ²ÎÊý:	FileName	ÎÄ¼þÃû
// ·µ»Ø:	TRUE		³É¹¦
//			FALSE		Ê§°Ü
//---------------------------------------------------------------------------
BOOL KIniFile::LoadPack(LPCSTR FileName)
{
	KPakFile	File;
	PVOID		PackBuf;
	PVOID		DataBuf;
#ifndef _SERVER
	KCodec* pCodec;
	TCodeInfo	CodeInfo;
#endif
	TPackHead	Header;

	// check file name
	if (FileName[0] == 0)
		return FALSE;

	// close open file
	if (m_Header.pNextNode)
		Clear();

	// open the pack ini file
	if (!File.Open((char*)FileName))
		return FALSE;

	// read pack file header
	File.Read(&Header, sizeof(Header));

	// if not a pack file read directly
	if (Header.Id != 0x4b434150) // "PACK"
		return FALSE;

	// init codec
#ifndef _SERVER
	pCodec = NULL;
	g_InitCodec(&pCodec, Header.Method);
	if (pCodec == NULL)
		return FALSE;
#endif
	// allocate buffer for pack data
	PackBuf = m_MemStack.Push(Header.PackLen);

	// allocate bufer for real data
	DataBuf = m_MemStack.Push(Header.DataLen + 2);

	// read pack data for file
	File.Read(PackBuf, Header.PackLen);

	// decompress data
#ifndef _SERVER
	CodeInfo.lpPack = (PBYTE)PackBuf;
	CodeInfo.dwPackLen = Header.PackLen;
	CodeInfo.lpData = (PBYTE)DataBuf;
	CodeInfo.dwDataLen = Header.DataLen;
	pCodec->Decode(&CodeInfo);
	g_FreeCodec(&pCodec, Header.Method);
	if (CodeInfo.dwDataLen != Header.DataLen)
	{
		g_DebugLog("Ini file unpack fail : %s", FileName);
		return FALSE;
	}
#endif
	// check data length

	// create ini link in memory
	CreateIniLink(DataBuf, Header.DataLen);

	return TRUE;
}
//---------------------------------------------------------------------------
// º¯Êý:	Save
// ¹¦ÄÜ:	±£´æµ±Ç°µÄINIÎÄ¼þ
// ²ÎÊý:	void
// ·µ»Ø:	TRUE		³É¹¦
//			FALSE		Ê§°Ü
//---------------------------------------------------------------------------
BOOL KIniFile::Save(LPCSTR FileName)
{
	KFile		File;
	LPSTR		DataBuf;
	LPSTR		pBuffer;
	DWORD		dwLen;

	if (FileName[0] == 0)
		return FALSE;

	if (g_nCodec)
		return SavePack(FileName);

	dwLen = m_MemStack.GetStackSize();

	DataBuf = (LPSTR)m_MemStack.Push(dwLen);
	pBuffer = DataBuf;

	SECNODE* SecNode = m_Header.pNextNode;
	KEYNODE* KeyNode = NULL;

	// write ini file to memory buffer
	while (SecNode != NULL)
	{
		dwLen = sprintf(pBuffer, "%s\r\n", SecNode->pSection);
		pBuffer += dwLen;
		KeyNode = SecNode->pKeyNode.pNextNode;
		while (KeyNode != NULL)
		{
			dwLen = sprintf(pBuffer, "%s%c%s\r\n", KeyNode->pKey, '=',
				KeyNode->pValue);
			pBuffer += dwLen;
			KeyNode = KeyNode->pNextNode;
		}
		SecNode = SecNode->pNextNode;
		dwLen = sprintf(pBuffer, "\r\n");
		pBuffer += dwLen;
	}

	// get buffer len
	dwLen = pBuffer - DataBuf;

	// create ini file
	if (!File.Create((char*)FileName))
		return FALSE;

	// write ini file
	File.Write(DataBuf, dwLen);

	return TRUE;
}
//---------------------------------------------------------------------------
// º¯Êý:	SavePack
// ¹¦ÄÜ:	±£´æÒ»¸öÑ¹ËõµÄINIÎÄ¼þ
// ²ÎÊý:	FileName	ÎÄ¼þÃû
// ·µ»Ø:	TRUE		³É¹¦
//			FALSE		Ê§°Ü
//---------------------------------------------------------------------------
BOOL KIniFile::SavePack(LPCSTR FileName)
{
	KFile		File;
	PVOID		DataBuf;
	PVOID		PackBuf;
#ifndef _SERVER
	KCodec* pCodec;
	TCodeInfo	CodeInfo;
#endif
	TPackHead	Header;
	LPSTR		pBuffer;
	DWORD		dwLen;

	// check file name
	if (FileName[0] == 0)
		return FALSE;

	dwLen = m_MemStack.GetStackSize();

	// allocate realdata buffer
	DataBuf = m_MemStack.Push(dwLen);

	// allocate compress buffer
	PackBuf = m_MemStack.Push(dwLen + 256);

	// print ini file to realdata buffer
	pBuffer = (LPSTR)DataBuf;
	SECNODE* SecNode = m_Header.pNextNode;
	KEYNODE* KeyNode = NULL;

	// write ini file to memory buffer
	while (SecNode != NULL)
	{
		dwLen = sprintf(pBuffer, "%s\r\n", SecNode->pSection);
		pBuffer += dwLen;
		KeyNode = SecNode->pKeyNode.pNextNode;
		while (KeyNode != NULL)
		{
			dwLen = sprintf(pBuffer, "%s%c%s\r\n", KeyNode->pKey, '=',
				KeyNode->pValue);
			pBuffer += dwLen;
			KeyNode = KeyNode->pNextNode;
		}
		SecNode = SecNode->pNextNode;
		dwLen = sprintf(pBuffer, "\r\n");
		pBuffer += dwLen;
	}

	// set header
	Header.Id = 0x4b434150; // "PACK"
	Header.Method = g_nCodec; // CODEC_LZO = 2;

	// init codec
#ifndef _SERVER
	pCodec = NULL;
	g_InitCodec(&pCodec, Header.Method);
	if (pCodec == NULL)
		return FALSE;
#endif

	// compress the data buffer
#ifndef _SERVER
	CodeInfo.lpData = (PBYTE)DataBuf;
	CodeInfo.lpPack = (PBYTE)PackBuf;
	CodeInfo.dwDataLen = pBuffer - (LPCSTR)DataBuf;
	CodeInfo.dwPackLen = 0;
	pCodec->Encode(&CodeInfo);
	g_FreeCodec(&pCodec, Header.Method);
	Header.DataLen = CodeInfo.dwDataLen;
	Header.PackLen = CodeInfo.dwPackLen;
#endif

	// create ini file
	if (!File.Create((char*)FileName))
		return FALSE;

	// write file header
	File.Write(&Header, sizeof(Header));

	// write file data
	File.Write(PackBuf, Header.PackLen);

	return TRUE;
}
//---------------------------------------------------------------------------
// º¯Êý:	Clear
// ¹¦ÄÜ:	Çå³ýINIÎÄ¼þµÄÄÚÈÝ
// ²ÎÊý:	void
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::Clear()
{
	ReleaseIniLink();
}
//---------------------------------------------------------------------------
// º¯Êý:	ReadLine
// ¹¦ÄÜ:	¶ÁÈ¡INIÎÄ¼þµÄÒ»ÐÐ
// ²ÎÊý:	Buffer	»º´æ
//			Szie	³¤¶È
// ·µ»Ø:	TRUE		³É¹¦
//			FALSE		Ê§°Ü
//---------------------------------------------------------------------------
BOOL KIniFile::ReadLine(LPSTR Buffer, LONG Size)
{
	if (m_Offset >= Size)
	{
		return FALSE;
	}
	while (Buffer[m_Offset] != 0x0D && Buffer[m_Offset] != 0x0A)
	{
		m_Offset++;
		if (m_Offset >= Size)
			break;
	}
	Buffer[m_Offset] = 0;
	if (Buffer[m_Offset] == 0x0D && Buffer[m_Offset + 1] == 0x0A)
		m_Offset += 2;
	else
		m_Offset += 1;	//linux [wxb 2003-7-29]
	return TRUE;
}
//---------------------------------------------------------------------------
// º¯Êý:	IsKeyChar
// ¹¦ÄÜ:	ÅÐ¶ÏÒ»¸ö×Ö·ûÊÇ·ñÎª×ÖÄ¸
// ²ÎÊý:	ch		ÒªÅÐ¶ÏµÄ×Ö·û	
// ·µ»Ø:	TRUE	ÊÇ×ÖÄ¸
//			FALSE	²»ÊÇ×ÖÄ¸
//---------------------------------------------------------------------------
BOOL KIniFile::IsKeyChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return TRUE;
	if ((ch >= 'A') && (ch <= 'Z'))
		return TRUE;
	if ((ch >= 'a') && (ch <= 'z'))
		return TRUE;
	return FALSE;
}
//---------------------------------------------------------------------------
// º¯Êý:	CreateIniLink
// ¹¦ÄÜ:	´´½¨IniÁ´±í
// ²ÎÊý:	pBuffer		»º´æ
//			nBufLen		³¤¶È
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::CreateIniLink(LPVOID pBuffer, LONG nBufLen)
{
	LPSTR lpBuffer = (LPSTR)pBuffer;
	LPSTR lpString = NULL;
	LPSTR lpValue = NULL;
	char  szSection[64] = "[MAIN]";

	m_Offset = 0;
	while (m_Offset < nBufLen)
	{
		lpString = &lpBuffer[m_Offset];
		if (!ReadLine(lpBuffer, nBufLen))
			break;
		if (IsKeyChar(*lpString))
		{
			lpValue = SplitKeyValue(lpString);
			SetKeyValue(szSection, lpString, lpValue);
		}
		if (*lpString == '[')
		{
			g_StrCpyLen(szSection, lpString, sizeof(szSection));
		}
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	ReleaseIniLink()
// ¹¦ÄÜ:	ÊÍ·ÅIniÁ´±í
// ²ÎÊý:	void
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::ReleaseIniLink()
{
	SECNODE* pThisSec = &m_Header;
	SECNODE* pNextSec = pThisSec->pNextNode;
	KEYNODE* pThisKey = NULL;
	KEYNODE* pNextKey = NULL;

	while (pNextSec != NULL)
	{
		pThisSec = pNextSec->pNextNode;
		pThisKey = &pNextSec->pKeyNode;
		pNextKey = pThisKey->pNextNode;
		while (pNextKey != NULL)
		{
			pThisKey = pNextKey->pNextNode;
			m_MemStack.Free(pNextKey->pKey);
			m_MemStack.Free(pNextKey->pValue);
			m_MemStack.Free(pNextKey);
			pNextKey = pThisKey;
		}
		m_MemStack.Free(pNextSec->pSection);
		m_MemStack.Free(pNextSec);
		pNextSec = pThisSec;
	}
	m_Header.pNextNode = NULL;

	m_MemStack.FreeAllChunks();
}
//---------------------------------------------------------------------------
// º¯Êý:	SplitKeyValue
// ¹¦ÄÜ:	·Ö¸îKeyºÍValue
// ²ÎÊý:	pString		Key=Value
// ·µ»Ø:	Ö¸ÏòValue
//---------------------------------------------------------------------------
LPSTR KIniFile::SplitKeyValue(LPSTR pString)
{
	LPSTR pValue = pString;
	while (*pValue)
	{
		if (*pValue == '=')
			break;
		pValue++;
	}
	*pValue = 0;
	return pValue + 1;
}
//---------------------------------------------------------------------------
// º¯Êý:	String2Id
// ¹¦ÄÜ:	×Ö·û´®×ª³É32 bits ID
// ²ÎÊý:	pString		×Ö·û´®
// ·µ»Ø:	32 bits ID
//---------------------------------------------------------------------------
DWORD KIniFile::String2Id(LPCSTR pString)
{
	DWORD Id = 0;
	for (int32_t i = 0; pString[i]; i++)
	{
		Id = (Id + (i + 1) * pString[i]) % 0x8000000b * 0xffffffef;
	}
	return Id ^ 0x12345678;
}
//---------------------------------------------------------------------------
// º¯Êý:	IsSectionExist
// ¹¦ÄÜ:	SectionÊÇ·ñ´æÔÚ
// ²ÎÊý:	pSection	½ÚµÄÃû×Ö
// ·µ»Ø:	BOOL
//---------------------------------------------------------------------------
BOOL KIniFile::IsSectionExist(LPCSTR pSection)
{
	// setup section name
	char szSection[64] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}

	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
			return TRUE;
		pSecNode = pSecNode->pNextNode;
	}

	return FALSE;
}
//---------------------------------------------------------------------------
// º¯Êý:	EraseSection
// ¹¦ÄÜ:	Çå³ýÒ»¸öSectionµÄÄÚÈÝ
// ²ÎÊý:	pSection	½ÚµÄÃû×Ö
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::EraseSection(LPCSTR pSection)
{
	// setup section name
	char szSection[64] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}

	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
		{
			break;
		}
		pSecNode = pSecNode->pNextNode;
	}

	// if no such section found
	if (pSecNode == NULL)
	{
		return;
	}

	// erase all key in the section
	KEYNODE* pThisKey = &pSecNode->pKeyNode;
	KEYNODE* pNextKey = pThisKey->pNextNode;
	while (pNextKey != NULL)
	{
		pThisKey = pNextKey->pNextNode;
		m_MemStack.Free(pNextKey->pKey);
		m_MemStack.Free(pNextKey->pValue);
		m_MemStack.Free(pNextKey);
		pNextKey = pThisKey;
	}
	pSecNode->pKeyNode.pNextNode = NULL;
}

//---------------------------------------------------------------------------
// º¯Êý:	EraseKey
// ¹¦ÄÜ:	Çå³ýSectionµÄÏÂÒ»¸öKeyµÄÄÚÈÝ
// ²ÎÊý:	pSection	½ÚµÄÃû×Ö
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void	KIniFile::EraseKey(LPCSTR lpSection, LPCSTR lpKey)
{
	// setup section name
	char szSection[64] = "[";
	if (lpSection[0] != '[')
	{
		g_StrCat(szSection, lpSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, lpSection);
	}

	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
		{
			break;
		}
		pSecNode = pSecNode->pNextNode;
	}

	// if no such section found
	if (pSecNode == NULL)
	{
		return;
	}

	// erase all key in the section
	KEYNODE* pThisKey = &pSecNode->pKeyNode;
	KEYNODE* pNextKey = pThisKey->pNextNode;
	dwID = String2Id(lpKey);
	while (pNextKey != NULL)
	{

		if (pNextKey->dwID == dwID)
		{
			pThisKey->pNextNode = pNextKey->pNextNode;
			m_MemStack.Free(pNextKey->pKey);
			m_MemStack.Free(pNextKey->pValue);
			m_MemStack.Free(pNextKey);
			return;
		}
		pThisKey = pNextKey;
		pNextKey = pNextKey->pNextNode;
	}

}



//---------------------------------------------------------------------------
// º¯Êý:	SetKeyValue
// ¹¦ÄÜ:	ÉèÖÃKeyµÄValue
// ²ÎÊý:	pSection	½ÚÃû
//			pKey		½¨Ãû
//			pValue		½¨Öµ
// ·µ»Ø:	TRUE£­³É¹¦ FALSE£­Ê§°Ü
//---------------------------------------------------------------------------
BOOL KIniFile::SetKeyValue(
	LPCSTR	pSection,
	LPCSTR	pKey,
	LPCSTR	pValue)
{
	int32_t		nLen;
	DWORD	dwID;

	// setup section name
	char szSection[64] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}

	// search for the matched section
	SECNODE* pThisSecNode = &m_Header;
	SECNODE* pNextSecNode = pThisSecNode->pNextNode;
	dwID = String2Id(szSection);
	while (pNextSecNode != NULL)
	{
		if (dwID == pNextSecNode->dwID)
		{
			break;
		}
		pThisSecNode = pNextSecNode;
		pNextSecNode = pThisSecNode->pNextNode;
	}

	// if no such section found create a new section
	if (pNextSecNode == NULL)
	{
		nLen = g_StrLen(szSection) + 1;
		pNextSecNode = (SECNODE*)m_MemStack.Push(sizeof(SECNODE));
		pNextSecNode->pSection = (char*)m_MemStack.Push(nLen);
		g_MemCopy(pNextSecNode->pSection, szSection, nLen);
		pNextSecNode->dwID = dwID;
		pNextSecNode->pKeyNode.pNextNode = NULL;
		pNextSecNode->pNextNode = NULL;
		pThisSecNode->pNextNode = pNextSecNode;
	}

	// search for the same key
	KEYNODE* pThisKeyNode = &pNextSecNode->pKeyNode;
	KEYNODE* pNextKeyNode = pThisKeyNode->pNextNode;
	dwID = String2Id(pKey);
	while (pNextKeyNode != NULL)
	{
		if (dwID == pNextKeyNode->dwID)
		{
			break;
		}
		pThisKeyNode = pNextKeyNode;
		pNextKeyNode = pThisKeyNode->pNextNode;
	}

	// if no such key found create a new key
	if (pNextKeyNode == NULL)
	{
		pNextKeyNode = (KEYNODE*)m_MemStack.Push(sizeof(KEYNODE));

		nLen = g_StrLen(pKey) + 1;
		pNextKeyNode->pKey = (char*)m_MemStack.Push(nLen);
		g_MemCopy(pNextKeyNode->pKey, (void*)pKey, nLen);

		nLen = g_StrLen(pValue) + 1;
		pNextKeyNode->pValue = (char*)m_MemStack.Push(nLen);
		g_MemCopy(pNextKeyNode->pValue, (void*)pValue, nLen);

		pNextKeyNode->dwID = dwID;
		pNextKeyNode->pNextNode = NULL;
		pThisKeyNode->pNextNode = pNextKeyNode;
	}
	// replace the old value with new
	else
	{
		m_MemStack.Free(pNextKeyNode->pValue);
		nLen = g_StrLen(pValue) + 1;
		pNextKeyNode->pValue = (char*)m_MemStack.Push(nLen);
		g_MemCopy(pNextKeyNode->pValue, (void*)pValue, nLen);
	}
	return TRUE;
}
//---------------------------------------------------------------------------
// º¯Êý:	GetKeyValue
// ¹¦ÄÜ:	È¡µÃKeyµÄValue
// ²ÎÊý:	pSection	½ÚÃû
//			pKey		½¨Ãû
//			pValue		½¨Öµ
// ·µ»Ø:	TRUE£­³É¹¦ FALSE£­Ê§°Ü
//---------------------------------------------------------------------------
BOOL KIniFile::GetKeyValue(
	LPCSTR	pSection,
	LPCSTR	pKey,
	LPSTR	pValue,
	DWORD	dwSize)
{
	DWORD	dwID;

	// setup section name
	char szSection[64] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}

	// search for the matched section
	SECNODE* pSecNode = m_Header.pNextNode;
	dwID = String2Id(szSection);
	while (pSecNode != NULL)
	{
		if (dwID == pSecNode->dwID)
		{
			break;
		}
		pSecNode = pSecNode->pNextNode;
	}

	// if no such section founded
	if (pSecNode == NULL)
	{
		return FALSE;
	}

	// search for the same key
	KEYNODE* pKeyNode = pSecNode->pKeyNode.pNextNode;
	dwID = String2Id(pKey);
	while (pKeyNode != NULL)
	{
		if (dwID == pKeyNode->dwID)
		{
			break;
		}
		pKeyNode = pKeyNode->pNextNode;
	}

	// if no such key found
	if (pKeyNode == NULL)
	{
		return FALSE;
	}

	// copy the value of the key
	g_StrCpyLen(pValue, pKeyNode->pValue, dwSize);
	return TRUE;
}
//---------------------------------------------------------------------------
// º¯Êý:	GetString
// ¹¦ÄÜ:	¶ÁÈ¡Ò»¸ö×Ö·û´®
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			lpDefault		È±Ê¡Öµ
//			lpRString		·µ»ØÖµ
//			dwSize			·µ»Ø×Ö·û´®µÄ×î´ó³¤¶È
// ·µ»Ø:	void
//---------------------------------------------------------------------------
BOOL KIniFile::GetString(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	LPCSTR lpDefault,		// points to default string
	LPSTR lpRString,		// points to destination buffer
	DWORD dwSize			// size of string buffer
)
{
	if (GetKeyValue(lpSection, lpKeyName, lpRString, dwSize))
		return TRUE;
	g_StrCpyLen(lpRString, lpDefault, dwSize);
	return FALSE;
}
//---------------------------------------------------------------------------
// º¯Êý:	GetInteger
// ¹¦ÄÜ:	¶ÁÈ¡Ò»¸öÕûÊý
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			nDefault		È±Ê¡Öµ
//			pnValue			·µ»ØÖµ
// ·µ»Ø:	void
//---------------------------------------------------------------------------
BOOL KIniFile::GetInteger(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	int32_t   nDefault,			// default value
	int32_t* pnValue          // points to value
)
{
	char Buffer[64];
	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		*pnValue = atoi(Buffer);
		return TRUE;
	}
	else
	{
		*pnValue = nDefault;
		return FALSE;
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	GetInteger2
// ¹¦ÄÜ:	¶ÁÈ¡2¸öÕûÊý£¬ÕûÊýÖ®¼äÓÃ¶ººÅ·Ö¸î¡£
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			pnValue1		·µ»ØÖµ1
//			pnValue2		·µ»ØÖµ2
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::GetInteger2(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	int32_t* pnValue1,		// value 1
	int32_t* pnValue2			// value 2
)
{
	char  Buffer[64];
	char* pChar = Buffer;
	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		while (*pChar)
		{
			if (',' == *pChar)
			{
				*pChar++ = 0;
				break;
			}
			pChar++;
		}
		*pnValue1 = atoi(Buffer);
		*pnValue2 = atoi(pChar);
	}
}

void KIniFile::GetInteger3(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	int32_t* pnValue1,		// value 1
	int32_t* pnValue2,		// value 2
	int32_t* pnValue3			// value 3
)
{
	char  Buffer[64];
	char* pChar = Buffer;
	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		while (*pChar)
		{
			if (',' == *pChar)
			{
				*pChar++ = 0;
				break;
			}
			pChar++;
		}
		*pnValue1 = atoi(Buffer);
		*pnValue2 = atoi(pChar);

		pChar++;
		while (*pChar)
		{
			if (',' == *pChar)
			{
				*pChar++ = 0;
				break;
			}
			pChar++;
		}
		*pnValue3 = atoi(pChar);
	}
	else
	{
		*pnValue1 = 0;
		*pnValue2 = 0;
		*pnValue3 = 0;
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	GetFloat
// ¹¦ÄÜ:	¶ÁÈ¡1¸ö¸¡µãÊý
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			fDefault		È±Ê¡Öµ
//			pfValue			·µ»ØÖµ
// ·µ»Ø:	void
//---------------------------------------------------------------------------
BOOL KIniFile::GetFloat(
	LPCSTR	lpSection,		// points to section name
	LPCSTR	lpKeyName,		// points to key name
	float	fDefault,		// default value
	float* pfValue        // return value
)
{
	char Buffer[64];
	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		*pfValue = (float)atof(Buffer);
		return TRUE;
	}
	else
	{
		*pfValue = fDefault;
		return FALSE;
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	GetFloat2
// ¹¦ÄÜ:	¶ÁÈ¡2¸ö¸¡µãÊý£¬Ö®¼äÓÃ¶ººÅ·Ö¸î¡£
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			pfValue1		·µ»ØÖµ1
//			pfValue2		·µ»ØÖµ2
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::GetFloat2(
	LPCSTR lpSection,		// points to section name
	LPCSTR lpKeyName,		// points to key name
	float* pfValue1,		// value 1
	float* pfValue2			// value 2
)
{
	char  Buffer[64];
	char* pChar = Buffer;

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		while (*pChar)
		{
			if (',' == *pChar)
			{
				*pChar++ = 0;
				break;
			}
			pChar++;
		}
		*pfValue1 = (float)atof(Buffer);
		*pfValue2 = (float)atof(pChar);
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	GetStruct
// ¹¦ÄÜ:	¶ÁÈ¡Ò»¸ö½á¹¹
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			lpStruct		»º´æ
//			dwSize			»º´æ´óÐ¡
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::GetStruct(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	LPVOID	lpStruct,		// pointer to buffer that contains data to add
	DWORD	dwSize			// size, in bytes, of the buffer
)
{
	char    Buffer[512];
	LPBYTE	lpByte;
	BYTE	ah, al;

	if (!GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
		return;
	lpByte = (LPBYTE)lpStruct;
	int32_t len = g_StrLen(Buffer);
	if (len / 2 != (int32_t)dwSize)
		return;
	for (int32_t i = 0; i < len; i += 2)
	{
		// get byte high
		ah = Buffer[i];
		if ((ah >= 48) && (ah <= 57))
			ah = (BYTE)(ah - 48);
		else
			ah = (BYTE)(ah - 65 + 10);
		// get byte low
		al = Buffer[i + 1];
		if ((al >= 48) && (al <= 57))
			al = (BYTE)(al - 48);
		else
			al = (BYTE)(al - 65 + 10);
		// set struct bye
		*lpByte++ = (BYTE)(ah * 16 + al);
	}
}
//---------------------------------------------------------------------------
// º¯Êý:	WriteString
// ¹¦ÄÜ:	Ð´ÈëÒ»¸ö×Ö·û´®
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			lpString		×Ö·û´®
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::WriteString(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	LPCSTR	lpString		// pointer to string to add
)
{
	SetKeyValue(lpSection, lpKeyName, lpString);
}
//---------------------------------------------------------------------------
// º¯Êý:	WriteInteger
// ¹¦ÄÜ:	Ð´ÈëÒ»¸öÕûÊý
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			nValue			ÕûÊý
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::WriteInteger(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	int32_t 	nValue			// Integer to write
)
{
	char Buffer[64];

	sprintf(Buffer, "%d", nValue);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// º¯Êý:	WriteInteger2
// ¹¦ÄÜ:	Ð´Èë2¸öÕûÊý
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			nValue1			ÕûÊý1
//			nValue2			ÕûÊý2
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::WriteInteger2(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	int32_t 	Value1,			// value 1 to write
	int32_t		Value2			// value 2 to write
)
{
	char Buffer[64];

	sprintf(Buffer, "%d,%d", Value1, Value2);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}


void KIniFile::WriteInteger3(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	int32_t 	Value1,			// value 1 to write
	int32_t		Value2,			// value 2 to write
	int32_t		Value3			// value 3 to write
)
{
	char Buffer[64];

	sprintf(Buffer, "%d,%d,%d", Value1, Value2, Value3);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}

void KIniFile::WriteDword(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	DWORD 	nValue			// Integer to write
)
{
	char Buffer[64];

	sprintf(Buffer, "%u", nValue);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// º¯Êý:	WriteFloat
// ¹¦ÄÜ:	Ð´Èë1¸ö¸¡µãÊý
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			fValue			¸¡µãÊý
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::WriteFloat(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	float	fValue			// Integer to write
)
{
	char Buffer[64];

	sprintf(Buffer, "%f", fValue);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// º¯Êý:	WriteFloat2
// ¹¦ÄÜ:	Ð´Èë2¸ö¸¡µãÊý
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			fValue1			¸¡µãÊý1
//			fValue2			¸¡µãÊý2
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::WriteFloat2(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	float 	fValue1,		// value 1 to write
	float	fValue2			// value 2 to write
)
{
	char Buffer[64];

	sprintf(Buffer, "%f,%f", fValue1, fValue2);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
// º¯Êý:	WriteFloat2
// ¹¦ÄÜ:	Ð´ÈëÒ»¸ö½á¹¹
// ²ÎÊý:	lpSection		½ÚÃû
//			lpKeyName		½¨Ãû
//			lpStruct		½á¹¹
//			dwSize			½á¹¹´óÐ¡
// ·µ»Ø:	void
//---------------------------------------------------------------------------
void KIniFile::WriteStruct(
	LPCSTR	lpSection,		// pointer to section name
	LPCSTR	lpKeyName,		// pointer to key name
	LPVOID	lpStruct,		// pointer to buffer that contains data to add
	DWORD 	dwSize			// size, in bytes, of the buffer
)
{
	char    Buffer[512];
	LPSTR	lpBuff = Buffer;
	LPBYTE	lpByte;

	if (dwSize * 2 >= 512)
	{
		return;
	}
	lpByte = (LPBYTE)lpStruct;
	for (DWORD i = 0; i < dwSize; i++)
	{
		sprintf(lpBuff, "%02x", *lpByte);
		lpBuff++;
		lpBuff++;
		lpByte++;
	}
	g_StrUpper(Buffer);
	SetKeyValue(lpSection, lpKeyName, Buffer);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
BOOL KIniFile::GetNextSection(LPCSTR pSection, LPSTR pNextSection)
{

	if (!pSection[0])	//	´«Èë²ÎÊýÎª""Ê±¾ÍÊÇÈ¡µÚÒ»¸öSection
	{
		if (!m_Header.pNextNode)
			return FALSE;
		else
		{
			g_StrCpy(pNextSection, m_Header.pNextNode->pSection);
			return TRUE;
		}
	}
	else
	{
		char szSection[64] = "[";
		if (pSection[0] != '[')
		{
			g_StrCat(szSection, pSection);
			g_StrCat(szSection, "]");
		}
		else
		{
			g_StrCpy(szSection, pSection);
		}
		//	²éÕÒ²ÎÊý¸ø¶¨µÄSection
		SECNODE* pThisSecNode = &m_Header;
		SECNODE* pNextSecNode = pThisSecNode->pNextNode;
		DWORD dwID = String2Id(szSection);
		while (pNextSecNode != NULL)
		{
			if (dwID == pNextSecNode->dwID)
			{
				break;
			}
			pThisSecNode = pNextSecNode;
			pNextSecNode = pThisSecNode->pNextNode;
		}
		//	Ã»ÓÐÕâ¸öSection£¬ÎÞËùÎ½ÏÂÒ»¸ö
		if (!pNextSecNode)
		{
			return FALSE;
		}
		else
		{
			//	²ÎÊý¸ø¶¨µÄSectionÒÑ¾­ÊÇ×îºóÒ»¸öÁË
			if (!pNextSecNode->pNextNode)
			{
				return FALSE;
			}
			else
			{
				g_StrCpy(pNextSection, pNextSecNode->pNextNode->pSection);
				return TRUE;
			}
		}
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
BOOL KIniFile::GetNextKey(LPCSTR pSection, LPCSTR pKey, LPSTR pNextKey)
{
	char szSection[64] = "[";
	if (pSection[0] != '[')
	{
		g_StrCat(szSection, pSection);
		g_StrCat(szSection, "]");
	}
	else
	{
		g_StrCpy(szSection, pSection);
	}
	//	²éÕÒ²ÎÊý¸ø¶¨µÄSection
	SECNODE* pThisSecNode = &m_Header;
	SECNODE* pNextSecNode = pThisSecNode->pNextNode;
	DWORD dwID = String2Id(szSection);
	while (pNextSecNode != NULL)
	{
		if (dwID == pNextSecNode->dwID)
		{
			break;
		}
		pThisSecNode = pNextSecNode;
		pNextSecNode = pThisSecNode->pNextNode;
	}
	//	Ã»ÓÐÕâ¸öSection£¬ÎÞËùÎ½È¡Key
	if (!pNextSecNode)
	{
		return FALSE;
	}

	KEYNODE* pThisKeyNode = &pNextSecNode->pKeyNode;
	KEYNODE* pNextKeyNode = pThisKeyNode->pNextNode;

	//	Key¸øµÄ²ÎÊýÎª""µÄ»°£¬È¡µÚÒ»¸öKey
	if (!pKey[0])
	{
		//	Ã»ÓÐKey£¬·µ»ØÊ§°Ü
		if (!pNextKeyNode->pKey)
		{
			return FALSE;
		}
		else
		{
			g_StrCpy(pNextKey, pNextKeyNode->pKey);
			return TRUE;
		}
	}

	dwID = String2Id(pKey);
	while (pNextKeyNode != NULL)
	{
		if (dwID == pNextKeyNode->dwID)
		{
			break;
		}
		pThisKeyNode = pNextKeyNode;
		pNextKeyNode = pThisKeyNode->pNextNode;
	}
	//	ÕÒ²»µ½Ëù¸øµÄKey£¬ÎÞËùÎ½ÏÂÒ»¸ö
	if (!pNextKeyNode)
	{
		return FALSE;
	}
	//	²ÎÊý¸ø¶¨µÄKeyÒÑ¾­ÊÇ×îºóÒ»¸öÁË
	if (!pNextKeyNode->pNextNode)
	{
		return FALSE;
	}
	else
	{
		g_StrCpy(pNextKey, pNextKeyNode->pNextNode->pKey);
		return TRUE;
	}
}

int32_t	KIniFile::GetSectionCount()
{
	char Section[200];
	char Section1[200];
	if (!GetNextSection("", Section)) return 0;
	int32_t i = 1;
	while (1)
	{
		strcpy(Section1, Section);
		if (!GetNextSection(Section1, Section)) return i;
		i++;
	}
	return i;
}

// Èç¹û´ÓÎÄ¼þÖÐÃ»ÓÐÈ¡µ½¶ÔÓ¦µÄSection ºÍ KeyÔò pRectÀïÃæµÄÖµ²»±ä
void KIniFile::GetRect(LPCSTR lpSection, LPCSTR lpKeyName, RECT* pRect)
{
	char  Buffer[256];

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		const char* pcszTemp = Buffer;

		pRect->left = KSG_StringGetInt(&pcszTemp, 0);
		KSG_StringSkipSymbol(&pcszTemp, ',');
		pRect->top = KSG_StringGetInt(&pcszTemp, 0);
		KSG_StringSkipSymbol(&pcszTemp, ',');
		pRect->right = KSG_StringGetInt(&pcszTemp, 0);
		KSG_StringSkipSymbol(&pcszTemp, ',');
		pRect->bottom = KSG_StringGetInt(&pcszTemp, 0);
		//sscanf(Buffer, "%d,%d,%d,%d", &(pRect->left), &(pRect->top), &(pRect->right), &(pRect->bottom));
	}
}

void KIniFile::GetFloat3(LPCSTR lpSection, LPCSTR lpKeyName, float* pRect)
{
	char	Buffer[256];

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		int32_t r = sscanf(Buffer, "%f,%f,%f", &pRect[0], &pRect[1], &pRect[2]);
	}
}

void KIniFile::GetFloat4(LPCSTR lpSection, LPCSTR lpKeyName, float* pRect)
{
	char  Buffer[256];

	if (GetKeyValue(lpSection, lpKeyName, Buffer, sizeof(Buffer)))
	{
		int32_t r = sscanf(Buffer, "%f,%f,%f,%f", &pRect[0], &pRect[1], &pRect[2], &pRect[3]);
	}
}

void KIniFile::GetBool(LPCSTR lpSection, LPCSTR lpKeyName, BOOL* pBool)
{
	char  Buffer;

	if (GetKeyValue(lpSection, lpKeyName, &Buffer, 1))
	{
		if ('T' == Buffer)
			*pBool = TRUE;
		else
			*pBool = FALSE;
	}
}
