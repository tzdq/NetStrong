
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

//#define CONFIG_LIMIT 1 // 限制功能：一次启动邮件只能发送20次
//#define CONFIG_DEBUG 1 // 调试运行
#define CONFIG_BASE64_PWD 1// 采用加密后的密码发送，否则SendMail输入的是明文密码，不利于源码发布者保密账户


#ifdef CONFIG_LIMIT
#define PROGRAM_TITLE "NetStrong(受限版)"
#else
#define PROGRAM_TITLE "NetStrong(完整版)"
#endif

#ifdef CONFIG_DEBUG
#define PROGRAM_VERSION "Debug"
#define EMAIL_TITLE "NS-D"
#define CONFIG_MAX_AUTO_MAIL (3)
#else
#define PROGRAM_VERSION "Release"
#define EMAIL_TITLE "NS"
#define CONFIG_MAX_AUTO_MAIL (24)
#endif


#define _TICK (1)  // 1TICK
#define _SEC  (_TICK) // 1s
#define _MIN  (60 * _SEC)  // 1min
#define _HOUR (60 * _MIN)  // 1min

#ifdef CONFIG_DEBUG
#define SAVE_TICK (1*_MIN)
#else
#define SAVE_TICK (1*_HOUR)
#endif






#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


