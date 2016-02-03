/************************************************************************
 * ʹ����³��(zhangluduo)��д�Ĵ��룬�������ɵ�dll�ļ�
 * ���ã�
 * project -> setting -> c/c++ ->Code Generation -> use run-time library (Debug Multithreaded DLL)
 * project -> setting -> c/c++ -> Preprocessor -> preprocessor definitions ->_AFXDLL
 * ����#pragma comment(lib,"..·��\\zsmtp.lib")
 ************************************************************************/
#ifndef _ZLD_H_
#define _ZLD_H_


#ifdef ZLD_EXPORTS
	#define ZLD_API __declspec(dllexport)
#else
	#define ZLD_API //__declspec(dllimport)
	#pragma message("Add zld.lib")
	#pragma comment(lib,"zld.lib")
#endif

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "ZSmtp.h"
#include "ZString.h"


#endif