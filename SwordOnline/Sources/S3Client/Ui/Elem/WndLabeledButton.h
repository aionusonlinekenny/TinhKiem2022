/*****************************************************************************************
//	界面窗口体系结构--带文字按键窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-10-4
------------------------------------------------------------------------------------------
	带文字按钮窗口。
*****************************************************************************************/
#pragma once
#include "WndButton.h"
#include "WndText.h"
#include "WndImage.h"

//===========================================
//	简单的带文字按钮窗口。
//	文字单行，居中处理，文字中不包含控制符。
//===========================================
class KWndLabeledButton : public KWndButton
{
public:
	KWndLabeledButton();
	void			SetLabel(const char* pLabel);		//设置按钮标题文字
	void			Set3IntLabel(INT nNumber, char Separator = 0);
	INT				GetLabel(char* pLabel, INT nSize);
	void			SetLabelColor(UINT Color);	//设置按钮标题文字颜色
	void			SetLabelBorderColor(UINT Color);	//设置按钮标题文字颜色
	virtual INT		Init(KIniFile* pIniFile, const char* pSection);	//初始化
	virtual void	PaintWindow();									//窗体绘制
	void			Clone(KWndLabeledButton* pCopy);
	void			Enable(INT bEnable);

private:
	char	m_Label[32];		//标题文字
	INT		m_nLabelLen;		//标题文字串的长度，不包括结束符号
	INT		m_nFontSize;		//标题文字的大小
	INT		m_nLabelXOffset;	//标题文字向下偏移的量
	INT		m_nLabelYOffset;	//标题文字向下偏移的量
	UINT m_FontColor;	//标题文字的颜色
	UINT m_FontBorderColor;	//标题文字的边缘颜色
	UINT m_OverColor;	//鼠标移动到上面时候的文字颜色
	UINT m_OverBorderColor;	//鼠标移动到上面时候的文字边缘颜色
	UINT m_EnableColor;	//鼠标移动到上面时候的文字边缘颜色
	UINT m_EnableBdColor;	//鼠标移动到上面时候的文字边缘颜色
	UINT m_DisableColor;	//鼠标移动到上面时候的文字边缘颜色
	UINT m_DisableBdColor;	//鼠标移动到上面时候的文字边缘颜色
};
