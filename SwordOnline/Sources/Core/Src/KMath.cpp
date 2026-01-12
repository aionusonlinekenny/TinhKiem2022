#include "KCore.h"
#include <math.h>
#include "GameDataDef.h"
#include "KMath.h"


#ifdef _USBKEY
#include "USBKey/epsJO.h" 

//---------------------------------------------------------------------------
// ÕýÏÒ±í (½«¸¡µãÊý *1024 ÕûÐÍ»¯)
INT* g_nSin = NULL;

// ÓàÏÒ±í (½«¸¡µãÊý *1024 ÕûÐÍ»¯)
INT* g_nCos = NULL;

// ÕýÏÒÓàÏÒµÄ²é±íº¯Êý´úÂë»º³åÇø
unsigned char* g_InternalDirSinCosCode = NULL;


//---------------------------------------------------------------------------
// ÎåÐÐÏàÉúÏà¿Ë

// ÎåÐÐÏàÉúÏà¿Ëº¯Êý´úÂë»º³åÇø
unsigned char* g_InternalIsAccrueConquerCode = NULL;
//---------------------------------------------------------------------------

INT g_nEPJInitFlag = false;

// {982CD92C-027E-4638-8BD4-5855FFB6DCC1}
const unsigned char g_cExeData[16] = {
	0x98, 0x2c, 0xd9, 0x2c, 0x02, 0x7e, 0x46, 0x38,
	0x8b, 0xd4, 0x58, 0x55, 0xff, 0xb6, 0xdc, 0xc1
};


INT _ReadDataFromUSBKey(INT nSize, unsigned char abyBuffer[], INT* pnRetSize, INT nIndex)
{
	INT nResult = false;
	INT nRetCode = false;

	INT   nReadSize = 0;
	unsigned char* pbyReadBuf = NULL;

	if (pnRetSize)
		*pnRetSize = 0;

	if (nSize <= 0)
		goto Exit0;

	if (!abyBuffer)
		goto Exit0;

	if (nIndex < 0)
		goto Exit0;

	nRetCode = epj_ReadKeyCode(
		(const char*)g_cExeData, sizeof(g_cExeData),
		(char**)(&pbyReadBuf), &nReadSize,
		nIndex
	);
	if (!nRetCode)
		goto Exit0;

	if (!pbyReadBuf)
		goto Exit0;

	if (nReadSize <= 0)
		goto Exit0;

	if (nReadSize > nSize)
		goto Exit0;

	memcpy(abyBuffer, pbyReadBuf, nReadSize);

	if (pnRetSize)
		*pnRetSize = nReadSize;

	free(pbyReadBuf);
	pbyReadBuf = NULL;
	nReadSize = 0;

	nResult = true;
Exit0:
	return nResult;
}

INT g_InitMath()
{
	INT nResult = false;
	INT nRetCode = false;

	if (g_nEPJInitFlag)
		goto Exit1;

	g_nSin = new INT[1024 / sizeof(INT)];
	if (!g_nSin)
		goto Exit0;

	g_nCos = new INT[1024 / sizeof(INT)];
	if (!g_nCos)
		goto Exit0;

	g_InternalDirSinCosCode = new unsigned char[1024];
	if (!g_InternalDirSinCosCode)
		goto Exit0;

	g_InternalIsAccrueConquerCode = new unsigned char[1024];
	if (!g_InternalIsAccrueConquerCode)
		goto Exit0;

	// Õâ¸öÊ±ºò¿ªÊ¼´ÓUSBKeyÖÐ¶ÁÈëÏàÓ¦µÄÊý¾Ý
	nRetCode = epj_Init("rockey");
	if (!nRetCode)
		goto Exit0;

	g_nEPJInitFlag = true;

	nRetCode = _ReadDataFromUSBKey(
		1024, (unsigned char*)g_nSin, NULL,
		0
	);
	if (!nRetCode)
		goto Exit0;

	nRetCode = _ReadDataFromUSBKey(
		1024, (unsigned char*)g_nCos, NULL,
		1
	);
	if (!nRetCode)
		goto Exit0;

	nRetCode = _ReadDataFromUSBKey(
		1024, (unsigned char*)g_InternalDirSinCosCode, NULL,
		2
	);
	if (!nRetCode)
		goto Exit0;


	nRetCode = _ReadDataFromUSBKey(
		1024, (unsigned char*)g_InternalIsAccrueConquerCode, NULL,
		3
	);
	if (!nRetCode)
		goto Exit0;

Exit1:
	nResult = true;
Exit0:
	if (!nResult)
	{
		if (g_nEPJInitFlag)
		{
			epj_UnInit();
			g_nEPJInitFlag = false;
		}

		if (g_InternalIsAccrueConquerCode)
		{
			delete[]g_InternalIsAccrueConquerCode;
			g_InternalIsAccrueConquerCode = NULL;
		}

		if (g_InternalDirSinCosCode)
		{
			delete[]g_InternalDirSinCosCode;
			g_InternalDirSinCosCode = NULL;
		}

		if (g_nCos)
		{
			delete[]g_nCos;
			g_nCos = NULL;
		}

		if (g_nSin)
		{
			delete[]g_nSin;
			g_nSin = NULL;
		}
	}
	return nResult;
}

