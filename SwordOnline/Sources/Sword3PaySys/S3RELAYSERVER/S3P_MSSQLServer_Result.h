// S3P_MSSQLServer_Result.h: interface for the S3P_MSSQLServer_Result class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_S3P_MSSQLSERVER_RESULT_H__16A5506B_D906_41FD_91A2_9A87D4EB4E53__INCLUDED_)
#define AFX_S3P_MSSQLSERVER_RESULT_H__16A5506B_D906_41FD_91A2_9A87D4EB4E53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KStdAfx.h"
#include "S3PResultVBC.h"

#include "msado15.tlh"

class S3P_MSSQLServer_Result : public S3PResultVBC  
{
protected:
	_RecordsetPtr m_pResult;
public:
	INT m_nAddRef;
	S3P_MSSQLServer_Result()
	{
		m_pResult = NULL;
		m_nAddRef = 0;
	}
	virtual ~S3P_MSSQLServer_Result();

	virtual INT GetResult( _Recordset** ppResult);
	virtual INT num_rows() const;
	virtual INT num_fields() const;
	virtual void data_seek (UINT offset, INT nType) const;
	virtual bool get_field_data(UINT nfieldindex, void* pData, ULONG nsize);
	virtual INT unuse();
};

#endif // !defined(AFX_S3P_MSSQLSERVER_RESULT_H__16A5506B_D906_41FD_91A2_9A87D4EB4E53__INCLUDED_)
