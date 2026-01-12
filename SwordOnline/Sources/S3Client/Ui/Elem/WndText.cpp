/*****************************************************************************************
//	½çÃæ´°¿ÚÌåÏµ½á¹¹--ÎÄ±¾´°¿Ú
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-23
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "GameDataDef.h"
#include "../Elem/WndMessage.h"
#include "WndText.h"
#include "MouseHover.h"
#include "../../../Engine/Src/Text.h"
#include "../../../Engine/Src/KDebug.h"


#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell* g_pRepresentShell;
UINT KWndText::ms_uBtnTipTextColor = 0xffffffff;	//°´Å¥ÌáÊ¾Ãû³ÆÎÄ×ÖµÄÑÕÉ«
INT			 KWndText::ms_nDisableBtnTip = false;		//ÊÇ·ñ¾²Ö¹°´Å¥µÄÌáÊ¾ÎÄ×Ö

//ÉèÖÃ°´Å¥ÌáÊ¾Ãû³ÆÎÄ×ÖµÄÑÕÉ«
void KWndText::SetAllButtonTipTextColor(UINT uColor)
{
	ms_uBtnTipTextColor = uColor;
}

//½ûÖ¹/ÔÊÐí°´Å¥µÄÌáÊ¾ÎÄ×Ö
void KWndText::EnableAllButtonTip(INT bEnable)
{
	ms_nDisableBtnTip = ~bEnable;
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KWndText::KWndText()
{
	m_nFontSize = 12;
	m_pText = NULL;
	m_nTextLen = 0;
	m_nBuffLen = 0;
	m_nTopLine = 0;
	m_nLineCount = 0;
	m_BorderColor = 0;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃÎÄ±¾ÎÄ×Ö
//--------------------------------------------------------------------------
void KWndText::SetText(const char* pText, INT nLen/*= -1*/)
{
	if (m_pText && g_pRepresentShell)
	{
		if (pText && pText[0])
		{
			if (nLen < 0)
				nLen = strlen(pText);
			if (nLen > m_nBuffLen)
				nLen = TSplitString(pText, m_nBuffLen - 1, true);
			memcpy(m_pText, pText, nLen);
			m_pText[nLen] = 0;
			m_nTextLen = nLen;
			if (m_Style & WNDTEXT_ES_MULTILINE)
			{
				INT nLineLen;
				m_nLineCount = TGetEncodedTextLineCount(
					m_pText, m_nTextLen, (m_Width * 2) / m_nFontSize, nLineLen, m_nFontSize);
			}
		}
		else
		{
			m_pText[0] = 0;
			m_nTextLen = 0;
			m_nLineCount = 0;
		}
		m_nTopLine = 0;
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»ñÈ¡×Ö·û´®ÄÚÈÝ
//--------------------------------------------------------------------------
INT KWndText::GetText(char* pBuffer, INT nSize)
{
	INT nRet = 0;
	if (m_pText && pBuffer)
	{
		if (m_nTextLen + 1 <= nSize)
		{
			memcpy(pBuffer, m_pText, m_nTextLen + 1);
			nRet = m_nTextLen;
		}
		else
			nRet = m_nTextLen + 1;
	}
	return nRet;
}

void KWndText::Clear(bool bSafe)
{
	if (m_pText)
	{
		m_nTextLen = 0;
		if (bSafe == false)
			m_pText[0] = 0;
		else
			memset(m_pText, 0, m_nTextLen + 1);
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºset the index of the first visible line INT the text control wnd
//--------------------------------------------------------------------------
INT	KWndText::SetTopLine(INT nTopLine)
{
	if (nTopLine >= 0 && nTopLine < m_nLineCount)
		m_nTopLine = nTopLine;
	return 0;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃÎÄ±¾»º³åÇøÖ¸Õë
//--------------------------------------------------------------------------
void KWndText::SetTextPtr(char* pText, INT nMaxLen)
{
	m_pText = pText;
	m_nBuffLen = nMaxLen;
	m_nTextLen = 0;
	m_nTopLine = 0;
	m_nLineCount = 0;
	m_szTip[0] = 0;
	m_nTipLen = 0;
	m_bTipHover = false;
}

void KWndText::Clone(KWndText* pCopy)
{
	if (pCopy)
	{
		KWndWindow::Clone(pCopy);
		pCopy->m_nFontSize = m_nFontSize;
		pCopy->m_TextColor = m_TextColor;
		pCopy->m_BorderColor = m_BorderColor;
		memcpy(pCopy->m_szTip, m_szTip, sizeof(m_szTip));
		pCopy->m_nTipLen = m_nTipLen;
		pCopy->m_bTipHover = m_bTipHover;
	}
}

void KWndText::SetFontSize(INT nFontSize)
{
	if (nFontSize < 12 || nFontSize > 16)
		return;

	if (m_nFontSize == nFontSize)
		return;

	m_nFontSize = nFontSize;
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
INT KWndText::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndWindow::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Font", 12, &m_nFontSize);
		//====¶ÁÈ¡ÎÄ×Ö¶ÔÆë·½Ê½====
		m_Style &= ~WNDTEXT_ES_ALIGN_FILTER;
		INT	nValue;
		pIniFile->GetInteger(pSection, "HAlign", 0, &nValue);
		if (nValue == 1)
			m_Style |= WNDTEXT_ES_HALIGN_CENTRE;
		else if (nValue == 2)
			m_Style |= WNDTEXT_ES_HALIGN_RIGHT;
		pIniFile->GetInteger(pSection, "VAlign", 0, &nValue);
		if (nValue == 1)
			m_Style |= WNDTEXT_ES_VALIGN_CENTRE;
		else if (nValue == 2)
			m_Style |= WNDTEXT_ES_VALIGN_BOTTOM;

		//===¶ÁÈ¡ÎÄ×Ö====
		char	Buff[256];
		pIniFile->GetString(pSection, "Color", "0,0,0", Buff, 16);
		m_TextColor = GetColor(Buff);
		pIniFile->GetString(pSection, "BorderColor", "0,0,0", Buff, 16);
		m_BorderColor = GetColor(Buff);
		if (m_pText && m_pText[0] == 0)
		{
			if (pIniFile->GetString(pSection, "Text", "", Buff, 256))
			{
				strncpy(m_pText, Buff, m_nBuffLen - 1);
				m_pText[m_nBuffLen - 1] = 0;
				m_nTextLen = strlen(m_pText);
			}
		}
		pIniFile->GetInteger(pSection, "MultiLine", 0, &nValue);
		if (nValue)
		{
			m_Style |= WNDTEXT_ES_MULTILINE;
			if (g_pRepresentShell)
			{
				INT nLineLen;
				m_nLineCount = TGetEncodedTextLineCount(
					m_pText, m_nTextLen, (m_Width * 2) / m_nFontSize, nLineLen, m_nFontSize);
			}
			else
				m_nLineCount = 0;
			if (m_nTopLine >= m_nLineCount)
				m_nTopLine = 0;
		}
		else
		{
			m_Style &= ~WNDTEXT_ES_MULTILINE;
			m_nLineCount = 0;
			m_nTopLine = 0;
		}
		pIniFile->GetInteger(pSection, "TipHover", 0, &nValue);
		if (nValue)
			m_bTipHover = true;
		else
			m_bTipHover = false;
		pIniFile->GetString(pSection, "Tip", "", m_szTip, sizeof(m_szTip));
		m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
		return true;
	}
	return false;
}

INT KWndText::WndProc(UINT uMsg, UINT uParam, INT nParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONUP:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WND_M_MOUSE_ENTER:
	case WM_MOUSEMOVE:
		if (m_bTipHover)
		{
			g_MouseOver.CancelMouseHoverInfo();
			if (ms_nDisableBtnTip == false && m_szTip[0] && !g_MouseOver.IsMoseHoverWndObj((void*)(KWndWindow*)this, 0))
			{
				char szTip[256];

				INT nTipLen = GetToolTipInfo(szTip, 255);
				szTip[nTipLen] = 0;

				g_MouseOver.SetMouseHoverInfo((void*)(KWndWindow*)this, 0,
					LOWORD(nParam), HIWORD(nParam), false, true);
				g_MouseOver.SetMouseHoverTitle(szTip, nTipLen, ms_uBtnTipTextColor);
			}
		}
		break;
	case WND_M_MOUSE_LEAVE:
		if (g_MouseOver.IsMoseHoverWndObj((void*)(KWndWindow*)this, 0))
			g_MouseOver.CancelMouseHoverInfo();
		KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	case WM_MOUSEHOVER:
		if (ms_nDisableBtnTip == false && m_szTip[0])
		{
			char szTip[256];

			INT nTipLen = GetToolTipInfo(szTip, 255);
			szTip[nTipLen] = 0;

			g_MouseOver.SetMouseHoverInfo((void*)(KWndWindow*)this, 0,
				LOWORD(nParam), HIWORD(nParam), false, true);
			g_MouseOver.SetMouseHoverTitle(szTip, nTipLen, ms_uBtnTipTextColor);
		}
		break;
	default:
		return KWndWindow::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

INT	KWndText::GetToolTipInfo(char* szTip, INT nMax)
{
	INT nTipLen = min(m_nTipLen, nMax);
	memcpy(szTip, m_szTip, nTipLen);
	const char* pKey = GetShortKey();
	if (pKey && pKey[0] != 0 && (nTipLen + (INT)strlen(pKey)) < nMax)
	{
		szTip[nTipLen++] = '(';
		strcpy(szTip + nTipLen, pKey);
		nTipLen += strlen(pKey);
		szTip[nTipLen++] = ')';
	}

	return nTipLen;
}

INT KWndText::SetToolTipInfo(char* szTip, INT nMax)
{
	m_nTipLen = min(nMax, sizeof(m_szTip) - 1);
	memcpy(m_szTip, szTip, m_nTipLen);
	m_szTip[m_nTipLen] = 0;
	m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
	return m_nTipLen;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»æÖÆ´°¿Ú
//--------------------------------------------------------------------------
void KWndText::PaintWindow()
{
	KWndWindow::PaintWindow();
	if (m_pText == NULL || m_pText[0] == 0 || g_pRepresentShell == NULL)
		return;
	KOutputTextParam	Param;
	Param.nX = m_nAbsoluteLeft;
	Param.nY = m_nAbsoluteTop;
	Param.nZ = TEXT_IN_SINGLE_PLANE_COORD;

	if ((m_Style & WNDTEXT_ES_MULTILINE) == 0)
	{
		switch (m_Style & WNDTEXT_ES_HALIGN_FILTER)
		{
		case WNDTEXT_ES_HALIGN_CENTRE:// ¾ÓÖÐ
			Param.nX += (m_Width - m_nTextLen * m_nFontSize / 2) / 2;
			if (Param.nX < m_nAbsoluteLeft)
				Param.nX = m_nAbsoluteLeft;
			break;
		case WNDTEXT_ES_HALIGN_RIGHT:// ¾ÓÓÒ
			Param.nX += m_Width - m_nTextLen * m_nFontSize / 2;
			if (Param.nX < m_nAbsoluteLeft)
				Param.nX = m_nAbsoluteLeft;
			break;
		}
		switch (m_Style & WNDTEXT_ES_VALIGN_FILTER)
		{
		case WNDTEXT_ES_VALIGN_CENTRE:
			Param.nY += (m_Height - m_nFontSize - 1) / 2;
			break;
		case WNDTEXT_ES_VALIGN_BOTTOM:
			Param.nY += m_Height - m_nFontSize - 1;
			break;
		}
		Param.nNumLine = 1;
	}
	else	//¶àÐÐ		
		Param.nNumLine = m_Height / (m_nFontSize + 1);

	Param.nSkipLine = m_nTopLine;
	Param.Color = m_TextColor;
	Param.BorderColor = m_BorderColor;
	g_pRepresentShell->OutputRichText(m_nFontSize, &Param, m_pText, m_nTextLen, m_Width);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃÎÄ±¾ÎÄ×ÖÑÕÉ«
//--------------------------------------------------------------------------
void KWndText::SetTextColor(UINT Color)
{
	m_TextColor = Color;
}

void KWndText::SetBorderTextColor(UINT Color)
{
	m_BorderColor = Color;
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃÊýÖµÎÄ×Ö
//--------------------------------------------------------------------------
void KWndText::SetIntText(LONG nNumber, char Separator)
{
	char	Buff[16];
	INT		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
			nLen = sprintf(Buff, "%d", nNumber);
		SetText(Buff, nLen);
	}
}

void KWndText::SetExtPointText(INT nNumber)
{
	char    Buff[64];
	INT        nLen;
	if (m_pText)
	{
		nLen = sprintf(Buff, "%d xu", nNumber);
		SetText(Buff, nLen);
	}
}

void KWndText::SetMoneyUnitText(INT nMoneyUnit)
{
	char    Buff[64];
	INT        nLen;
	if (m_pText)
	{
		if (nMoneyUnit == moneyunit_extpoint)
			nLen = sprintf(Buff, "xu");
		else if (nMoneyUnit == moneyunit_fuyuan)
			nLen = sprintf(Buff, "p.duyªn");
		else if (nMoneyUnit == moneyunit_repute)
			nLen = sprintf(Buff, "d.väng");
		else if (nMoneyUnit == moneyunit_accum)
			nLen = sprintf(Buff, "t.lòy");
		else if (nMoneyUnit == moneyunit_honor)
			nLen = sprintf(Buff, "vinh dù");
		else if (nMoneyUnit == moneyunit_respect)
			nLen = sprintf(Buff, "uy danh");
		else if (nMoneyUnit == moneyunit_coin)
			nLen = sprintf(Buff, "tiÒn ®ång");
		else
			nLen = sprintf(Buff, "l­îng");
		SetText(Buff, nLen);
	}
}

void KWndText::SetMoneyUnitPrice(INT nPrice, INT nMoneyUnit)
{
	char    Buff[64];
	INT        nLen;
	if (m_pText)
	{
		if (nMoneyUnit == moneyunit_extpoint)
			nLen = sprintf(Buff, "%d xu", nPrice);
		else if (nMoneyUnit == moneyunit_fuyuan)
			nLen = sprintf(Buff, "%d p.duyªn", nPrice);
		else if (nMoneyUnit == moneyunit_repute)
			nLen = sprintf(Buff, "%d d.väng", nPrice);
		else if (nMoneyUnit == moneyunit_accum)
			nLen = sprintf(Buff, "%d t.lòy", nPrice);
		else if (nMoneyUnit == moneyunit_honor)
			nLen = sprintf(Buff, "%d vinh dù", nPrice);
		else if (nMoneyUnit == moneyunit_respect)
			nLen = sprintf(Buff, "%d uy danh", nPrice);
		else if (nMoneyUnit == moneyunit_coin)
			nLen = sprintf(Buff, "%d tiÒn ®ång", nPrice);
		else
			nLen = sprintf(Buff, "%d l­îng", nPrice);
		SetText(Buff, nLen);
	}
}

void KWndText::Set3IntText(LONG nNumber, bool bRound, const char* szUnit)
{
	char	Buff[64];
	INT		nLen;
	if (m_pText)
	{
		INT vNumber = nNumber / MONEY_FLOOR;
		INT lNumber = nNumber % MONEY_FLOOR;
		if (nNumber < MONEY_FLOOR)
			nLen = sprintf(Buff, "%d %s", nNumber, szUnit);
		else
		{
			if (bRound)
				nLen = sprintf(Buff, "%d v¹n %s", vNumber, szUnit);
			else
			{
				if (lNumber == 0)
					nLen = sprintf(Buff, "%d v¹n %s", vNumber, szUnit);
				else
					nLen = sprintf(Buff, "%d v¹n %d %s", vNumber, lNumber, szUnit);
			}
		}
		SetText(Buff, nLen);
	}
}
//-----------------------------------------------
void KWndText::Set4IntText(LONG nNumber)
{
	char	Buff[64];
	INT		nLen = 0;
	if (m_pText)
	{
		if (nNumber)
			nLen = sprintf(Buff, "Trïng sinh %d", nNumber);
		else
			nLen = sprintf(Buff, "Ch­a trïng sinh");

		if (nLen)
			SetText(Buff, nLen);
	}
}
//-----------------------------------------------

void KWndText::Set5IntText(LONG nNumber, bool bFlag)
{
	char	Buff[16];
	INT		nLen;
	if (m_pText)
	{
		if (nNumber == 0)
		{
			if (bFlag)
				nLen = sprintf(Buff, "H¹ng ?");
			else
				nLen = sprintf(Buff, "?");
		}
		else
		{
			if (bFlag)
				nLen = sprintf(Buff, "H¹ng %d", nNumber);
			else
				nLen = sprintf(Buff, "%d", nNumber);
		}
		SetText(Buff, nLen);
	}
}
void KWndText::Set6IntText(LONG nNumber, char Separator)
{
	char	Buff[5];
	INT		nLen;
	if (m_pText)
	{
		if (Separator)
			nLen = sprintf(Buff, "%d%c", nNumber, Separator);
		else
		{

			if (nNumber == -1)
				nLen = sprintf(Buff, "");
			else
				nLen = sprintf(Buff, "%d", nNumber);

		}
		SetText(Buff, nLen);
	}
}

void KWndText::Set6IntText(LONG nNumber1, LONG nNumber2, char Separator1, char Separator2)
{
	char	Buff[50];
	INT		nLen = 0;
	if (m_pText)
	{
		if (Separator1)
			nLen = sprintf(Buff, "%d%c%c%d%c", nNumber1, Separator2, Separator1, nNumber2, Separator2);
		else
		{
			nLen = sprintf(Buff, "%d/%d", nNumber1, nNumber2);
		}
		SetText(Buff, nLen);
	}
}

void KWndText::SetResistPlus(INT nNumber, char Separator)
{
	char Buff[16];
	INT	nLen = 0;
	if (m_pText)
	{
		if (Separator)
		{
			nLen = sprintf(Buff, "%c%d%%", Separator, nNumber);
		}
		if (nLen)
			SetText(Buff, nLen);
	}
}
//--------------------------------

//	¹¦ÄÜ£ºÉèÖÃÊýÖµÎÄ×Ö
//--------------------------------------------------------------------------
void KWndText::Set2IntText(LONG nNumber1, LONG nNumber2, char Separator)
{
	char	Buff[32];
	INT		nLen;
	if (m_pText)
	{
		nLen = sprintf(Buff, "%d%c%d", nNumber1, Separator, nNumber2);
		SetText(Buff, nLen);
	}
}

void KWndText::Set2IntText2(LONG nNumber1, LONG nNumber2, char Separator1, char Separator2)
{
	char	Buff[32];
	INT		nLen;
	if (m_pText)
	{
		if (nNumber1)
		{
			if (nNumber2)
				nLen = sprintf(Buff, "%d%c%d", nNumber1, Separator2, nNumber2);
			else
				nLen = sprintf(Buff, "%d%c%c", nNumber1, Separator2, Separator1);
		}
		else
		{
			if (nNumber2)
				nLen = sprintf(Buff, "%c%c%d", Separator1, Separator2, nNumber2);
			else
				nLen = sprintf(Buff, "%c%c%c", Separator1, Separator2, Separator1);
		}
		SetText(Buff, nLen);
	}
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KWndText32::KWndText32()
{
	m_Text[0] = m_Text[31] = 0;
	SetTextPtr(m_Text, 31);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KWndText80::KWndText80()
{
	m_Text[0] = m_Text[79] = 0;
	SetTextPtr(m_Text, 79);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KWndText256::KWndText256()
{
	m_Text[0] = m_Text[255] = 0;
	SetTextPtr(m_Text, 255);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KWndText512::KWndText512()
{
	m_Text[0] = m_Text[511] = 0;
	SetTextPtr(m_Text, 511);
}
