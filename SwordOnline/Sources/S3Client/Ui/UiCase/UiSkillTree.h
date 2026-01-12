/*****************************************************************************************
//	界面--技能树窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../../../core/src/gamedatadef.h"

#define	SKILLTREE_MAX_SKILL_COUNT		65
#define	SKILLTREE_SHORTCUT_SKILL_COUNT	9
enum SHORTCUT_KEY_INDEX;

class KUiSkillTree : protected KWndWindow
{
public:
	static KUiSkillTree*	OpenWindow(INT bLeft);			//打开窗口，返回唯一的一个类对象实例
	static KUiSkillTree*	GetIfVisible();					//如果窗口正被显示，则返回实例指针
	static void				CloseWindow(bool bDestroy);		//关闭窗口，同时可以选则是否删除对象实例
	static void				LoadScheme(const char* pScheme);//载入界面方案
	static void				HandleShortcutKey(INT nIndex);
	static void				LoadConfig(KIniFile* pIni);
	static void				SaveConfig(KIniFile* pIni);		//保存自定义配置

	static void				DirectHandleShortcutKey(INT nIndex);
private:
	KUiSkillTree();
	~KUiSkillTree() {}
	void	Initialize();							//初始化
	void	PaintWindow();							//窗体绘制
	INT		PtInWindow(INT x, INT y);
	void	UpdateData();							//更新数据
	void	UpdateWndRect();
	void	Hide();									//隐藏窗口
	INT		GetSkillAtPos(INT x, INT y);			//得到指定位置的技能的索引
	INT		WndProc(UINT uMsg, UINT uParam, INT nParam);//窗口函数
private:
	static KUiSkillTree*	m_pSelf;
private:
	INT				m_nNumSkills;
	KUiSkillData	m_Skills[SKILLTREE_MAX_SKILL_COUNT];
					//KUiSkillData::nLevel在此的含义表示位此技能显示在第几列

	INT				m_nFont;
	UINT	m_uColor;

	UINT	m_bLeft;
	INT				m_nLeftLeft, m_nLeftTop;
	INT				m_nRightLeft, m_nRightTop;
	INT				m_nWidthPerSkill, m_nHeightPerSkill;
	static KUiSkillData	ms_ShortcutSkills[SKILLTREE_SHORTCUT_SKILL_COUNT];
					//KUiSkillData::nLevel在此的含义0表示左手技能，1表示右手技能

};