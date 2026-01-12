#include "../Elem/WndImage.h"
#include "../Elem/WndObjContainer.h"
#include "../../../core/src/GameDataDef.h"



class KUiChatItem : protected KWndImage
{
public:

public:
	static KUiChatItem* OpenWindow(INT nIdx);
	static void			CloseWindow();
	static KUiChatItem* GetIfVisible();
	static void			LoadScheme(const char* pScheme);
public:

private:
	static  KUiChatItem* m_pSelf;
private:
	KUiChatItem();
	~KUiChatItem();
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);
	void    Initialize();
	void	PaintWindow();
	void	UpdateData(INT nIdx);
private:
	UINT			m_uBoderColor;
	uint16_t			m_nRectColor;
	int16_t					m_nMaxLineLen;
	int16_t					m_nNumLine;
	char					m_pItemDesc[GOD_MAX_OBJ_TITLE_LEN];
	int16_t					m_pItemLen;
	KWndObjectMatrix		m_ItemBox;
	KRUImagePart			m_Image;
	BYTE					m_nWidth;
	BYTE					m_nHeight;
};