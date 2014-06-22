#ifdef PLATFORM_OSX
#include <cstdio>
#define SGFatalErrorIf(cond, msg) if(cond) { printf(msg); __builtin_trap(); }
#define SGFatalError(msg) { printf(msg); __builtin_trap(); }
#define SGDebugBreakIf(cond, msg) if(cond) { printf(msg); __builtin_trap(); }
#define SGDebugBreak(msg) { printf(msg); __builtin_trap(); }
#endif

#ifdef APPLE_IOS
#include <cstdio>
#define SGFatalErrorIf(cond, msg) if(cond) { printf(msg); __builtin_trap(); }
#define SGFatalError(msg) { printf(msg); __builtin_trap(); }
#define SGDebugBreakIf(cond, msg) if(cond) { printf(msg); __builtin_trap(); }
#define SGDebugBreak(msg) { printf(msg); __builtin_trap(); }
#endif

#ifdef WIN32
#define SGFatalErrorIf(cond, msg) if(cond) { __debugbreak(); }
#define SGFatalError(msg) { __debugbreak(); }
#define SGDebugBreakIf(cond, msg) if(cond) { __debugbreak(); }
#define SGDebugBreak(msg) { __debugbreak(); }
#endif