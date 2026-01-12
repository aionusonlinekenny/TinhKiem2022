#include "stdafx.h"
#include "Exception.h"
#include <iostream>

/*
 * namespace OnlineGameLib::Win32
 */

namespace OnlineGameLib {
	namespace Win32 {

		/*
		 * CException
		 */
		CException::CException(const _tstring& where, const _tstring& message) : m_where(where), m_message(message)
		{
			std::cerr << "Exception: " << GetMessage().c_str() << " - Where: % s" << GetWhere().c_str() << std::endl;
		}
		CException::CException(const _tstring& where) : m_where(where)
		{
			std::cerr << "Exception Where: % s" << GetWhere().c_str() << std::endl;
		}

		_tstring CException::GetWhere() const
		{
			return m_where;
		}

		_tstring CException::GetMessage() const
		{
			return m_message;
		}

		void CException::MessageBox(HWND hWnd /* = NULL */) const
		{
			::MessageBox(hWnd, GetMessage().c_str(), GetWhere().c_str(), MB_ICONSTOP);
			//printf("Exception: %s - Where: %s\n", GetMessage().c_str(), GetWhere().c_str());
		}

	} // End of namespace OnlineGameLib
} // End of namespace Win32