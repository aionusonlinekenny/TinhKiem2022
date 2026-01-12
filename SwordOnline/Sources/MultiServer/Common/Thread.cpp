#include "stdafx.h"
#include "Thread.h"
#include "Macro.h"

#include <process.h> //Thread define

#include "Win32Exception.h"

/*
 * namespace OnlineGameLib::Win32
 */

namespace OnlineGameLib
{
	namespace Win32
	{

		CThread::CThread() : m_hThread(NULL), m_bShouldExit(0)
		{
		}

		CThread::~CThread()
		{
			SAFE_CLOSEHANDLE(m_hThread);
		}

		HANDLE CThread::GetHandle() const
		{
			return m_hThread;
		}

		void CThread::Start()
		{
			if (m_hThread == NULL)
			{
				unsigned int threadID = 0;
				// 开始创建线程
				m_hThread = (HANDLE)::_beginthreadex(0,
					0,
					ThreadFunction,
					(void*)this,
					0,
					&threadID);
				// printf("----Start creating thread thread----\n");
				// The first parameter: security attribute, NULL is the default security attribute
				// 2nd parameter: Specify the size of the thread stack. If 0, the thread stack size is the same as the thread that created it. Generally use 0
				// The third parameter: Specify the address of the thread function, which is the address of the function called and executed by the thread (just use the function name, the function name represents the address)
				// The fourth parameter: the pointer of the parameter passed to the thread can be passed in as the pointer of the object and then converted into a pointer of the corresponding class in the thread function
				// The fifth parameter: initial state of the thread, 0: run immediately; CREATE_SUSPEND: suspended (suspended)
				// The 6th parameter: the address used to record the thread ID

				if (m_hThread == NULL)
				{
					throw CWin32Exception(_T("CThread::Start() - _beginthreadex"), GetLastError());
				}
			}
			else
			{
				throw CException(_T("CThread::Start()"), _T("Thread already running - you can only call Start() once!"));
			}
		}

		void CThread::Wait() const
		{
			if (!Wait(INFINITE))
			{
				throw CException(_T("CThread::Wait()"), _T("Unexpected timeout on infinite wait"));
			}
		}

		bool CThread::Wait(DWORD timeoutMillis) const
		{
			bool ok;

			if (!m_hThread)
			{
				return true;
			}

			DWORD result = ::WaitForSingleObject(m_hThread, timeoutMillis);

			if (result == WAIT_TIMEOUT)
			{
				ok = false;
			}
			else if (result == WAIT_OBJECT_0)
			{
				ok = true;
			}
			else
			{
				throw CWin32Exception(_T("CThread::Wait() - WaitForSingleObject"), ::GetLastError());
			}

			return ok;
		}
		// 线程回调函数
		unsigned int __stdcall CThread::ThreadFunction(void* pV)
		{
			int result = 0;

			CThread* pThis = (CThread*)pV;

			if (pThis && pThis->m_hThread)
			{
				try
				{
					result = pThis->Run();
				}
				catch (...)
				{
					//	TRACE( "CThread::ThreadFunction exception!" );
					//printf("CThread::ThreadFunction exception!\n");
				}
			}

			return result;
		}
		// 终止线程
		void CThread::Terminate(DWORD exitCode /* = 0 */)
		{
			if (m_hThread && !::TerminateThread(m_hThread, exitCode))
			{
				// TRACE( "CThread::Terminate error!" );
			}

			SAFE_CLOSEHANDLE(m_hThread);
		}

	} // End of namespace OnlineGameLib
} // End of namespace Win32