#ifdef PLATFORM_OSX
#include <cstdio>
#define atl_break_debug(__Message__) { printf("%s\n", __Message__); __builtin_trap(); }
#define atl_break_fatal(__Message__) atl_break_debug(__Message__)
#endif

#ifdef PLATFORM_IOS
#include <cstdio>
#define atl_break_debug(__Message__) { printf("%s\n", __Message__); __builtin_trap(); }
#define atl_break_fatal(__Message__) atl_break_debug(__Message__)
#endif

#ifdef PLATFORM_WINDOWS
#include <cstdio>
#define atl_break_debug(__Message__) { printf("%s\n", __Message__); __debugbreak(); }
#define atl_break_fatal(__Message__) atl_break_debug(__Message__)
#endif

#define atl_break_if_debug(__Condition__, __Message__) if(__Condition__) atl_break_debug(__Message__)
#define atl_break_if_fatal(__Condition__, __Message__) if(__Condition__) atl_break_fatal(__Message__)
#define atl_assert_debug(__Condition__, __Message__) if(!(__Condition__)) atl_break_debug(__Message__)
#define atl_assert_fatal(__Condition__, __Message__) if(!(__Condition__)) atl_break_fatal(__Message__)

// TODO: Legacy, cleanup
#define SGFatalErrorIf(__Condition__, __Message__) atl_break_if_fatal(__Condition__, __Message__)
#define SGFatalError(__Message__) atl_break_fatal(__Message__)
#define SGDebugBreakIf(__Condition__, __Message__) atl_break_if_debug(__Condition__, __Message__)
#define SGDebugBreak(__Message__) atl_break_debug(__Message__)
#define ATLAssertFatal(__Condition__, __Message__) atl_break_if_fatal(__Condition__, __Message__)
#define ATLAssertWarn(__Condition__, __Message__) atl_break_if_debug(__Condition__, __Message__)
