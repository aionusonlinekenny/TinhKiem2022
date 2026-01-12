/*****************************************************************************************
//	ÎÄ×Ö´®´¦Àí
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-31
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include "KDebug.h"
#include "../../Represent/iRepresent/KRepresentUnit.h"
#include <string.h>
#ifndef __linux
#include <crtdbg.h>
#else
#define HRESULT	long
#define S_OK 0
#endif

#include "Text.h"
#define		MAX_ENCODED_CTRL_LEN	4	//µ¥¸ö¿ØÖÆ·û£¨¿ØÖÆ±êÊ¶+¿ØÖÆ²ÎÊý£©µÄ×î´ó´æ´¢³¤¶È


IInlinePicEngineSink* g_pIInlinePicSink = NULL;	//Ç¶ÈëÊ½Í¼Æ¬µÄ´¦Àí½Ó¿Ú[wxb 2003-6-19]
extern "C" ENGINE_API HRESULT 
AdviseEngine(IInlinePicEngineSink* pSink)
{
	_ASSERT(NULL == g_pIInlinePicSink);	//Ò»°ã²»»á¹Ò½ÓÁ½´Î
	g_pIInlinePicSink = pSink;
	return S_OK;
}

extern "C" ENGINE_API HRESULT 
UnAdviseEngine(IInlinePicEngineSink* pSink)
{
	if (pSink == g_pIInlinePicSink)
		g_pIInlinePicSink = NULL;
	return S_OK;
}

//¼ì²âÄ³¸ö×Ö·ûÊÇ·ñÎª²»Ðí·ÅÖÃÐÐÊ×µÄ×Ö·û
#define	NUM_CHARACTER_IN_A1	11
unsigned char	s_NotAllowAtLineHeadA1Characters[NUM_CHARACTER_IN_A1] = 
{
	 //¡¢¡£¡¯¡³¡µ¡·¡¹¡»¡½¡¿
	0xa2, 0xa3, 0xaf, 0xb1, 0xb3, 0xb5, 0xb7, 0xb9, 0xbb, 0xbd, 0xbf
};
#define	NUM_CHARACTER_IN_A3	10
unsigned char	s_NotAllowAtLineHeadA3Characters[NUM_CHARACTER_IN_A3] = 
{
	//£¡   £©    £¬   £®     £º    £»   £¾    £¿    £Ý    £ý
	0xa1, 0xa9, 0xac, 0xae, 0xba, 0xbb, 0xbe, 0xbf, 0xdd, 0xfd
};
#define	NUM_CHARACTER_IN_00	7
unsigned char	s_NotAllowAtLineHead00Characters[NUM_CHARACTER_IN_00] =
{
	//!),.:;>?
	0x21, 0x29, 0x2c, /*0x2e,*/ 0x3a, 0x3b, 0x3e, 0x3f
};
extern "C" ENGINE_API
void TReplaceText(char* pBuffer, const char* pszName1, const char* pszName2)
{
	int nMsgLen=strlen(pBuffer);
	int nMsgLen1=strlen(pszName1);
	int nMsgLen2=strlen(pszName2);
	int i=0,j=0,pst=0;
	while(pBuffer[i])
	{
		pst=i;
		j=0;
		while (pszName1[j] && pBuffer[pst]==pszName1[j]) {pst++;j++;}
		if (j==nMsgLen1)
		{
			memmove(pBuffer+i+nMsgLen2,pBuffer+i+nMsgLen1,nMsgLen-i-nMsgLen1+1);
			if (nMsgLen2<nMsgLen1)
			{
				memcpy(pBuffer+i,pszName2,nMsgLen2);
				nMsgLen-=nMsgLen1-nMsgLen2;
				i+= nMsgLen2;
			}
			else
			{
				memcpy(pBuffer+i,pszName2,nMsgLen2);
				nMsgLen+=nMsgLen2-nMsgLen1;
				i+=nMsgLen2;
			}
		}
		else i++;
	}
}

extern "C" ENGINE_API
unsigned int TGetColor(const char* pColor)
{
	if (pColor == NULL)
		return false;

	unsigned int Color = 0xFF000000;

	char Buf[16] = "";
	int  i = 0;
	int  n = 0;
	while (pColor[i] != ',')
	{
		if (pColor[i] == 0 || n >= 15)
			return atoi(pColor);
		Buf[n++] = pColor[i++];
	}
	
	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 16);
	n = 0;
	i++;
	while (pColor[i] != ',')
	{

		if (pColor[i] == 0 || n >= 15)
			return Color;
		Buf[n++] = pColor[i++];
	}
	Buf[n] = 0;
	Color += ((atoi(Buf) & 0xFF) << 8);
	n = 0;
	i++;
	while (pColor[i] != 0)
	{
		if (n >= 15)
			return Color;
		Buf[n++] = pColor[i++];
	}
	Buf[n] = 0;
	Color += (atoi(Buf) & 0xFF);
	return Color;
}

//¼ì²âÄ³¸ö×Ö·ûÊÇ·ñÎª²»Ðí·ÅÖÃÐÐÊ×µÄ×Ö·û£¬²»ÊÇÏÞÖÆ×Ö·ûÔò·µ»Ø0£¬·ñÔò·µ»Ø×Ö·ûÕ¼µÄ×Ó½ÚÊý
extern "C" ENGINE_API
int TIsCharacterNotAlowAtLineHead(const char* pCharacter)
{
	int				i;
	unsigned char	cChar;
	cChar = (unsigned char)(*pCharacter);
	if (cChar == 0xa3)
	{
		cChar = (unsigned char)pCharacter[1];
		if (cChar >= 0xa1 && cChar <= 0xfd)
		{
			for (i = 0; i < NUM_CHARACTER_IN_A3; i++)
				if (s_NotAllowAtLineHeadA3Characters[i] == cChar)
					return 2;
		}
	}
	else if (cChar == 0xa1)
	{
		cChar = (unsigned char)pCharacter[1];
		if (cChar >= 0xa2 && cChar <= 0xbf)
		{
			for (i = 0; i < NUM_CHARACTER_IN_A1; i++)
				if (s_NotAllowAtLineHeadA1Characters[i] == cChar)
					return 2;
		}
	}
	else if (cChar >= 0x21 && cChar <= 0x3f)
	{
		for (i = 0; i < NUM_CHARACTER_IN_00; i++)
			if (s_NotAllowAtLineHead00Characters[i] == cChar)
				return 1;
	}
	return false;
}

