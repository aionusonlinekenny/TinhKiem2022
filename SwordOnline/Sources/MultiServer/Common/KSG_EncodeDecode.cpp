//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   KSG_EncodeDecode.cpp
//  Version     :   1.0
//  Creater     :   
//  Date        :   2003-6-3 10:28:57
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KSG_EncodeDecode.h"
#include <string.h>

int32_t KSG_DecodeEncode(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey)
{
	uint32_t* puBuf = (uint32_t*)pbyBuf;
	uint32_t uKey = *puKey;
	uint32_t uRemainSize = uSize % 4;
	uSize /= 4;

	while (uSize-- > 0)
		*puBuf++ ^= uKey;

	pbyBuf = (unsigned char*)puBuf;

	while (uRemainSize-- > 0)
	{
		*pbyBuf++ ^= (unsigned char)(uKey & 0xff);
		uKey >>= 8;
	}

	*puKey = (*puKey * 31) + 134775813L;

	return true;
}

//int KSG_DecodeEncode_ASM(size_t uSize, unsigned char* pbyBuf, unsigned* puKey)
//{
//	__asm mov eax, [uSize]
//		__asm mov esi, [puKey]
//		__asm mov edi, [pbyBuf]
//		__asm mov ecx, eax
//	__asm mov edx, [esi]        // key
//
//		__asm shr eax, 2
//	__asm jz  Next1
//
//	Loop1 :
//	__asm mov ebx, [edi]
//		__asm add edi, 4
//	__asm xor ebx, edx
//	__asm dec eax
//	__asm mov[edi - 4], ebx
//	__asm jnz Loop1
//
//
//	Next1 :
//	__asm and ecx, 3
//	__asm mov eax, edx  // Key
//	__asm jz Next2
//
//	Loop2 :
//	__asm mov bl, [edi]
//		__asm inc edi
//	__asm xor bl, al
//	__asm shr eax, 8
//	__asm mov[edi - 1], bl
//	__asm dec ecx
//	__asm jnz Loop2
//	Next2 :
//
//	__asm mov eax, edx      // NewKey = OldKey * 31 +  134775813L
//	__asm shl edx, 5
//	__asm sub edx, eax
//	__asm add edx, 134775813    // 08088405H
//	__asm mov[esi], edx
//
//	return true;
//}

int32_t KSG_DecodeEncode_CPP(size_t uSize, unsigned char* pbyBuf, uint32_t* puKey) {
	size_t dwordCount = uSize / 4;
	uint32_t* pDwordBuf = reinterpret_cast<uint32_t*>(pbyBuf);
	uint32_t key = *puKey;

	for (size_t i = 0; i < dwordCount; ++i) {
		pDwordBuf[i] ^= key;
	}

	size_t remainingBytes = uSize % 4;
	if (remainingBytes > 0) {
		unsigned char* pRemainingBuf = pbyBuf + dwordCount * 4;

		for (size_t i = 0; i < remainingBytes; ++i) {
			pRemainingBuf[i] ^= static_cast<unsigned char>(key);
			key >>= 8;
		}
	}

	// NewKey = OldKey * 31 + 134775813L
	*puKey = (key << 5) - key + 134775813;

	return true;
}

int32_t KSG_DecodeEncode_StrongerCPP(size_t dataSize, unsigned char* dataBuffer, uint32_t* encryptionKey) {
	size_t dwordCount = dataSize / sizeof(uint32_t);
	uint32_t* pDwordBuf = reinterpret_cast<uint32_t*>(dataBuffer);
	uint32_t key = *encryptionKey;

	// Encryption
	for (size_t i = 0; i < dwordCount; ++i) {
		pDwordBuf[i] ^= key;
		key = (key << 5) - key + 134775813;  // Stronger key update
	}

	// Handle remaining bytes
	size_t remainingBytes = dataSize % sizeof(uint32_t);
	if (remainingBytes > 0) {
		unsigned char* pRemainingBuf = dataBuffer + dwordCount * sizeof(uint32_t);

		for (size_t i = 0; i < remainingBytes; ++i) {
			pRemainingBuf[i] ^= static_cast<unsigned char>(key);
			key = (key << 5) - key + 134775813;  // Stronger key update
		}
	}

	// Stronger key update for the final key
	*encryptionKey = (key << 5) - key + 134775813;

	return true;
}