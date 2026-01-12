/*****************************************************************************************
//  iRepresentShell»æÖÆµÄÍ¼Ôª¶ÔÏó±íÊö¶¨Òå
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-11-11
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include <cstdint>


#ifndef KREPRESENTUNIT_H_HEADER_INCLUDED_C23172D7
#define KREPRESENTUNIT_H_HEADER_INCLUDED_C23172D7


//##ModelId=3DCA20D1035C
//##Documentation
//## Í¼ÔªµÄ¸÷ÖÖÀàÐÍÃ¶¾Ù¶¨Òå
enum REPRESENT_UNIT_TYPE
{
	//##Documentation
	//## Ïñµã	KRUPoint
	RU_T_POINT,
	//##Documentation
	//## Ïß¶Î	KRULine
	RU_T_LINE,
	//## ¾àÐÎ±ß¿ò	KRURect
	RU_T_RECT,
	//##Documentation
	//## Í¼ÐÎ(¾ØÐÎÍ¼ÐÎ£¬Ö»¸ø³öÁ½¸öÍ¼Ôª×ø±êµã)	KRUImage
	RU_T_IMAGE,
	//##Documentation	
	//## Í¼ÐÎ¾Ö²¿	KRUImagePart
	RU_T_IMAGE_PART,
	//##Documentation	KRUImage4
	//## Í¼ÐÎ(ËÄ±ßÐÎÍ¼ÐÎ£¬¸ø³öËÄ¸öÍ¼Ôª×ø±êµã)
	RU_T_IMAGE_4,
	//Ëõ·ÅµØ»æÖÆÍ¼ÐÎ	KRUImageStretch
	//Ö»ÔÚµ¥Æ½ÃæÉÏ»æÖÆ£¬ÇÒÍ¼ÐÎÎªISI_T_BITMAP16 ÓÐÐ§
	RU_T_IMAGE_STRETCH,
	//##Documentation
	//## ±»»®·ÖµÄ¹âÕÕÍ¼ÐÎ¡£
	RU_T_DIVIDED_LIT_IMAGE,
	//##Documentation
	//## ÒõÓ°
	RU_T_SHADOW,
	//##Documentation
	//## ¸ù¾Ý»º³åÇø¸üÐÂÍ¼ÐÎ
	RU_T_BUFFER,
};

//ÕûÊý±íÊ¾µÄ¶þÎ¬µã×ø±ê
struct KRPosition2
{
	int32_t nX;
	int32_t nY;
};

//ÕûÊý±íÊ¾µÄÈýÎ¬µã×ø±ê
struct KRPosition3
{
	int32_t nX;
	int32_t nY;
	int32_t nZ;
};

//##ModelId=3DB0C4F20145
//##Documentation
//## »æÖÆÍ¼Ôª
struct KRepresentUnit
{
	KRPosition3	oPosition;
};


//##ModelId=3DD2107D01C5
//##Documentation
//## ±íÊöÑÕÉ«µÄ½á¹¹
union KRColor
{
	struct { unsigned char b, g, r, a; } Color_b;
	uint32_t	Color_dw;
};

//##ModelId=3DB617B20036
//##Documentation
//## Ïñµã
struct KRUPoint : public KRepresentUnit
{
	//##ModelId=3DB617D2032B
	//##Documentation
	//## µãµÄÑÕÉ«
	KRColor	Color;
};

//##ModelId=3DB3C020008B
//##Documentation
//## Ïß¶Î
struct KRULine : public KRepresentUnit
{
	KRPosition3	oEndPos;
	//##ModelId=3DB3C0B80278
	//##Documentation
	//## ÏßµÄÑÕÉ«
	KRColor	Color;
};

typedef KRULine	KRURect;

//##ModelId=3DB3C020008B
//##Documentation
//## ¸üÐÂÍ¼ÐÎµÄ»º³åÇø
struct KRUBuffer : public KRepresentUnit
{
	KRPosition2	BufferEntireSize;	//»º³åÇøµÄ´óÐ¡
	KRPosition2	BufferUpdateSize;	//»º³åÇøµÄ´óÐ¡ÓÃÀ´¸üÐÂÍ¼ÐÎµÄ·¶Î§´óÐ¡
	KRPosition2	oImgLTPos;			//Ä¿±êÍ¼ÐÎµÄ×óÉÏ½Çµã×ø±ê(µ¥Î»:ÌùÍ¼ÏñËØµã)
	KRColor		Color;				//ÑÕÉ«²ÎÊý
	const unsigned char* pBuffer;	//»º³åÇøÄÚ´æµÄÖ¸Õë
};

enum RUIMAGE_RENDER_STYLE
{
	IMAGE_RENDER_STYLE_ALPHA = 0,		//´øalpha»æÖÆ
	IMAGE_RENDER_STYLE_OPACITY,			//Ö»·ÖÈ«Í¨Í¸ÓëÍêÈ«²»Í¸Ã÷£¬KRUImage::AlphaÖµ±»ºöÂÔ¡£
	IMAGE_RENDER_STYLE_3LEVEL,			//Èý¼¶alpha»æÖÆ£¬KRUImage::AlphaÖµ±»ºöÂÔ¡£
	IMAGE_RENDER_STYLE_BORDER,
	IMAGE_RENDER_STYLE_BORDER_RECT,
	IMAGE_RENDER_STYLE_ALPHA_NOT_BE_LIT,//´øalpha»æÖÆµ«ÊÇ²»±»¹âÕÕ
	IMAGE_RENDER_STYLE_ALPHA_COLOR_ADJUST,//alpha»æÖÆ£¬ÇÒ´øÆ«É«µ÷Õû
	IMAGE_RENDER_STYLE_SCREEN,// = 3,		// Screen叠加
};

enum RUIMAGE_RENDER_FLAG
{
	RUIMAGE_RENDER_FLAG_REF_SPOT = 1,	//»æÖÆ×ø±êµãÊÇ·ñÒªÓëÍ¼ÐÎµÄ²Î¿¼µã(ÖÐÐÄµã)ÎÇºÏ¡£
	//Ê¹ÓÃ´Ë±ê¼ÇÊ±,nEndX,nEndY,nEndZµÄÖµ±»ºöÂÔ¡£ÔÚÓëÆÁÄ»Æ½ÐÐµÄÃæÉÏ»æÖÆÍ¼ÐÎ£¬²¢Ê¹µÃÍ¼ÐÎµÄ²Î¿¼µã(ÖØÐÄµã)ÓënX,nY,nZÃèÊöµÄµãÏàÎÇºÏ¡£
	RUIMAGE_RENDER_FLAG_FRAME_DRAW = 2,	//Ö¡»æÖÆ·½Ê½£¬²»¿¼ÂÇ»æÖÆÖ¡Ïà¶ÔÓÚÕû¸öÍ¼ÐÎµÄÎ»ÖÃÆ«ÒÆ
};

#define	IMAGE_IS_POSITION_INIT	-1

//##ModelId=3DB60D73035F
//##Documentation
//## Í¼ÐÎ
struct KRUImage : public KRepresentUnit
{
	KRPosition3	oEndPos;

	//ÑÕÉ«Ö¸¶¨£¬¾ßÌåÊÇ·ñÓëÈçºÎÒýÓÃ£¬ÒÀÀµÓÚbRenderStyleµÄÈ¡Öµ¡£
	//ÆäÖÐalpha²¿·ÖµÄÈ¡ÖµÎª0-255¼È256½×¡£
	KRColor	Color;

	//##ModelId=3DCF64DA029D
	//##Documentation
	//## »æÖÆ·½Ê½ È¡ÖµÎªRUIMAGE_RENDER_STYLEÃ¶¾ÙÖµÖ®Ò»
	unsigned char bRenderStyle;

	//»æÖÆ±ê¼Ç,È¡ÖµÎªRUIMAGE_RENDER_FLAGÃ¶¾ÙÖµµÄ×éºÏ¡£
	unsigned char bRenderFlag;

	//##ModelId=3DCF5BB001B7
	//##Documentation
	//## »æÖÆµÄÍ¼ÐÎµÄÀàÐÍ
	int16_t nType;

	//##ModelId=3DCBEAD0014F
	//##Documentation
	//## Í¼ÐÎ×ÊÔ´µÄÎÄ¼þÃû/»òÕßÍ¼ÐÎÃû
	char szImage[128];

	//##ModelId=3DCBEAF50291
	//##Documentation
	//## Í¼ÐÎid
	uint32_t uImage;

	//##ModelId=3DCBEB170189
	//##Documentation
	//## Í¼ÐÎÔ­À´ÔÚiImageStoreÄÚµÄÎ»ÖÃ¡£
	int16_t nISPosition;

	//##ModelId=3DCF5B9C0196
	//##Documentation
	//## »æÖÆµÄÍ¼ÐÎÖ¡Ë÷Òý
	int16_t nFrame;
};

//##ModelId=3DB618EC02CC
//##Documentation
//## Í¼ÐÎ¾Ö²¿
struct KRUImagePart : public KRUImage
{
	KRPosition2	oImgLTPos;	//ÌùÍ¼×óÉÏ½Çµã×ø±ê(µ¥Î»:ÌùÍ¼ÏñËØµã)
	KRPosition2	oImgRBPos;	//ÌùÍ¼ÓÒÏÂ½Çµã×ø±ê(µ¥Î»:ÌùÍ¼ÏñËØµã)
};

struct KRUImage4 : public KRUImagePart
{
	//(µã°´Ë³Ê±ÕëÏòÅÅÁÐ)
	//µÚÒ»¸öµã×ø±êÎª oPosition
	KRPosition3	oSecondPos;	//Í¼ÔªËÄ±ßÐÎµÄµÚ¶þ¸öµãµÄ×ø±ê
	KRPosition3	oThirdPos;	//Í¼ÔªËÄ±ßÐÎµÄµÚÈý¸öµãµÄ×ø±ê
	//µÚËÄ¸öµã×ø±êÎª oEndPos
};

typedef KRUImage	KRUImageStretch;


//##ModelId=3DBFF195012B
//##Documentation
//## ±»»®·ÖµÄ¹âÕÕÍ¼ÐÎ¡£
struct KRUDividedLitImage : public KRUImage
{
	KRPosition2 oImgLTPos;	//Í¼ÐÎ±»ÒýÓÃ·¶Î§µÄ×óÉÏ½Çµãºá×ø±ê(µ¥Î»:ÌùÍ¼ÏñËØµã)

	//##ModelId=3DBFF2A5013A
	//##Documentation
	//## ¹âÕÕ»®·Ö¼ÆËã£¬»®·ÖµÄ¸ñ×ÓµÄºá¿í£¨µ¥Î»£ºÏñËØµã£©
	int32_t nCellWidth;

	//##ModelId=3DBFF2B000C0
	//##Documentation
	//## ¹âÕÕ»®·Ö¼ÆËã£¬»®·ÖµÄ¸ñ×ÓµÄ×Ý¿í£¨µ¥Î»£ºÏñËØµã£©
	int32_t nCellHeight;

	//##ModelId=3DBFF2B5032B
	//##Documentation
	//## Ë®Æ½·½Ïò»®·ÖµÄ¹âÕÕ¼ÆËã¸ñ×ÓµÄÊýÄ¿
	int32_t nNumCellH;

	//##ModelId=3DBFF2D800B1
	//##Documentation
	//## ´¹Ö±·½Ïò»®·ÖµÄ¹âÕÕ¼ÆËã¸ñ×ÓµÄÊýÄ¿
	int32_t nNumCellV;

	//##ModelId=3DBFF2DD03A9
	//##Documentation
	//## ´æ´¢¹âÕÕ¼ÆËã»®·ÖµÄ¸ñ×Ó¸÷¸ö½ÚµãµÄ¹âÕÕÇ¿¶ÈµÄÊý×é¡£Ã¿¸ö½ÚµãµÄ¹âÕÕÇ¿¶È±íÊöÎªÒ»¸ö256½×µÄÕûÊýÖµ£¬´æ´¢¿Õ¼äÎªÒ»¸ö×Ö½Ú¡£
	unsigned char* pLightIntensityData;

};


//##ModelId=3DB61A8C0298
//##Documentation
//## ÒõÓ°
struct KRUShadow : public KRepresentUnit
{
	KRPosition3	oEndPos;

	//##ModelId=3DB61ADA0024
	//##Documentation
	//## ÒõÓ°µÄÑÕÉ«
	KRColor	Color;

};


//##ModelId=3DC0F495038B
//##Documentation
//## Í¼ÐÎÀàÐÍ¶¨Òå
enum KIS_IMAGE_TYPE
{
	//##Documentation
	//## 16Î»µ¥Ö¡Î»Í¼¡£
	//## ISI_T_BITMAP16µÄ¸ñÊ½Îª D3DFMT_R5G6B5 »òÕß D3DFMT_X1R5G5B5
	//## £¬¾ßÌåÎªÄÄÖÖ¸ñÊ½ÓÉiImageStoreÄÚ²¿È·¶¨¡£¿ÉÒÔÍ¨¹ý·½·¨iImageStore::IsBitmapFormat565À´ÖªµÀÊÇÓÃÄÄÖÖ¸ñÊ½¡£
	ISI_T_BITMAP16,
	//##Documentation
	//##  spr¸ñÊ½µÄ´øalphaÑ¹ËõÍ¼ÐÎ£¬Èô¸ÉÖ¡
	ISI_T_SPR,
};


//##ModelId=3DB511F30242
//##Documentation
//## Í¼ÐÎµÄÐÅÏ¢¡£
struct KImageParam
{
	//##ModelId=3DB512190144
	//##Documentation
	//## Í¼ÐÎµÄ×ÜµÄÖ¡ÊýÄ¿
	int16_t nNumFrames;

	//##ModelId=3DB51226012F
	//##Documentation
	//## Í¼ÐÎµÄÖ¡¼ä¸ô¡£
	int16_t nInterval;

	//##ModelId=3DB5123B0158
	//##Documentation
	//## Í¼ÐÎºá¿í£¨µ¥Î»£ºÏñËØµã£©¡£
	int16_t nWidth;

	//##ModelId=3DB5123E036F
	//##Documentation
	//## Í¼ÐÎ×Ý¿í£¨µ¥Î»£ºÏñËØµã£©
	int16_t nHeight;

	//##ModelId=3DB512900118
	//##Documentation
	//## Í¼ÐÎ²Î¿¼µã£¨ÖØÐÄ£©µÄºá×ø±êÖµ¡£
	int16_t nReferenceSpotX;

	//##ModelId=3DB512A70306
	//##Documentation
	//## Í¼ÐÎ²Î¿¼µã£¨ÖØÐÄ£©µÄ×Ý×ø±êÖµ¡£
	int16_t nReferenceSpotY;

	//##ModelId=3DB512CD0280
	//##Documentation
	//## Í¼ÐÎµÄÖ¡·Ö×éÊýÄ¿£¨Í¨³£Ó¦ÓÃÎªÍ¼ÐÎ·½ÏòÊýÄ¿£©¡£
	int16_t nNumFramesGroup;

};


#endif /* KREPRESENTUNIT_H_HEADER_INCLUDED_C23172D7 */