//»ñÈ¡±¾ÐÐµÄÏÂ¸öÏÔÊ¾×Ö·û
extern "C" ENGINE_API
const char* TGetSecondVisibleCharacterThisLine(const char* pCharacter, int nPos, int nLen)
{
	if (pCharacter && nLen > 0)
	{
		bool bFoundFirst = false;
		while(nPos < nLen)
		{
			unsigned char cChar = (unsigned char)(pCharacter[nPos]);
			if (cChar >= 0x20)
			{
				if (bFoundFirst)
					return (pCharacter + nPos);
				bFoundFirst = true;
				if (cChar > 0x80)
					nPos ++;
				else
					nPos ++;
				continue;
			}
			if (cChar == KTC_COLOR || cChar == KTC_BORDER_COLOR)
				nPos += 4;
			else if (cChar == KTC_INLINE_PIC)
				nPos += 1 + sizeof(WORD);
			else if (cChar == KTC_COLOR_RESTORE || cChar == KTC_BORDER_RESTORE)
				nPos++;
			break;
		};
	}
	return NULL;
}


//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÑ°ÕÒ·Ö¸î×Ö·û´®µÄºÏÊÊÎ»ÖÃ
//	²ÎÊý£ºpString    --> ÏëÒª·Ö¸îµÄ×Ö·û´®
//		¡¡nDesirePos --> ÆÚÍû·Ö¸îµÄÎ»ÖÃ£¨ÒÔ×Ö½ÚÎªµ¥Î»£©
//		  bLess      --> Èç¹ûÆÚÍû·Ö¸îµÄÎ»ÖÃ´¦ÓÚÒ»¸ö×Ö·û±àÂëµÄÖÐ¼äÊ±£¬½á¹ûÎ»ÖÃ
//					ÎªÇ°¿¿»¹ÊÇºó¿¿£¬0: Ïòºó¿¿; ·Ç0: ÏòÇ°¿¿¡£
//	×¢ÊÍ£ºChinese GBK±àÂë°æ±¾£¬´Ë×Ö·û´®ÖÐ×Ö·ûÈ«²¿ÊÓÎªÏÔÊ¾×Ö·û£¬²»°üº¬¿ØÖÆ×Ö·û
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int TSplitString(const char* pString, int nDesirePos, int bLess)
{
	register int	nPos = 0;
	if (pString)
	{
		nDesirePos -= 2;
		while(nPos < nDesirePos)
		{
			if ((unsigned char)pString[nPos] > 0x80)
				nPos++;
			else if (pString[nPos])
				nPos++;
			else
				break;
		};
		nDesirePos += 2;
		while(nPos < nDesirePos)
		{
			if ((unsigned char)pString[nPos] > 0x80)			
				nPos ++;
			else if (pString[nPos])
				nPos ++;
			else
				break;
		}
	}
	return nPos;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔÚ±àÂë×Ö´®Ñ°ÕÒ·Ö¸î×Ö·û´®µÄºÏÊÊÎ»ÖÃ
//	²ÎÊý£ºpString    --> ÏëÒª·Ö¸îµÄ×Ö·û´®
//		  nCount	 -->×Ö·û´®ÄÚÈÝµÄ³¤¶È£¨ÒÔ×Ö½ÚÎªµ¥Î»£©
//		¡¡nDesirePos --> ÆÚÍû·Ö¸îµÄÎ»ÖÃ£¨ÒÔ»º³åÇý´æ´¢×Ö½ÚÎªµ¥Î»£©
//		  bLess      --> Èç¹ûÆÚÍû·Ö¸îµÄÎ»ÖÃ´¦ÓÚÒ»¸öÖÐÎÄ×Ö·û±àÂëµÄÖÐ¼äÊ±£¬
//						½á¹ûÎ»ÖÃÎªÇ°¿¿»¹ÊÇºó¿¿£¬0: Ïòºó¿¿; ·Ç0: ÏòÇ°¿¿¡£
//	×¢ÊÍ£ºChinese GBK±àÂë°æ±¾£¬´Ë×Ö·û´®ÖÐ¿É°üº¬ÒÑ¾­±àÂëµÄ¿ØÖÆ·û
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int	TSplitEncodedString(const char* pString, int nCount, int nDesirePos, int bLess)
{
	int	nPos = 0;
	if (pString)
	{
		if (nDesirePos <= nCount)
		{
			register unsigned char cCharacter;
			nDesirePos -= MAX_ENCODED_CTRL_LEN;
			while (nPos < nDesirePos)
			{
				cCharacter = (unsigned char)pString[nPos];
				if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
					nPos += 4;
				else if (cCharacter == KTC_INLINE_PIC)
					nPos += 3;//1 + sizeof(WORD);
				else
					nPos ++;
			}
			nPos += MAX_ENCODED_CTRL_LEN;
			while(nPos < nDesirePos)
			{
				cCharacter = (unsigned char)pString[nPos];
				if (cCharacter > 0x80)
				{
					nPos ++;
				}				
				else if (cCharacter == KTC_INLINE_PIC)
				{
					if (bLess && nPos + 3 > nDesirePos)
						break;
					if (nPos + 3 >= nCount)
					{
						nPos = nCount;
						break;
					}
					nPos += 3;//1 + sizeof(WORD);
				}
				else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				{
					if (bLess && (nPos + 4 > nDesirePos))
						break;
					if (nPos + 4 >= nCount)
					{
						nPos = nCount;
						break;
					}
					nPos += 4;
				}
				else
					nPos ++;
			}

		}
		else
		{
			nPos = nCount;
		}
	}
	return nPos;
	
}


//×Ö·û»»¿ØÖÆÂëµÄ×Ö·û±ê¼Ç±íÊ¾ÓëÄÚ²¿±àÂëµÄ¶ÔÓ¦½á¹¹
#define	KTC_CTRL_CODE_MAX_LEN	7
typedef struct _KCtrlTable
{
	char    szCtrl[KTC_CTRL_CODE_MAX_LEN + 1];	//×Ö·û»»¿ØÖÆÂëµÄ×Ö·û±íÊ¾
	short	nCtrlLen;							//×Ö·û»»¿ØÖÆÂëµÄ×Ö·û±íÊ¾µÄ³¤¶È
	short   nCtrl;								//×Ö·û»»¿ØÖÆÂëµÄÄÚ²¿±àÂë
}KCtrlTable;

//ÑÕÉ«½á¹¹
typedef struct _KColorTable
{
	char			Token[8];		//ÑÕÉ«µÄ×Ö·û±íÊ¾
	unsigned char	Red;			//ÑÕÉ«µÄR·ÖÁ¿
	unsigned char	Green;			//ÑÕÉ«µÄG·ÖÁ¿
	unsigned char	Blue;			//ÑÕÉ«µÄB·ÖÁ¿
}KColorTable;

//¿ØÖÆÂëÁÐ±í
static	const KCtrlTable	s_CtrlTable[] =
{	
	{ "enter",	5, KTC_ENTER		},
	{ "color",	5, KTC_COLOR		},
	{ "bclr",	4, KTC_BORDER_COLOR	},
	{ "pic",	3, KTC_INLINE_PIC	},
};

//¿ØÖÆÂëµÄÊýÄ¿
static	const int	s_nCtrlCount = sizeof(s_CtrlTable)/sizeof(KCtrlTable);

//ÑÕÉ«ÁÐ±í
static	const KColorTable	s_ColorTable[] =
{
	{ "Black",	0,		0,		0	},
	{ "White",	255,	255,	255	},
	{ "Red",	255,	0,		0	},
	{ "Green",	0,		255,	0	},
	{ "Blue",	100,	100,	255 },
	{ "Yellow",	255,	255,	0	},
	{ "Orange",	252,	171,	58	},
	{ "Pink",	181,	0,		181	},
	{ "Violet", 170,	30,		255	},
	{ "Cyan",	0,		255,	255	},
	{ "Metal",	246,	255,	117	},
	{ "Wood",	0,		255,	120	},
	{ "Water",	78,		124,	255	},
	{ "Fire",	255,	90,		0	},
	{ "Earth",	254,	207,	179	},
	{ "DBlue",	120,	120,	120 },
	{ "HBlue",	100,	100,	255 },
	{ "Gold",	222,	225, 	165	},
	{ "Gray",	189,	190, 	189},
};

//ÑÕÉ«µÄÊýÄ¿
static	const int	s_nColorCount = sizeof(s_ColorTable)/sizeof(KColorTable);

//Ç¶ÈëÍ¼Æ¬[wxb 2003-6-19]
#define MAXPICTOKENLEN	16


static bool TEncodeCtrl(char* pBuffer, int nCount, int& nReadPos, int& nShortCount);
static int  TEncodeCtrl(int nCtrl, char* pParamBuffer, int nParamLen, char* pEncodedBuffer);

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¶ÔÎÄ±¾´®ÖÐµÄ¿ØÖÆ±ê¼Ç½øÐÐ×ª»»£¬È¥³ýÎÞÐ§×Ö·û£¬Ëõ¶ÌÎÄ±¾´®´æ´¢³¤¶È
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int	TEncodeText(char* pBuffer, int nCount, int* nCurLen/* = 0*/)
{
	int nShortCount = 0;
	int nPrefixSize = 0;
	if (pBuffer)
	{
		unsigned char	cCharacter;
		int		nReadPos = 0;		
		while(nReadPos < nCount)
		{
			cCharacter = pBuffer[nReadPos];
			if (cCharacter > 0x80)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos ++;
			}
			else if (cCharacter == 0x0d)	//»»ÐÐ
			{
				if (nReadPos + 1 < nCount && pBuffer[nReadPos + 1] == 0x0a)
					nReadPos += 2;
				else
					nReadPos ++;
				pBuffer[nShortCount++] = 0x0a;
			}
			else if (pBuffer[nReadPos] == '<')
			{
				int nOldCount = nShortCount;
				TEncodeCtrl(pBuffer, nCount, nReadPos, nShortCount);
				nPrefixSize += nShortCount - nOldCount;
			}
			else if((cCharacter >= 0x20 && cCharacter < 0x7F) ||
				cCharacter == 0x0a || cCharacter == 0x09)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos++;
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				memmove((pBuffer + nShortCount), (pBuffer + nReadPos), 3);
				nShortCount += 3;
				nReadPos += 3;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
				int nTemp = *(int*)(pBuffer + nReadPos);
				*(int*)(pBuffer + nShortCount) = nTemp;
				nShortCount += 4;
				nReadPos += 4;
			}
			else
				nReadPos++;
		}
		if (nShortCount <nCount)
			pBuffer[nShortCount] = 0;
		if (nCurLen)
			*nCurLen = nShortCount - nPrefixSize;
	}
	return nShortCount;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¶ÔÎÄ±¾´®ÖÐµÄ¿ØÖÆ±ê¼Ç½øÐÐ×ª»»£¬È¥³ýÎÞÐ§×Ö·û£¬Ëõ¶ÌÎÄ±¾´®´æ´¢³¤¶È
