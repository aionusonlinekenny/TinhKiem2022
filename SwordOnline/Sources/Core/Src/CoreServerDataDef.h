#ifndef CORESERVERDATADEF_H
#define CORESERVERDATADEF_H

#ifndef _STANDALONE 
#include "../../lib/s3dbinterface.h"
#else
#include "S3DBInterface.h"
#endif

#include "CoreUseNameDef.h"

class KMutex;

struct KCoreConnectInfo
{
	INT	nNumPlayer;
};

typedef struct
{
	char				szAccount[32];
	INT					nRoleCount;
	S3DBI_RoleBaseInfo	PlayerBaseInfo[MAX_PLAYER_IN_ACCOUNT];
	INT					nStep;
	UINT		nParam;
	DWORD				dwKey;
} LoginData;

#include "KPlayerDef.h"
#endif
