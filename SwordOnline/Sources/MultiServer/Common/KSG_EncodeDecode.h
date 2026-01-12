//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_EncodeDecode.h
//  Version     :   1.0
//  Creater     :   
//  Date        :   2003-6-3 10:29:43
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef _KSG_ENCODEDECODE_H
#define _KSG_ENCODEDECODE_H 1

#include <crtdbg.h>
#include <cstdint>

int32_t KSG_DecodeEncode(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey);
int32_t KSG_DecodeEncode_ASM(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey);
int32_t KSG_DecodeEncode_CPP(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey);
int32_t KSG_DecodeEncode_StrongerCPP(size_t dataSize, unsigned char* dataBuffer, uint32_t* encryptionKey);
int32_t KSG_EncryptDecrypt_AES(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey, bool bEncrypt);

inline int32_t KSG_DecodeBuf(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey)
{
	_ASSERT(uSize);
	_ASSERT(pbyBuf);
	_ASSERT(puKey);

	uint32_t uKey = *puKey;
	//return KSG_DecodeEncode(uSize, pbyBuf, puKey);
	//return KSG_DecodeEncode_ASM(uSize, pbyBuf, &uKey);
	return KSG_DecodeEncode_StrongerCPP(uSize, pbyBuf, &uKey);
	//return KSG_EncryptDecrypt_AES(uSize, pbyBuf, &uKey, false);
}

inline int32_t KSG_EncodeBuf(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey)
{
	_ASSERT(uSize);
	_ASSERT(pbyBuf);
	_ASSERT(puKey);

	uint32_t uKey = *puKey;
	//return KSG_DecodeEncode(uSize, pbyBuf, puKey);
	//return KSG_DecodeEncode_ASM(uSize, pbyBuf, &uKey);
	return KSG_DecodeEncode_StrongerCPP(uSize, pbyBuf, &uKey);
	//return KSG_EncryptDecrypt_AES(uSize, pbyBuf, &uKey, true);
}

#endif  // _KSG_ENCODEDECODE_H