//--------------------------------------------------------------------------
extern "C" ENGINE_API
int TFilterEncodedText(char* pBuffer, int nCount)
{
	int nShortCount = 0;
	if (pBuffer)
	{
		unsigned char	cCharacter;
		int nReadPos = 0;
		while(nReadPos < nCount)
		{
			cCharacter = pBuffer[nReadPos];
			if (cCharacter > 0x80)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos++;
			}
			else if ((cCharacter >= 0x20 && cCharacter < 0x7F) ||
				cCharacter == 0x0a || cCharacter == 0x09)
			{
				pBuffer[nShortCount++] = cCharacter;
				nReadPos++;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
				if (nReadPos + 4 < nCount)
				{
					*(int*)(pBuffer + nShortCount) = *(int*)(pBuffer + nReadPos);
					nShortCount += 4;
					nReadPos += 4;
				}
				else
				{
					nReadPos++;
					break;
				}
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				if ((int)(nReadPos + 1 + sizeof(WORD)) < nCount)
				{
					memcpy(pBuffer + nShortCount, pBuffer + nReadPos, 1 + sizeof(WORD));;
					nShortCount += 1 + sizeof(WORD);
					nReadPos += 1 + sizeof(WORD);
				}
				else
				{
					nReadPos++;
					break;
				}
			}
			else
				nReadPos ++;
		}
		if (nShortCount < nCount)
			pBuffer[nShortCount] = 0;
	}

	return nShortCount;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º·ÖÎö×ª»»¿ØÖÆ·û
