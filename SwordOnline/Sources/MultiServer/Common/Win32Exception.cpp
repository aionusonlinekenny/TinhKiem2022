#include "stdafx.h"
#include "Win32Exception.h"
#include "Utils.h"
#include <iostream>

/*
 * namespace OnlineGameLib::Win32
 */

namespace OnlineGameLib {
	namespace Win32 {

		CWin32Exception::CWin32Exception(const _tstring& where, DWORD error) : CException(where), m_error(error)
		{
			//#ifdef _DEBUG
			std::cerr << "Exception Where: % s" << GetWhere().c_str() << std::endl;
			//#endif
		}

		CWin32Exception::CWin32Exception(const _tstring& where) : CException(where)
		{
			m_error = 0;
			//#ifdef _DEBUG
			std::cerr << "Exception Where: % s" << GetWhere().c_str() << std::endl;
			//#endif
		}

	} // End of namespace OnlineGameLib
} // End of namespace Win32