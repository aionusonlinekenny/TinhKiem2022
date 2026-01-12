/*****************************************************************************************
//	剑侠引擎，界面窗口体系结构的最基本窗口对象
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-9
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

//============窗口风格================
#define WND_S_VISIBLE				0x80000000	//可见
#define WND_S_DISABLED				0x40000000	//不可操作
#define	WND_S_TOPMOST				0x20000000	//置顶窗口
#define	WND_S_MOVEALBE				0x10000000	//可被拖动
#define	WND_S_DISABLE				0x04000000	//窗口不可被操作
#define	WND_S_TOBEDESTROY			0x02000000	//window is to be destroy
#define	WND_S_MOVE_WITH_R_EDGE		0x01000000	//跟随父窗口右下边界的变化移动窗口
#define	WND_S_MOVE_WITH_B_EDGE		0x00800000	//跟随父窗口右下边界的变化移动窗口
#define	WND_S_SIZE_WITH_R_EDGE		0x00400000	//跟随父窗口大小的变化调整窗口大小
#define	WND_S_SIZE_WITH_B_EDGE		0x00200000	//跟随父窗口大小的变化调整窗口大小
#define	WND_S_SIZE_WITH_ALL_CHILD	0x00100000	//以子窗口的范围测试的集合作为自己的范围测试的集合

extern	INT WND_SHOW_DEBUG_FRAME_TEXT;

class KIniFile;

class KWndWindow
{
protected:
	//----窗口参数----
	UINT m_Style;			//窗口风格,见Wnd.h中窗口风格的定义
	INT			m_Left;				//左上角X坐标，相对于父窗口
	INT			m_Top;				//左上角Y坐标，相对于父窗口
	INT			m_Width;			//宽度
	INT			m_Height;			//高度
	INT			m_nAbsoluteLeft;	//窗口左上角的绝对坐标x
	INT			m_nAbsoluteTop;		//窗口左上角的绝对坐标y
	INT			m_nValue;
#ifdef _DEBUG
	char		m_Caption[64];		//标题文字
#endif

	INT			m_bMoving;
	INT			m_nLastMouseHoldPosX;
	INT			m_nLastMouseHoldPosY;

	//----与其他窗口的级连关系----
	KWndWindow* m_pPreviousWnd;		//前一个同级窗口
	KWndWindow* m_pNextWnd;			//后一个同级窗口
	KWndWindow* m_pFirstChild;		//第一个子窗口
	KWndWindow* m_pParentWnd;		//父窗口

public:
	KWndWindow();
	virtual	~KWndWindow();

	void	Destroy();
	virtual void UpdateData() {}

	//====可重载的函数====
	// flying add this function
	virtual INT		CloseWindow(bool bDestory) { return 0; };

	virtual INT		Init(KIniFile* pIniFile, const char* pSection);//初始化
	virtual INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
	virtual INT		PtInWindow(INT x, INT y);				//判断一个点是否在窗口范围内,传入的是绝对坐标

	virtual void	PaintWindow();							//窗体绘制
	void			PaintDebugInfo();

	virtual void	SetSize(INT nWidth, INT nHeight);		//设置窗口大小

	//====窗口行为操作====
	void			BringToTop();							//把窗口置顶
	void			GetPosition(INT* pLeft, INT* pTop);		//获取窗口位置，相对坐标
	void			SetPosition(INT nLeft, INT nTop);		//设置窗口位置，相对坐标
	void			GetAbsolutePos(INT* pLeft, INT* pTop);	//获取窗口位置，绝对坐标
	void			GetSize(INT* pWidth, INT* pHeight);		//获取窗口大小
	void			SetCursorAbove();						//使鼠标指针以移动到悬浮在此窗口中的位置上
	void			GetAllChildLayoutRect(RECT* pRect);		//取得包含所有子窗口分布区域的最小区域
	void			SetValue(INT nValue) { m_nValue = nValue; };
	INT				GetValue() { return m_nValue; };

	virtual void	Show();									//显示窗口
	virtual void	Hide();									//隐藏窗口
	INT				IsVisible();							//判断窗口是否被显示
	INT				IsEnable();							//判断窗口是否被显示
	virtual	void	Enable(INT bEnable);					//禁止或者允许使窗口被操作
	//	void			SetStyle(DWORD	dwStyle);				//修改窗口风格
	void			AddChild(KWndWindow* pChild);			//添加子窗口
	void			AddBrother(KWndWindow* pBrother);		//添加兄弟窗口

	virtual KWndWindow* TopChildFromPoint(INT x, INT y);	//得到处于指定坐标位置的最上层窗口，传入的坐标为绝对坐标

	KWndWindow* GetPreWnd() const { return m_pPreviousWnd; }	//得到前一个兄弟窗口
	KWndWindow* GetNextWnd() const { return m_pNextWnd; }		//得到后一个兄弟窗口
	KWndWindow* GetParent() const { return m_pParentWnd; }		//得到父窗口
	KWndWindow* GetFirstChild() const { return m_pFirstChild; }	//得到第一个子窗口
	KWndWindow* GetOwner();								//获得不再有父窗口的祖先窗口

	void			Paint();								//绘制
	void			LetMeBreathe();							//让窗口活动

	void			SplitSmaleFamily();						//把自己（及子窗口）从窗口树里面里面分离出来
	void			LeaveAlone();							//世间再无窗在我左右，一无牵连

	//----属性设置，获取----
#ifdef _DEBUG
	void			SetCaption(char* pszCaption);
#endif
	INT				IsDisable() { return (m_Style & WND_S_DISABLE); }
	INT				GetStyle() { return m_Style; }
	INT				SetStyle(UINT nStyle)
	{
		m_Style = nStyle;
		return m_Style;
	}

protected:
	void			Clone(KWndWindow* pCopy);

private:
	virtual void	Breathe() {}							//窗口的持续行为
	void			OnLBtnDown(INT x, INT y);				//响应鼠标左键按下的操作，传入的坐标为绝对坐标
	void			OnMoveWnd();							//响应鼠标左键按下移动的操作，传入的坐标为绝对坐标

private:
	void			AbsoluteMove(INT dx, INT dy);			//绝对坐标的调整
};

//把字符串表示的颜色信息转为数值表示
UINT	GetColor(const char* pString);
//把数值表示的颜色信息转为字符串表示
const char* GetColorString(UINT nColor);
