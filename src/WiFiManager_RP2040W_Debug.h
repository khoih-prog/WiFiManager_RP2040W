/****************************************************************************************************************************
  WiFiManager_RP2040W_Debug.h
  For RP2040W boards

  WiFiManager_RP2040W is a library for the SeeedStudiocWIO Terminal/Arduino platform

  Modified from
  1) Tzapu        https://github.com/tzapu/WiFiManager
  2) Ken Taylor   https://github.com/kentaylor
  3) Khoi Hoang   https://github.com/khoih-prog/WiFiManager_RP2040W

  Built by Khoi Hoang https://github.com/khoih-prog/WiFiManager_RP2040W
  Licensed under MIT license
  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      20/08/2022 Initial Coding for RP2040W
 *****************************************************************************************************************************/

#pragma once

#ifndef WiFiManager_RP2040W_Debug_H
#define WiFiManager_RP2040W_Debug_H

#ifdef WIFIMGR_DEBUG_PORT
  #define WM_DBG_PORT      WIFIMGR_DEBUG_PORT
#else
  #define WM_DBG_PORT      Serial
#endif

// Change _WIFIMGR_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _WIFIMGR_LOGLEVEL_
  #define _WIFIMGR_LOGLEVEL_       1
#endif

const char WM_MARK[] = "[WM] ";
const char WM_SP[]   = " ";

#define WM_PRINT        WM_DBG_PORT.print
#define WM_PRINTLN      WM_DBG_PORT.println

#define WM_PRINT_MARK   WM_PRINT(WM_MARK)
#define WM_PRINT_SP     WM_PRINT(WM_SP)

////////////////////////////////////////////////////

#define LOGERROR(x)         if(_WIFIMGR_LOGLEVEL_>0) { WM_PRINT_MARK; WM_PRINTLN(x); }
#define LOGERROR0(x)        if(_WIFIMGR_LOGLEVEL_>0) { WM_PRINT(x); }
#define LOGERROR1(x,y)      if(_WIFIMGR_LOGLEVEL_>0) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINTLN(y); }
#define LOGERROR2(x,y,z)    if(_WIFIMGR_LOGLEVEL_>0) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINTLN(z); }
#define LOGERROR3(x,y,z,w)  if(_WIFIMGR_LOGLEVEL_>0) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINT(z); WM_PRINT_SP; WM_PRINTLN(w); }

////////////////////////////////////////////////////

#define LOGWARN(x)          if(_WIFIMGR_LOGLEVEL_>1) { WM_PRINT_MARK; WM_PRINTLN(x); }
#define LOGWARN0(x)         if(_WIFIMGR_LOGLEVEL_>1) { WM_PRINT(x); }
#define LOGWARN1(x,y)       if(_WIFIMGR_LOGLEVEL_>1) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINTLN(y); }
#define LOGWARN2(x,y,z)     if(_WIFIMGR_LOGLEVEL_>1) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINTLN(z); }
#define LOGWARN3(x,y,z,w)   if(_WIFIMGR_LOGLEVEL_>1) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINT(z); WM_PRINT_SP; WM_PRINTLN(w); }

////////////////////////////////////////////////////

#define LOGINFO(x)          if(_WIFIMGR_LOGLEVEL_>2) { WM_PRINT_MARK; WM_PRINTLN(x); }
#define LOGINFO0(x)         if(_WIFIMGR_LOGLEVEL_>2) { WM_PRINT(x); }
#define LOGINFO1(x,y)       if(_WIFIMGR_LOGLEVEL_>2) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINTLN(y); }
#define LOGINFO2(x,y,z)     if(_WIFIMGR_LOGLEVEL_>2) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINTLN(z); }
#define LOGINFO3(x,y,z,w)   if(_WIFIMGR_LOGLEVEL_>2) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINT(z); WM_PRINT_SP; WM_PRINTLN(w); }

////////////////////////////////////////////////////

#define LOGDEBUG(x)         if(_WIFIMGR_LOGLEVEL_>3) { WM_PRINT_MARK; WM_PRINTLN(x); }
#define LOGDEBUG0(x)        if(_WIFIMGR_LOGLEVEL_>3) { WM_PRINT(x); }
#define LOGDEBUG1(x,y)      if(_WIFIMGR_LOGLEVEL_>3) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINTLN(y); }
#define LOGDEBUG2(x,y,z)    if(_WIFIMGR_LOGLEVEL_>3) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINTLN(z); }
#define LOGDEBUG3(x,y,z,w)  if(_WIFIMGR_LOGLEVEL_>3) { WM_PRINT_MARK; WM_PRINT(x); WM_PRINT_SP; WM_PRINT(y); WM_PRINT_SP; WM_PRINT(z); WM_PRINT_SP; WM_PRINTLN(w); }

////////////////////////////////////////////////////

#endif    //WiFiManager_RP2040W_Debug_H
