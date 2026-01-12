#ifndef __COREDRAWGAMEOBJ__H__
#define __COREDRAWGAMEOBJ__H__

struct KLightInfo;

//绘制游戏对象
void	CoreDrawGameObj(UINT uObjGenre, UINT uId, INT x, INT y, INT Width, INT Height, INT nParam);

//查询对象的光源属性
void	CoreGetGameObjLightInfo(UINT uObjGenre, UINT uId, KLightInfo* pLightInfo);

#endif //#ifndef __COREDRAWGAMEOBJ__H__