INT g_UnInitMath()
{
	INT nResult = false;

	if (g_nEPJInitFlag)
	{
		epj_UnInit();
		g_nEPJInitFlag = false;
	}


	if (g_InternalIsAccrueConquerCode)
	{
		delete[]g_InternalIsAccrueConquerCode;
		g_InternalIsAccrueConquerCode = NULL;
	}

	if (g_InternalDirSinCosCode)
	{
		delete[]g_InternalDirSinCosCode;
		g_InternalDirSinCosCode = NULL;
	}

	if (g_nCos)
	{
		delete[]g_nCos;
		g_nCos = NULL;
	}

	if (g_nSin)
	{
		delete[]g_nSin;
		g_nSin = NULL;
	}

	nResult = true;
	//Exit0:
	return nResult;
}



#else
// ²»ÐèÒª¼ÓÔØUSBKey

	// ÕýÏÒ±í (½«¸¡µãÊý *1024 ÕûÐÍ»¯)
static INT		g_nSinBuffer[64] =
{
	1024,	1019,	1004,	979,	946,	903,	851,	791,
	724,	649,	568,	482,	391,	297,	199,	100,
	0,	   -100,	-199,	-297,	-391,	-482,	-568,	-649,
	-724,	-791,	-851,	-903,	-946,	-979,	-1004,	-1019,
	-1024,	-1019,	-1004,	-979,	-946,	-903,	-851,	-791,
	-724,	-649,	-568,	-482,	-391,	-297,	-199,	-100,
	0,	     100,	199,	297,	391,	482,	568,	649,
	724,	791,	851,	903,	946,	979,	1004,	1019
};

//---------------------------------------------------------------------------
// ÓàÏÒ±í (½«¸¡µãÊý *1024 ÕûÐÍ»¯)
INT		g_nCosBuffer[64] =
{
	0,	    -100,	-199,	-297,	-391,	-482,	-568,	-649,
	-724,	-791,	-851,	-903,	-946,	-979,	-1004,	-1019,
	-1024,	-1019,	-1004,	-979,	-946,	-903,	-851,	-791,
	-724,	-649,	-568,	-482,	-391,	-297,	-199,	-100,
	0,	     100,	199,	297,	391,	482,	568,	649,
	724,	791,	851,	903,	946,	979,	1004,	1019,
	1024,	1019,	1004,	979,	946,	903,	851,	791,
	724,	649,	568,	482,	391,	297,	199,	100,
};

// ÕýÏÒÓàÏÒµÄ²é±íº¯Êý´úÂë»º³åÇø
unsigned char g_InternalDirSinCosCodeBuffer[] =
{
	0x8B, 0x44, 0x24, 0x08, 0x85, 0xC0, 0x7C, 0x16,
	0x8B, 0x4C, 0x24, 0x0C, 0x3B, 0xC1, 0x7D, 0x0E,
	0xC1, 0xE0, 0x06, 0x99, 0xF7, 0xF9, 0x8B, 0x4C,
	0x24, 0x04, 0x8B, 0x04, 0x81, 0xC3, 0x83, 0xC8,
	0xFF, 0xC3
};

// ÕýÏÒÓàÏÒµÄ²é±íº¯Êý´úÂë
//INT	__cdecl g_InternalDirSinCos(INT pSinCosTable[], INT nDir, INT nMaxDir)
//{
//	if (nDir < 0 || nDir >= nMaxDir)
//		return -1;
//
//	INT	nIndex = (nDir << 6) / nMaxDir;
//
//	return pSinCosTable[nIndex];
//}


