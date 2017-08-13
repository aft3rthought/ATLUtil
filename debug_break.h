#if defined(PLATFORM_OSX) || defined(PLATFORM_IOS) || defined(PLATFORM_WINDOWS)

#include <cstdio>
#include <stdexcept>

#ifndef atl_log_function_impl
#define atl_log_function_impl printf
#endif

#ifndef atl_log_impl
#define atl_log_impl(Message) { atl_log_function_impl("%s, line %d: %s\n", __FILE__, __LINE__, Message); }
#endif

#ifndef atl_fatal_impl
#define atl_fatal_impl(Message) { atl_log_function_impl("%s, line %d: %s\n", __FILE__, __LINE__, Message); exit(0); }
#endif

#endif

#define atl_debug_log(Message) { atl_log_impl(Message) }
#define atl_fatal(Message) { atl_fatal_impl(Message) }
#define atl_fatal_if(Condition, Message) { if(Condition) atl_fatal(Message) }
#define atl_fatal_assert(Condition, Message) { if(!(Condition)) atl_fatal(Message) }
