//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KPakList.cpp
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Pack Data List Class
//---------------------------------------------------------------------------
#include <windows.h>
#include "KWin32.h"
#include "KDebug.h"
#include "KFilePath.h"
#include "KIniFile.h"
#include "KPakList.h"
#include "crtdbg.h"

//---------------------------------------------------------------------------
ENGINE_API KPakList* g_pPakList = NULL;

//---------------------------------------------------------------------------
// ¹¦ÄÜ:	¹ºÔìº¯Êý
//---------------------------------------------------------------------------
KPakList::KPakList()
{
	g_pPakList = this;
	m_nPakNumber = 0;
}

//---------------------------------------------------------------------------
// ¹¦ÄÜ:	·ÖÔìº¯Êý
//---------------------------------------------------------------------------
KPakList::~KPakList()
{
	Close();
}

//---------------------------------------------------------------------------
// ¹¦ÄÜ:	¹Ø±ÕËùÓÐÎÄ¼þ
//---------------------------------------------------------------------------
void KPakList::Close()
{
	for (int i = 0; i < m_nPakNumber; i++)
		delete m_PakFilePtrList[i];
	m_nPakNumber = 0;
}

//---------------------------------------------------------------------------
// ¹¦ÄÜ:	ÔÚËùÓÐ°üÖÐÉ¨ÃèÖ¸¶¨ÎÄ¼þ
// ²ÎÊý:	uId			ÎÄ¼þÃûID
//			ElemRef		ÓÃÓÚ´æ·Å£¨´«³ö£©ÎÄ¼þÐÅÏ¢
// ·µ»Ø:	ÊÇ·ñ³É¹¦ÕÒµ½
//---------------------------------------------------------------------------
bool KPakList::FindElemFile(unsigned long uId, XPackElemFileRef& ElemRef)
{
	bool bFounded = false;
	for (int i = 0; i < m_nPakNumber; i++)
	{
		if (m_PakFilePtrList[i]->XFindElemFile(uId, ElemRef)) //Find and read files in the package
		{
			bFounded = true;
			break;
		}
	}
	return bFounded;
}


//---------------------------------------------------------------------------
// ¹¦ÄÜ:	°ÑÎÄ¼þÃû×ª»»Îª°üÖÐµÄid
// ²ÎÊý:	pszFileName	ÎÄ¼þÃû
// ·µ»Ø:	ÎÄ¼þÃû¶ÔÓ¦µÄ°üÖÐµÄid
//---------------------------------------------------------------------------
unsigned long KPakList::FileNameToId(const char* pszFileName)
{
	_ASSERT(pszFileName && pszFileName[0]);
	unsigned long id = 0;
	const char* ptr = pszFileName;
	int index = 0;
	while (*ptr)
	{
		if (*ptr >= 'A' && *ptr <= 'Z') id = (id + (++index) * (*ptr + 'a' - 'A')) % 0x8000000b * 0xffffffef;
		else id = (id + (++index) * (*ptr)) % 0x8000000b * 0xffffffef;
		ptr++;
	}
	return (id ^ 0x12345678);
}

//---------------------------------------------------------------------------
// ¹¦ÄÜ:	ÔÚËùÓÐ°üÖÐÉ¨ÃèÖ¸¶¨ÎÄ¼þ
// ²ÎÊý:	pszFileName	ÎÄ¼þÃû
//			ElemRef	ÓÃÓÚ´æ·Å£¨´«³ö£©ÎÄ¼þÐÅÏ¢
// ·µ»Ø:	ÊÇ·ñ³É¹¦ÕÒµ½
//---------------------------------------------------------------------------
bool KPakList::FindElemFile(const char* pszFileName, XPackElemFileRef& ElemRef)
{
	bool bFounded = false;
	if (pszFileName && pszFileName[0])
	{
		char szPackName[128];
#ifdef WIN32
		szPackName[0] = '\\';
#else
		szPackName[0] = '/';
#endif
		g_GetPackPath(szPackName + 1, (char*)pszFileName);
		unsigned long uId = FileNameToId(szPackName);
		bFounded = FindElemFile(uId, ElemRef);
	}
	return bFounded;
}

