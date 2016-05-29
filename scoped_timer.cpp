

#include "scoped_timer.h"

namespace atl
{
    scoped_timer::scoped_timer(std::string && in_debugStr) :
    _debugStr(in_debugStr),
    _startTime(std::chrono::high_resolution_clock().now())
    {}
    
    scoped_timer::~scoped_timer()
    {
        auto l_duration = std::chrono::high_resolution_clock().now() - _startTime;
        
        auto l_durationNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(l_duration).count();
        
        auto l_durationSeconds = std::chrono::duration_cast<std::chrono::duration<double>>(l_duration).count();
        
        printf("Timer_%s: Took %f seconds (%lli ns)\n", _debugStr.c_str(), l_durationSeconds, l_durationNanoseconds);
    }
}