//--------------------------------------------------------------------------
static bool TEncodeCtrl(char* pBuffer, int nCount, int& nReadPos, int& nShortCount)
{

	_ASSERT(pBuffer != NULL && nReadPos < nCount && nShortCount < nCount && nShortCount <= nReadPos);
	
	int nCtrlCodeSize, nEndPos, nCtrl;

	//Ñ°ÕÒ½áÊø·ûºÅ'='µÄÎ»ÖÃ»ò'>'µÄÎ»ÖÃ
	int nEqualPos = nReadPos + 1;
	for (; nEqualPos < nCount && nEqualPos <= nReadPos + KTC_CTRL_CODE_MAX_LEN; nEqualPos++)
		if (pBuffer[nEqualPos] == '>' || pBuffer[nEqualPos] == '=')
			break;	

	if(nEqualPos >= nCount || nEqualPos > nReadPos + KTC_CTRL_CODE_MAX_LEN)
		goto NO_MATCHING_CTRL;	//Î´ÕÒµ½'='»òÕß'>'½áÊø·ûºÅ

	nCtrlCodeSize = nEqualPos - nReadPos - 1;	//¿ØÖÆÃüÁî·ûºÅµÄ³¤¶È

	for (nCtrl = 0; nCtrl < s_nCtrlCount; nCtrl++)
	{
		if (nCtrlCodeSize ==  s_CtrlTable[nCtrl].nCtrlLen &&
			memcmp(pBuffer + nReadPos + 1, s_CtrlTable[nCtrl].szCtrl, nCtrlCodeSize) == 0)
			break;
	}
	if (nCtrl >= s_nCtrlCount)		//Î´ÕÒµ½Æ¥ÅäÒ»ÖÂµÄ¿ØÖÆÃüÁî
		goto NO_MATCHING_CTRL;
	nCtrl = s_CtrlTable[nCtrl].nCtrl;

	nEndPos = nEqualPos;
	if (pBuffer[nEqualPos] != '>')
	{
		for(nEndPos++; nEndPos < nCount; nEndPos++)
			if (pBuffer[nEndPos] == '>')
				break;
		if (nEndPos >= nCount)
			goto NO_MATCHING_CTRL;
		nShortCount += TEncodeCtrl(nCtrl, pBuffer + nEqualPos + 1,
			nEndPos - nEqualPos - 1, pBuffer + nShortCount);
	}
	else
		nShortCount += TEncodeCtrl(nCtrl, NULL, 0, pBuffer + nShortCount);
	nReadPos = nEndPos + 1;
	return true;

NO_MATCHING_CTRL:
	pBuffer[nShortCount++] = '<';
	nReadPos++;
	return false;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º×ª»»²¢´æ´¢¿ØÖÆÃüÁî¼°¸÷¿ØÖÆ²ÎÊý
//--------------------------------------------------------------------------
static int TEncodeCtrl(int nCtrl, char* pParamBuffer, int nParamLen, char* pEncodedBuffer)
{
	_ASSERT(pEncodedBuffer && (nParamLen == 0 || pParamBuffer != NULL));

	int nEncodedSize = 0;
	static char	Color[16];
	static char	szPic[MAXPICTOKENLEN];

	switch(nCtrl)
	{
	case KTC_ENTER:
		pEncodedBuffer[nEncodedSize ++] = nCtrl;
		break;
	case KTC_INLINE_PIC:	//[wxb 2003-6-19]
		if (nParamLen == 0 && nParamLen >= MAXPICTOKENLEN)
			break;
		{
			memcpy(szPic, pParamBuffer, nParamLen);
			szPic[nParamLen] = 0;
			int nPicIndex = atoi(szPic);
			if(nPicIndex >= MAX_SYSTEM_INLINE_PICTURES)
				break;
			pEncodedBuffer[nEncodedSize] = KTC_INLINE_PIC;
			*((WORD*)(pEncodedBuffer + nEncodedSize + 1)) = nPicIndex;
			nEncodedSize += 1 + sizeof(WORD);
		}		
		break;
	case KTC_COLOR:
		{
		if (nParamLen == 0)
		{
			pEncodedBuffer[nEncodedSize ++] = KTC_COLOR_RESTORE;
			return nEncodedSize;
		}
		else if (nParamLen < 8)
		{
			memcpy(Color, pParamBuffer, nParamLen);
			Color[nParamLen] = 0;
			for (int i = 0; i < s_nColorCount; i++)
			{
#ifndef __linux
				if (stricmp(Color,s_ColorTable[i].Token) == 0)
#else
				if(strcasecmp(Color,s_ColorTable[i].Token) == 0)
#endif
				{
					pEncodedBuffer[nEncodedSize] = KTC_COLOR;
					pEncodedBuffer[nEncodedSize + 1]= s_ColorTable[i].Red;
					pEncodedBuffer[nEncodedSize + 2]= s_ColorTable[i].Green;
					pEncodedBuffer[nEncodedSize + 3]= s_ColorTable[i].Blue;
					nEncodedSize += 4;
					return nEncodedSize;
				}
			}
		}
		memcpy(Color, pParamBuffer, nParamLen);
		if (nParamLen < sizeof(Color) / sizeof(Color[0])) {
			Color[nParamLen] = 0;
		}
		else {
			// Xử lý khi chỉ số nParamLen vượt quá kích thước của mảng
			// Ví dụ: in thông báo lỗi hoặc điều chỉnh chỉ số để nằm trong phạm vi hợp lệ
			// Color[MAX_INDEX] = 0; // MAX_INDEX là một giả sử về giới hạn chỉ số hợp lệ
			Color[15] = 0;
		}
		KRColor uTextColor;
		uTextColor.Color_dw = TGetColor(Color);
		pEncodedBuffer[nEncodedSize] = KTC_COLOR;
		pEncodedBuffer[nEncodedSize + 1]= uTextColor.Color_b.r;
		pEncodedBuffer[nEncodedSize + 2]= uTextColor.Color_b.g;
		pEncodedBuffer[nEncodedSize + 3]= uTextColor.Color_b.b;
		nEncodedSize += 4;
		}
		break;
	case KTC_BORDER_COLOR:
		{
		if (nParamLen == 0)
		{
			pEncodedBuffer[nEncodedSize ++] = KTC_BORDER_RESTORE;
			return nEncodedSize;
		}
		else if (nParamLen < 8)
		{
			memcpy(Color, pParamBuffer, nParamLen);
			Color[nParamLen] = 0;
			for (int i = 0; i < s_nColorCount; i++)
			{
#ifndef __linux
				if (stricmp(Color,s_ColorTable[i].Token) == 0)
#else
				if(strcasecmp(Color,s_ColorTable[i].Token) == 0)
#endif
				{
					pEncodedBuffer[nEncodedSize] = KTC_BORDER_COLOR;
					pEncodedBuffer[nEncodedSize + 1]= s_ColorTable[i].Red;
					pEncodedBuffer[nEncodedSize + 2]= s_ColorTable[i].Green;
					pEncodedBuffer[nEncodedSize + 3]= s_ColorTable[i].Blue;
					nEncodedSize += 4;
					return nEncodedSize;
				}
			}
		}
		memcpy(Color, pParamBuffer, nParamLen);
		Color[nParamLen] = 0;
		KRColor uTextColor;
		uTextColor.Color_dw = TGetColor(Color);
		pEncodedBuffer[nEncodedSize] = KTC_BORDER_COLOR;
		pEncodedBuffer[nEncodedSize + 1]= uTextColor.Color_b.r;
		pEncodedBuffer[nEncodedSize + 2]= uTextColor.Color_b.g;
		pEncodedBuffer[nEncodedSize + 3]= uTextColor.Color_b.b;
		nEncodedSize += 4;
		}
		break;
	}
	return nEncodedSize;
}

extern "C" ENGINE_API
int	TRemoveCtrlInEncodedText(char* pBuffer, int nCount)
{
	int nLen = 0;
	nCount = TFilterEncodedText(pBuffer, nCount);
	for (int nPos = 0; nPos < nCount; nPos++)
	{
		char cCharacter = pBuffer[nPos];
		if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			nPos += 3;
		else if (cCharacter == KTC_INLINE_PIC)
			nPos += sizeof(WORD);
		else if (cCharacter != KTC_COLOR_RESTORE && cCharacter != KTC_BORDER_RESTORE)
		{
			pBuffer[nLen] = cCharacter;
			nLen ++;
		}
	}
	return nLen;
}

//»ñÈ¡±àÂëÎÄ±¾µÄÐÐÊýÓë×î´óÐÐ¿í
//²ÎÊý£ºpBuffer			ÎÄ±¾»º³åÇø
//		nCount			ÎÄ±¾Êý¾ÝµÄ³¤¶È
//		nWrapCharaNum	ÏÞÖÆÃ¿ÐÐ²»Ðí³¬¹ýµÄ×Ö·ûÊýÄ¿
//		nMaxLineLen		ÓÃÓÚ»ñÈ¡ÎÄ±¾µÄÊµ¼Ê×î´óÐÐ¿í£¨×Ö·ûÊýÄ¿£©
//		nFontSize		²ÉÓÃ×ÖÌåµÄ´óÐ¡ [wxb 2003-6-19]
//		nSkipLine		Ìø¹ýÇ°Ãæ¶àÉÙÐÐµÄÊý¾Ý
//		nNumLineLimit	¼ì²âµÄÎÄ±¾µÄÐÐÊý£¬³¬¹ýÏÞÖÆÐÐÊýÄ¿Ö®ºóµÄÄÚÈÝ±»ºöÂÔ¡£Èç¹û´ËÖµÐ¡ÓÚµÈÓÚ0Ôò±íÊ¾ÎÞ´ËÏÞÖÆ¡£
//·µ»Ø£ºÎÄ±¾µÄÐÐÊý
extern "C" ENGINE_API
int	TGetEncodedTextLineCount(const char* pBuffer, int nCount, int nWrapCharaNum, int& nMaxLineLen, int nFontSize, int nSkipLine, int nNumLineLimit,
							 BOOL bPicSingleLine/* = FALSE*/)
{
	//ÉèÒ»¸ö¼«ÖµÃâµÃ³ö´í [wxb 2003-6-20]
	_ASSERT(nFontSize >= 4 && nFontSize < 64);
	nFontSize = max(4, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	nMaxLineLen = 0;
	if (pBuffer == 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);

	float fNumChars = 0;
	int nNumLine = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;
	if (nSkipLine < 0)
		nSkipLine = 0;
	if (nNumLineLimit <= 0)
		nNumLineLimit = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	int  nExtraLineForInlinePic = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)	//¿ÉÄÜÊÇÖÐÎÄ×Ö·û
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)//ÑÕÉ«¿ØÖÆ
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			//Ç¶ÈëÊ½Í¼Æ¬´¦Àí [wxb 2003-6-19]
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
			if (nSkipLine > 0)
			{
				nSkipLine -= 1 + nExtraLineForInlinePic;

				//´¦ÀíÍ¼Æ¬Õ¼¶àÐÐµÄÇé¿ö [wxb 2003-6-19]
				if (nSkipLine < 0)
				{
					if (fMaxLineLen < fNumChars)
						fMaxLineLen = fNumChars;
					nNumLine += (-nSkipLine);
					if (nNumLine >= nNumLineLimit)
						break;
				}
			}
			else
			{
				if (fMaxLineLen < fNumChars)
					fMaxLineLen = fNumChars;
				nNumLine += 1 + nExtraLineForInlinePic;
				if (nNumLine >= nNumLineLimit)
					break;
			}
			nExtraLineForInlinePic = 0;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}
	if (nNumLine < nNumLineLimit && fNumChars && nSkipLine == 0)
	{
		if (fMaxLineLen < fNumChars)
			fMaxLineLen = fNumChars;
		nNumLine += 1 + nExtraLineForInlinePic;
	}

	nMaxLineLen = (int)(fMaxLineLen + (float)0.9999);	//½ø1
	return nNumLine;
}


