//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_StringProcess.cpp
//  Version     :   1.0
//  Creater     :   Freeway Chen
//  Date        :   2003-8-1 12:45:11
//  Comment     :   Process String to Int, Skip Symbol
//
//////////////////////////////////////////////////////////////////////////////////////

#include "KWin32.h"
#include <ctype.h>

#include "KSG_StringProcess.h"
#include <math.h>
#include <time.h>
#include <ctime>

int KSG_StringGetInt(const char** ppcszString, int nDefaultValue)
{
	int nResult = false;
	int nRetValue = 0;
	int nNegSignFlag = false;
	int nRetValueValidFlag = false;
	const char* pcszString = NULL;

	if (!ppcszString)
		goto Exit0;

	pcszString = *ppcszString;

	if (!pcszString)
		goto Exit0;

	while (isspace(*pcszString) || (*pcszString) == '"')
		pcszString++;

	if ((*pcszString) == '\0')
		goto Exit0;

	if ((*pcszString) == '-')
	{
		nNegSignFlag = true;
		pcszString++;

		// Skip Prev Space
		while (isspace(*pcszString) || (*pcszString) == '"')
			pcszString++;

		if ((*pcszString) == '\0')
			goto Exit0;
	}


	while (isdigit(*pcszString))
	{
		nRetValueValidFlag = true;

		nRetValue = nRetValue * 10 + ((int)(*pcszString - '0'));

		pcszString++;
	}

	nResult = true;
Exit0:

	if (pcszString)
	{
		if (ppcszString)
			*ppcszString = pcszString;
	}

	if (nNegSignFlag)
		nRetValue = -nRetValue;

	if (
		(!nResult) ||
		(!nRetValueValidFlag)
		)
		nRetValue = nDefaultValue;


	return nRetValue;
}


bool KSG_StringSkipSymbol(const char** ppcszString, int nSymbol)
{
	bool bResult = false;
	const char* pcszString = NULL;

	if (!ppcszString)
		goto Exit0;

	pcszString = *ppcszString;

	if (!pcszString)
		goto Exit0;

	while (isspace(*pcszString))
		pcszString++;

	if (((unsigned)(*pcszString)) != (unsigned)nSymbol)
		goto Exit0;

	pcszString++;   // Skip Symbol

	bResult = true;
Exit0:

	if (pcszString)
	{
		if (ppcszString)
			*ppcszString = pcszString;
	}

	return bResult;
}

int KSG_GetCurSec()
{
	time_t curtime;
	time(&curtime);
	return curtime - 1451581200;
}

int KSG_GetCurMin()
{
	std::time_t current_time = std::time(nullptr); // Lấy Unix time cho thời điểm hiện tại
	return static_cast<int>(current_time / 60 * 60); 
}

// Hàm tính Unix time từ ngày, tháng, năm, giờ, phút, giây
std::time_t CalculateUnixTime(int year = 1970, int month = 1, int day = 1, int hour = 0, int minute = 0, int second = 0) {
	struct std::tm timeinfo = { 0 };
	timeinfo.tm_year = year - 1900; // Năm tính từ 1900
	timeinfo.tm_mon = month - 1;    // Tháng tính từ 0
	timeinfo.tm_mday = day;
	timeinfo.tm_hour = hour;
	timeinfo.tm_min = minute;
	timeinfo.tm_sec = second;
	return std::mktime(&timeinfo);
}

int KSG_GetExpiredSec(int year, int month, int day, int hour, int minute, int second)
{
	time_t expiredDate = CalculateUnixTime(year, month, day, hour, minute, second);
	//time(&expiredDate);
	return expiredDate - 1451581200;
}

#define BLOCKNUM 9
int KSG_StringSetValue(int t, int n, int p, int v)
{
	if (v < 0 || v >= (pow((double)10, t)))return n;
	int r = 0, l1 = 0, l2 = 0, i = 0;
	if (p<1 || p>BLOCKNUM / t)return n;
	char str[BLOCKNUM + 1], str1[BLOCKNUM + 1], str2[BLOCKNUM + 1];
	while (i < BLOCKNUM) {
		str[i] = '0';
		i++;
	}
	i = 0;
	itoa(n, str1, 10);
	itoa(v, str2, 10);
	l1 = strlen(str1);
	l2 = strlen(str2);
	str1[l1] = '\0';
	if (l2 < t) {
		memmove(str2 + (t - l2), str2, l2);
		while (l2 < t) {
			str2[i] = '0';
			i++;
			l2++;
		}
		i = 0;
	}
	str2[l2] = '\0';
	memcpy(str + (BLOCKNUM - l1), str1, l1);
	memcpy(str + ((p * t) - t) + l2 - t, str2, t - (l2 - t));
	str[BLOCKNUM] = '\0';
	r = atoi(str);
	return r;
}

int KSG_StringGetValue(int t, int n, int p)
{
	int r = 0, i = 0, l1 = 0, c = 0;
	if (p < 1 || p > BLOCKNUM / t)
		return n;
	char str[BLOCKNUM + 1], str1[BLOCKNUM + 1], str2[BLOCKNUM + 1] = {};

	while (i < BLOCKNUM) {
		str[i] = '0';
		i++;
	}
	i = 0;
	itoa(n, str1, 10);
	l1 = strlen(str1);
	str1[l1] = '\0';
	memcpy(str + (BLOCKNUM - l1), str1, l1);
	while (str[i]) {
		if (p * t - t == i) {
			while (c < t) {
				str2[c] = str[i + c];
				c++;
			}
			str2[t] = '\0';
			break;
		}
		i++;
	}
	r = atoi(str2);
	return r;
}