// ÎåÐÐÏàÉúÏà¿Ëº¯Êý´úÂë»º³åÇø
static unsigned char g_InternalIsAccrueConquerCodeBuffer[] =
{
	0x8B, 0x44, 0x24, 0x08, 0x85, 0xC0, 0x7C, 0x1E,
	0x83, 0xF8, 0x05, 0x7D, 0x19, 0x8B, 0x54, 0x24,
	0x0C, 0x56, 0x8B, 0x74, 0x24, 0x08, 0x57, 0x33,
	0xC9, 0x8B, 0x3C, 0x86, 0x3B, 0xD7, 0x5F, 0x0F,
	0x94, 0xC1, 0x8B, 0xC1, 0x5E, 0xC3, 0x33, 0xC0,
	0xC3
};

// ÎåÐÐÏàÉúÏà¿Ëº¯Êý´úÂë
//INT __cdecl g_InternalIsAccrueConquer(INT pAccrueConquerTable[], INT nSrcSeries, INT nDesSeries)
//{
//	if (nSrcSeries < 0 || nSrcSeries >= series_num)
//		return false;
//
//	return (nDesSeries == pAccrueConquerTable[nSrcSeries]);
//}

//---------------------------------------------------------------------------
// ÕýÏÒ±í (½«¸¡µãÊý *1024 ÕûÐÍ»¯)
INT* g_nSin = g_nSinBuffer;

// ÓàÏÒ±í (½«¸¡µãÊý *1024 ÕûÐÍ»¯)
INT* g_nCos = g_nCosBuffer;

// ÕýÏÒÓàÏÒµÄ²é±íº¯Êý´úÂë»º³åÇø
unsigned char* g_InternalDirSinCosCode = g_InternalDirSinCosCodeBuffer;


//---------------------------------------------------------------------------
// ÎåÐÐÏàÉúÏà¿Ë

// ÎåÐÐÏàÉúÏà¿Ëº¯Êý´úÂë»º³åÇø
unsigned char* g_InternalIsAccrueConquerCode = g_InternalIsAccrueConquerCodeBuffer;
//---------------------------------------------------------------------------

INT g_InitMath()
{
	return true;

}

INT g_UnInitMath()
{
	return true;
}

#endif

//---------------------------------------------------------------------------
// ÎåÐÐÏàÉúÏà¿Ë
//INT		g_nAccrueSeries[series_num];
//INT		g_nConquerSeries[series_num];
//
//void g_InitSeries()
//{
//	g_nAccrueSeries[series_metal] = series_water;
//	g_nConquerSeries[series_metal] = series_wood;
//	g_nAccrueSeries[series_wood] = series_fire;
//	g_nConquerSeries[series_wood] = series_earth;
//	g_nAccrueSeries[series_water] = series_wood;
//	g_nConquerSeries[series_water] = series_fire;
//	g_nAccrueSeries[series_fire] = series_earth;
//	g_nConquerSeries[series_fire] = series_metal;
//	g_nAccrueSeries[series_earth] = series_metal;
//	g_nConquerSeries[series_earth] = series_water;
//}

INT		g_nAccrueSeries[series_num];
INT		g_nConquerSeries[series_num];
INT		g_nAccruedSeries[series_num];
INT		g_nConqueredSeries[series_num];

VOID g_InitSeries()
{
	// 五行生克关系
	g_nAccrueSeries[series_metal] = series_water;  //相生
	g_nConquerSeries[series_metal] = series_wood;
	g_nAccruedSeries[series_metal] = series_earth;
	g_nConqueredSeries[series_metal] = series_fire;

	g_nAccrueSeries[series_wood] = series_fire;
	g_nConquerSeries[series_wood] = series_earth;
	g_nAccruedSeries[series_wood] = series_water;
	g_nConqueredSeries[series_wood] = series_metal;
	g_nAccrueSeries[series_water] = series_wood;
	g_nConquerSeries[series_water] = series_fire;
	g_nAccruedSeries[series_water] = series_metal;
	g_nConqueredSeries[series_water] = series_earth;
	g_nAccrueSeries[series_fire] = series_earth;
	g_nConquerSeries[series_fire] = series_metal;
	g_nAccruedSeries[series_fire] = series_wood;
	g_nConqueredSeries[series_fire] = series_water;
	g_nAccrueSeries[series_earth] = series_metal;
	g_nConquerSeries[series_earth] = series_water;
	g_nAccruedSeries[series_earth] = series_fire;
	g_nConqueredSeries[series_earth] = series_wood;
}