extern "C" ENGINE_API
int	TGetEncodedItemChatLineCount(const char* pBuffer, int nCount, int nWrapCharaNum, int& nMaxLineLen, int nFontSize,
	int& nFace, int& nLastPos, int& nTotalLen, int nSkipLine, int nNumLineLimit, bool bPicSingleLine/* = false*/ )
{
	_ASSERT(nFontSize >= 4 && nFontSize < 64);
	nFontSize = max(4, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	nMaxLineLen = 0;
	if (pBuffer == 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);
	nFace = 0;
	nLastPos = 0;
	nTotalLen = 0;
	float fNumChars = 0;
	int nNumLine = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;
	if (nSkipLine < 0)
		nSkipLine = 0;
	if (nNumLineLimit <= 0)
		nNumLineLimit = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	int  nExtraLineForInlinePic = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)
		{
			nPos ++;
			nLastPos ++;
			nTotalLen ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			nFace++;
			nTotalLen+=3;
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			nLastPos++;
			nTotalLen++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
			if (nSkipLine > 0)
			{
				nSkipLine -= 1 + nExtraLineForInlinePic;

				if (nSkipLine < 0)
				{
					if (fMaxLineLen < fNumChars)
						fMaxLineLen = fNumChars;
					nNumLine += (-nSkipLine);
					nLastPos = 0;
					if (cCode != KTC_INLINE_PIC)
						nFace = 0;
					else
						nFace = 1;
					if (nNumLine >= nNumLineLimit)
						break;
				}
			}
			else
			{
				if (fMaxLineLen < fNumChars)
					fMaxLineLen = fNumChars;
				nNumLine += 1 + nExtraLineForInlinePic;
				nLastPos = 0;
				if (cCode != KTC_INLINE_PIC)
					nFace = 0;
				else
					nFace = 1;
				if (nNumLine >= nNumLineLimit)
					break;
			}
			nExtraLineForInlinePic = 0;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}
	if (nNumLine < nNumLineLimit && fNumChars && nSkipLine == 0)
	{
		if (fMaxLineLen < fNumChars)
			fMaxLineLen = fNumChars;
		nNumLine += 1 + nExtraLineForInlinePic;
	}

	nMaxLineLen = (int)(fMaxLineLen + (float)0.9999);
	return nNumLine;
}

