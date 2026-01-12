#ifndef __epsJO_h__
#define __epsJO_h__

#include "epassapi.h"
#include "des.h"

/************************************************
* Function name		: epj_Init
* Description	    : 
* Return type		: INT(true/flase) 
* Argument			: const char*pszPassword:超级用户的密码(初始密码是“rockey”)
* Remark			: 最先调用
************************************************/
INT __stdcall epj_Init(const char*pszPassword);

/************************************************
* Function name		: epj_UnInit
* Description	    : 
* Return type		: void 
* Remark			: 最后调用
************************************************/
void __stdcall epj_UnInit();

/************************************************
* Function name		: epj_ClearUSBKey
* Description	    : 清除USBKEY中的内容
* Return type		: INT(true/flase) 
* Remark			: 重写key时需要清除以前的内容
************************************************/
INT __stdcall epj_ClearUSBKey();

/************************************************
* Function name		: epj_WriteKeyCode
* Description	    : 向key中写入一段数据
* Return type		: INT(true/flase) 
* Argument			: const char* lpExeData：将它作MD5 hash，得到的16字节摘要作为
						DES的密钥加密lpKeyCode数据
* Argument			: INT nExeDataLen：lpExeData长度
* Argument			: const char* lpKeyCode：此数据被加密后写入key中
* Argument			: INT nKeyCodeLen：lpKeyCode的长度
* Argument			: INT nIndex：唯一标识被加密的数据(lpKeyCode),必须 >=0 && <256
* Remark			: USBKEY只有32K的存储空间
************************************************/
INT __stdcall epj_WriteKeyCode(const char* lpExeData,INT nExeDataLen,
					  const char* lpKeyCode,INT nKeyCodeLen,INT nIndex);

/************************************************
* Function name		: epj_ReadKeyCode
* Description	    : 从key中读出一段数据
* Return type		: INT(true/flase) 
* Argument			: const char* lpExeData：将它作MD5 hash，得到的16字节摘要作为
						DES的密钥解密从key中读出的数据
* Argument			: INT nExeDataLen：lpExeData长度
* Argument			: |*out*|char** lppKeyCode：存放解密后的数据，用::free()释放空间
* Argument			: |*out*|INT* pnKeyCodeLen：解密后的数据的长度
* Argument			: INT nIndex：唯一标识被加密的数据,必须 >=0 && <256
* Remark			: 用完lppKeyCode后需要::free()掉
************************************************/
INT __stdcall epj_ReadKeyCode(const char* lpExeData,INT nExeDataLen,
					 /*out*/char** lppKeyCode,/*out*/INT* pnKeyCodeLen,INT nIndex);

/************************************************
* Function name		: epj_IsUSBKeyExist
* Description	    : 检查USNKEY是否存在
* Return type		: INT ：true：存在；false：不存在或发生错误
* Remark			: 在epj_Init()时在内存生成一个随机数(INT)，在每次epj_ReadKeyCode后将此
						随机数异或被解密数据的前4个字节，并写入到key中，调用epj_IsUSBKeyExist()
						时，比较key中的值是否与内存中的值相等，并从新生成随机数，写入到key中
************************************************/
INT __stdcall epj_IsUSBKeyExist();

#endif//__epsJO_h__