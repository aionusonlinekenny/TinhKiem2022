#ifndef __KWEATHERMGR_H__
#define __KWEATHERMGR_H__

#define WEATHERID_NOTHING	255

#ifdef _SERVER

#define MAX_WEATHER_NUM		8				// 一张地图上最大的天气种类数目

struct KWeatherInfo
{
	INT		m_weatherID;		// 天气ID
	INT		m_nOdds;			// 天气发生概率
	INT		m_nLifeTimeMin;		// 天气最小持续时间，单位为游戏帧数
	INT		m_nLifeTimeMax;		// 天气最大持续时间，单位为游戏帧数
};

class KWeatherMgr
{
public:
public:
	KWeatherMgr();
	// 从.wor和weather.ini文件读取信息，完成初始化
	// IniFile:已经打开的.wor文件
	bool InitFromIni(KIniFile& IniFile);
	// 返回当前天气ID
	INT Activate();
private:
	bool			m_bInWeather;					// 是否在天气状态下
	INT				m_nWeatherNum;					// 天气类型数目
	KWeatherInfo	m_weatherInfo[MAX_WEATHER_NUM];	// 天气信息数组
	INT				m_nFullOdds;					// 对应100%的概率值
	INT				m_nCurWeather;					// 当前天气
	INT				m_nHappenTimeMin;				// 天气发生最短时间间隔
	INT				m_nHappenTimeMax;				// 天气发生最大时间间隔
	INT				m_nLastHappentime;				// 上一次天气的发生时间
	INT				m_nNextHappentime;				// 下一次天气的发生时间
	INT				m_nWeatherDuration;				// 天气持续时间
private:
};

#endif

#endif