//--------------------------------------------------------------------
//Function: Open package ini file
//Parameter: char*filename
//Return: BOOL
//-------------------------------------------------------------------------
int KPakList::Open(const char* pPakListFile)
{
	Close();

	KIniFile IniFile;
#define	SECTION "Package"

	bool bResult = false;
	if (IniFile.Load(pPakListFile))
	{
		char	szBuffer[32], szKey[16], szFile[MAX_PATH];

		if (IniFile.GetString(SECTION, "Path", "\\data", szBuffer, sizeof(szBuffer)))
		{
			g_GetFullPath(szFile, szBuffer);
			int nNameStartPos = strlen(szFile);
			if (szFile[nNameStartPos - 1] != '\\' && szFile[nNameStartPos - 1] != '/')
			{
#ifdef WIN32
				szFile[nNameStartPos++] = '\\';
#else
				szFile[nNameStartPos++] = '/';
#endif

				szFile[nNameStartPos] = 0;
			}

			for (int i = 0; i < MAX_PAK; ++i)   //PAK数量限制
			{
				itoa(i, szKey, 10);
				if (!IniFile.GetString(SECTION, szKey, "", szBuffer, sizeof(szBuffer)))
					continue;
				//					break;
				if (szBuffer[0] == 0)
					continue;
				//					break;
				strcpy(szFile + nNameStartPos, szBuffer);
				m_PakFilePtrList[m_nPakNumber] = new XPackFile;  //分配内存

				//printf("--(%s)PAK包分配内存:%u b..--\n",szBuffer,sizeof(XPackFile));

				if (m_PakFilePtrList[m_nPakNumber])
				{
					if (m_PakFilePtrList[m_nPakNumber]->Open(szFile, m_nPakNumber)) //检测是否能打开PAK包
					{
						m_nPakNumber++;
						//						g_DebugLog("PakList Open : %s ... Ok", szFile);
					}
					/*					else if (m_PakFilePtrList[m_nPakNumber]->OpenX(szFile, m_nPakNumber)) //检测是否能打开PAK包
										{
											 m_nPakNumber++;
										}*/
					else
					{
						delete (m_PakFilePtrList[m_nPakNumber]);
						m_PakFilePtrList[m_nPakNumber] = NULL;
					}
				}
			}
			bResult = true;
		}
	}
	IniFile.Clear();
	return m_nPakNumber;
}

//¶ÁÈ¡°üÄÚµÄ×ÓÎÄ¼þ
int KPakList::ElemFileRead(XPackElemFileRef& ElemRef,
	void* pBuffer, unsigned uSize)
{
	if (this != NULL && ElemRef.nPackIndex >= 0 && ElemRef.nPackIndex < m_nPakNumber)
		return m_PakFilePtrList[ElemRef.nPackIndex]->XElemFileRead(ElemRef, pBuffer, uSize);
	return 0;
}

//¶ÁÈ¡sprÎÄ¼þÍ·²¿»òÕû¸öspr
SPRHEAD* KPakList::GetSprHeader(XPackElemFileRef& ElemRef, SPROFFS*& pOffsetTable)
{
	if (ElemRef.nPackIndex >= 0 && ElemRef.nPackIndex < m_nPakNumber)
		return (m_PakFilePtrList[ElemRef.nPackIndex]->GetSprHeader(ElemRef, pOffsetTable));
	return NULL;
}

//¶ÁÈ¡°´Ö¡Ñ¹ËõµÄsprµÄÒ»Ö¡µÄÊý¾Ý
SPRFRAME* KPakList::GetSprFrame(int nPackIndex, SPRHEAD* pSprHeader, int nFrame)
{
	if (nPackIndex >= 0 && nPackIndex < m_nPakNumber)
		return m_PakFilePtrList[nPackIndex]->GetSprFrame(pSprHeader, nFrame);
	return NULL;
}