//»ñµÃÖ¸¶¨ÐÐµÄ¿ªÊ¼Î»ÖÃ
int TGetEncodeStringLineHeadPos(const char* pBuffer, int nCount, int nLine, int nWrapCharaNum, int nFontSize, BOOL bPicSingleLine)
{
	//ÉèÒ»¸ö¼«ÖµÃâµÃ³ö´í [wxb 2003-6-20]
	_ASSERT(nFontSize > 1 && nFontSize < 64);
	nFontSize = max(1, nFontSize);
	nFontSize = min(64, nFontSize);

	float fMaxLineLen = 0;
	if (pBuffer == 0 || nLine <= 0)
		return 0;

	if (nCount < 0)
		nCount = strlen(pBuffer);

	float fNumChars = 0;
	int  nExtraLineForInlinePic = 0;
	int nPos = 0;
	unsigned char	cCode;

	if (nWrapCharaNum <= 0)
		nWrapCharaNum = 0x7fffffff;

	bool bNextLine = false;
	float fNumNextLineChar = 0;
	while(nPos < nCount)
	{
		cCode = pBuffer[nPos];
		if (cCode > 0x80)	//¿ÉÄÜÊÇÖÐÎÄ×Ö·û
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else if (cCode == KTC_COLOR || cCode == KTC_BORDER_COLOR)//ÑÕÉ«¿ØÖÆ
			nPos += 4;
		else if (cCode == KTC_INLINE_PIC)
		{
			//Ç¶ÈëÊ½Í¼Æ¬´¦Àí [wxb 2003-6-19]
			WORD wPicIndex = *((WORD*)(pBuffer + nPos + 1));
			nPos += 1 + sizeof(WORD);
			if (g_pIInlinePicSink)
			{
				int nWidth, nHeight;
				if (SUCCEEDED(g_pIInlinePicSink->GetPicSize(wPicIndex, nWidth, nHeight)))
				{
					if (nHeight > nFontSize)
					{
						int nExtraLines = nHeight - nFontSize;
						nExtraLines = nExtraLines / nFontSize + ((nExtraLines % nFontSize) ? 1 : 0);
						if (nExtraLines > nExtraLineForInlinePic && !bPicSingleLine)
							nExtraLineForInlinePic = nExtraLines;
					}
					if (fNumChars + (((float)nWidth) * 2 / nFontSize) < nWrapCharaNum)
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					else if (fNumChars + (((float)nWidth) * 2 / nFontSize) == nWrapCharaNum || fNumChars == 0)
					{
						bNextLine = true;
						fNumChars += ((float)nWidth) * 2 / nFontSize;
					}
					else
					{
						bNextLine = true;
						fNumNextLineChar = ((float)nWidth) * 2 / nFontSize;
					}
				}
			}
		}
		else if (cCode == KTC_ENTER)
		{
			nPos ++;
			bNextLine = true;
		}
		else if (cCode != KTC_COLOR_RESTORE && cCode != KTC_BORDER_RESTORE)
		{
			nPos ++;
			fNumChars += 1;
			if (fNumChars >= nWrapCharaNum)
			{
				bNextLine = true;
			}
		}
		else
		{
			nPos++;
		}

		if (bNextLine == false && fNumChars && fNumChars + 3 >= nWrapCharaNum)
		{
			const char* pNext = TGetSecondVisibleCharacterThisLine(pBuffer, nPos, nCount);
			if (pNext && TIsCharacterNotAlowAtLineHead(pNext))
				bNextLine = true;
		}
		if (bNextLine)
		{
//			if (nSkipLine > 0)
//			{
//				nSkipLine -= 1 + nExtraLineForInlinePic;
//
//				//´¦ÀíÍ¼Æ¬Õ¼¶àÐÐµÄÇé¿ö [wxb 2003-6-19]
//				if (nSkipLine < 0)
//				{
//					if (fMaxLineLen < fNumChars)
//						fMaxLineLen = fNumChars;
//					nNumLine += (-nSkipLine);
//					if (nNumLine >= nNumLineLimit)
//						break;
//				}
//			}
			if ((--nLine) == 0)
				break;
			fNumChars = (float)fNumNextLineChar;
			fNumNextLineChar = 0;
			bNextLine = false;
		}
	}

	return nPos;
}

extern "C" ENGINE_API
//Èç¹ûÔ­(°üº¬¿ØÖÆ·û)×Ö·û´®³¤¶È£¨°üÀ¨½áÎ²·û£©³¬¹ýÏÞ¶¨µÄ³¤¶È£¬Ôò½Ø¶ÌËü²¢¼ÓÉÏ..ºó×º
const char* TGetLimitLenEncodedString(const char* pOrigString, int nOrigLen, int nFontSize,
	int nWrapCharaNum, char* pLimitLenString, int& nShortLen, int nLineLimit, int bPicPackInSingleLine/*=false*/)
{
	if (pOrigString == NULL || pLimitLenString == NULL ||
		nOrigLen <= 0 || nShortLen < 2 || nLineLimit < 1 || nWrapCharaNum < 2)
	{
		nShortLen = 0;
		return NULL;
	}

	int nPreLineEndPos = 0, nFinalLineEndPos;
	if (nLineLimit > 1)	//Ìø¹ýÇ°Ãæ¼¸ÐÐ
	{
		nPreLineEndPos = TGetEncodeStringLineHeadPos(pOrigString, nOrigLen, nLineLimit - 1, nWrapCharaNum, nFontSize, bPicPackInSingleLine);
		if (nPreLineEndPos > nShortLen)
		{
			nShortLen = TSplitEncodedString(pOrigString, nOrigLen, nShortLen - 2, true);
			memcpy(pLimitLenString, pOrigString, nShortLen);
			pLimitLenString[nShortLen] = '.';
			pLimitLenString[nShortLen + 1] = '.';
			nShortLen += 2;
			return pLimitLenString;
		}
	}

	if (nPreLineEndPos < nOrigLen)
	{
		nFinalLineEndPos = TGetEncodeStringLineHeadPos(pOrigString + nPreLineEndPos,
			nOrigLen - nPreLineEndPos, 1, nWrapCharaNum, nFontSize, bPicPackInSingleLine) + nPreLineEndPos;
	}
	else
		nFinalLineEndPos = nOrigLen;

	if (nFinalLineEndPos >= nOrigLen)
	{
		nShortLen = TSplitEncodedString(pOrigString, nOrigLen, nShortLen, true);
		memcpy(pLimitLenString, pOrigString, nShortLen);
		return pLimitLenString;
	}

	int nDesireLen = (nFinalLineEndPos <= nShortLen) ? nFinalLineEndPos - 2 : nShortLen - 2;

	const char* pFinalLineHead = pOrigString + nPreLineEndPos;
	int nRemainCount = nOrigLen - nPreLineEndPos;
	nDesireLen -= nPreLineEndPos;
	while(true)
	{
		nShortLen = TSplitEncodedString(pFinalLineHead, nRemainCount, nDesireLen, true);
		int nMaxLineLen;
		TGetEncodedTextLineCount(pFinalLineHead, nShortLen, 0, nMaxLineLen, nFontSize, 0, 1, FALSE);
		if (nMaxLineLen <= nWrapCharaNum - 2)
			break;
		nDesireLen --;
	}
	nShortLen += nPreLineEndPos;

   	memcpy(pLimitLenString, pOrigString, nShortLen);
	pLimitLenString[nShortLen] = '.';
	pLimitLenString[nShortLen + 1] = '.';
	nShortLen += 2;
	return pLimitLenString;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÈç¹ûÔ­×Ö·û´®³¤¶È£¨°üÀ¨½áÎ²·û£©³¬¹ýÏÞ¶¨µÄ³¤¶È£¬Ôò½Ø¶ÌËü²¢¼ÓÉÏ..ºó×º
//	²ÎÊý£ºpOrigString     --> Ô­×Ö·û´®£¬ÒªÇó²»Îª¿ÕÖ¸Õë
//		¡¡nOrigLen		  --> Ô­×Ö·û´®³¤¶È£¨²»°üÀ¨½áÎ²·û£©
//		  pLimitLenString --> Èç¹ûÔ­×Ö·û´®³¬³öÏÞ³¤£¬ÓÃÀ´´æ´¢½Ø¶ÌºóµÄ×Ö·û´®µÄ»º³åÇø£¬ÒªÇó²»Îª¿ÕÖ¸Õë
//		  nLimitLen		  --> ÏÞ¶¨³¤¶È£¬´ËÖµÒªÇó´óÓÚµÈÓÚ3
//	·µ»Ø£ºÈçÔ­×Ö·û´®²»³¬¹ýÏÞ³¤£¬Ôò·µ»ØÔ­»º³åÇøÖ¸Õë£¬·ñÔò·µ»ØÓÃÀ´´æ´¢½Ø¶ÌºóµÄ×Ö·û´®µÄ»º³åÇøµÄÖ¸Õë
//	×¢ÊÍ£ºChinese GBK±àÂë°æ±¾£¬´Ë×Ö·û´®ÖÐ×Ö·ûÈ«²¿ÊÓÎªÏÔÊ¾×Ö·û£¬²»°üº¬¿ØÖÆ×Ö·û
//--------------------------------------------------------------------------
extern "C" ENGINE_API
const char* TGetLimitLenString(const char* pOrigString, int nOrigLen, char* pLimitLenString, int nLimitLen)
{
	if (pOrigString && pLimitLenString && nLimitLen > 0)
	{
		if (nOrigLen < 0)
			nOrigLen = strlen(pOrigString);
		if (nOrigLen < nLimitLen)
			return pOrigString;
		if (nLimitLen > 2)
		{
			nOrigLen = TSplitString(pOrigString, nLimitLen - 3, true);
			memcpy(pLimitLenString, pOrigString, nOrigLen);
			pLimitLenString[nOrigLen] = '.';
			pLimitLenString[nOrigLen + 1] = '.';
			pLimitLenString[nOrigLen + 2] = 0;
		}
		else if (nLimitLen == 2)
		{
			pLimitLenString[0] = '.';
			pLimitLenString[1] = 0;
		}
		return ((nLimitLen >= 2) ? pLimitLenString : NULL);
	}
	return NULL;
}


//¶ÔÒÑ¾­±àÂëµÄÎÄ±¾£¬´ÓÖ¸¶¨Î»ÖÃ¿ªÊ¼²éÕÒÖ¸¶¨µÄ¿ØÖÆ·ûºÅµÄÎ»ÖÃ£¬·µ»Ø-1±íÊ¾Î´ÕÒµ½
extern "C" ENGINE_API
int	TFindSpecialCtrlInEncodedText(const char* pBuffer, int nCount, int nStartPos, char cControl, char cRetControl/* = 0*/)
{
	int nFindPos = -1;
	if (pBuffer)
	{
		while(nStartPos < nCount)
		{
			unsigned char cCharacter = pBuffer[nStartPos];
			if(cRetControl && ((unsigned char)cRetControl == cCharacter))
			{
				return -1;
			}
			if ((unsigned char)cControl == cCharacter)
			{
				nFindPos = nStartPos;
				break;
			}
			if (cCharacter > 0x80)	//¿ÉÄÜÊÇÖÐÎÄÎÄ×Ö
				nStartPos ++;
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				nStartPos += 4;
			else if (cCharacter == KTC_INLINE_PIC)
				nStartPos += 3;
			else
				nStartPos ++;
		}
	}
	return nFindPos;
}

//¶ÔÒÑ¾­±àÂëµÄÎÄ±¾£¬È¥³ýÖ¸¶¨ÀàÐÍµÄ¿ØÖÆ·û
extern "C" ENGINE_API
int	TClearSpecialCtrlInEncodedText(char* pBuffer, int nCount, char cControl)
{
	int nFinalLen = 0;
	int nReadPos = 0;
	if (pBuffer)
	{
		if ((unsigned char)(cControl) <= 0x80)
		{
			int nMatchLen = 1;
			if (cControl == KTC_COLOR || cControl == KTC_BORDER_COLOR)
				nMatchLen = 4;
			else if (cControl == KTC_INLINE_PIC)
				nMatchLen = 3;
			while(nReadPos < nCount)
			{
				unsigned char cCharacter = pBuffer[nReadPos];
				if ((unsigned char)cControl == cCharacter)
				{
					nReadPos += nMatchLen;
				}
				else if (cCharacter > 0x80)
				{
					pBuffer[nFinalLen++] = pBuffer[nReadPos++];
				}
				else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
				{
					int nTemp = *(int*)(pBuffer + nReadPos);
					*(int*)(pBuffer + nFinalLen) = nTemp;
					nFinalLen += 4;
					nReadPos += 4;
				}
				else if (cCharacter == KTC_INLINE_PIC)
				{
					memmove((pBuffer + nFinalLen), (pBuffer + nReadPos), 3);
					nFinalLen += 3;
					nReadPos += 3;
				}
				else
				{
					pBuffer[nFinalLen++] = pBuffer[nReadPos++];
				}
			}
		}
	}
	return nFinalLen;
}

//¶ÔÒÑ¾­±àÂëµÄÎÄ±¾£¬Ö¸¶¨Êä³ö³¤¶ÈµÄÔÚ»º³åÇøÖÐÎ»ÖÃ
extern "C" ENGINE_API
int TGetEncodedTextOutputLenPos(const char* pBuffer, int nCount, int& nLen, bool bLess, int nFontSize)
{
	int nIndex = 0, nLenTemp = 0;

	_ASSERT(nFontSize >= 4);
	nFontSize = max(4, nFontSize);

    if (pBuffer)
	{
		int nWidth, nHeight;
		int nByteCount = 0, nCurCharLen = 0;
	    unsigned char cCharacter        = 0;

		while(nLenTemp < nLen)
		{
			cCharacter = pBuffer[nIndex];
			//¼ÆËã³öµ±Ç°ÔªËØµÄËùÕ¼×Ö½ÚÊýnByteCountºÍÔÚÏÔÊ¾»­ÃæÉÏËùÕ¼¿í¶ÈnCurCharLen
			if (cCharacter > 0x80)	//¿ÉÄÜÊÇÖÐÎÄÎÄ×Ö
			{
                nByteCount  = 1;
				nCurCharLen = 1;
			}
			else if (cCharacter == KTC_COLOR || cCharacter == KTC_BORDER_COLOR)
			{
			    nByteCount  = 4;
				nCurCharLen = 0;
			}
			else if (cCharacter == KTC_COLOR_RESTORE && cCharacter == KTC_BORDER_RESTORE)
			{
				nByteCount  = 1;
				nCurCharLen = 0;
			}
			else if (cCharacter == KTC_INLINE_PIC)
			{
				nByteCount  = 3;
				if(SUCCEEDED(g_pIInlinePicSink->GetPicSize(
					*((unsigned short *)(pBuffer + nIndex + 1)), nWidth, nHeight)))
				{
					nCurCharLen = ((nWidth * 2 + nFontSize - 1) /  nFontSize);
				}
				else
					nCurCharLen = 0;
			}
			else
			{
				nByteCount  = 1;
				nCurCharLen = 1;
			}

			//Èç¹û³¬³ö»º³åÇø£¬¾ÍÍ£Ö¹°É
			if(nIndex + nByteCount > nCount)
				break;
			//Èç¹û¿í¶È»¹Ã»³¬¹ýÒªÇó¿í¶È
			if(nLenTemp + nCurCharLen < nLen)
			{
				nLenTemp += nCurCharLen;
		        nIndex   += nByteCount;
			}
			//Èç¹û¿í¶ÈµÈÓÚÒªÇó¿í¶ÈÁË
			else if(nLenTemp + nCurCharLen == nLen)
			{
				nLenTemp += nCurCharLen;
				nIndex   += nByteCount;
				break;
			}
			//ÕâÀï¾ÍÊÇ³¬¹ýÁË
			else
			{
				nLenTemp = bLess ? nLenTemp : (nLenTemp + nCurCharLen);
				nIndex   = bLess ? nIndex   : (nIndex + nByteCount);
				break;
			}
		}
	}
	nLen = nLenTemp;
	return nIndex;
}

//¶ÔÒÑ¾­±àÂëµÄÎÄ±¾£¬Ö¸¶¨µÄÇ°¶Î»º³åÇøÖÐ¿ØÖÆ·û£¬¶ÔºóÃæµÄÊä³ö²úÉúÐ§¹ûÓ°Ïì
extern "C" ENGINE_API
int TGetEncodedTextEffectCtrls(const char* pBuffer, int nSkipCount, KTP_CTRL& Ctrl0, KTP_CTRL& Ctrl1)
{
	int nIndex = 0;
	Ctrl0.cCtrl = Ctrl1.cCtrl = KTC_INVALID;
	if (pBuffer)
	{
		KTP_CTRL PreCtrl0, PreCtrl1;
		PreCtrl0.cCtrl = PreCtrl1.cCtrl = KTC_INVALID;

		while(nIndex < nSkipCount)
		{
			unsigned char cCharacter = pBuffer[nIndex];
			if (cCharacter == KTC_COLOR)
			{
				PreCtrl0  =  Ctrl0;
				*(int*)(&Ctrl0) = *(int*)(pBuffer + nIndex);
				nIndex += 4;				
			}
			else if (cCharacter == KTC_BORDER_COLOR)
			{
				PreCtrl1  =  Ctrl1;
				*(int*)(&Ctrl1) = *(int*)(pBuffer + nIndex);
				nIndex += 4;
			}
			else if(cCharacter == KTC_COLOR_RESTORE)
			{
				Ctrl0 = PreCtrl0;
				nIndex ++;
			}
			else if(cCharacter == KTC_BORDER_RESTORE)
			{
				Ctrl1 = PreCtrl1;
				nIndex ++;
			}
			else
				nIndex ++;
		}
	}
	return nIndex;
}

