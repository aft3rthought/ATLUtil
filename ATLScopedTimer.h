

#pragma once

#include <chrono>
#include <string>

namespace atl
{
    class scoped_timer
    {
    public:
        scoped_timer(std::string && in_debugStr);
        ~scoped_timer();
        
    private:
        std::string _debugStr;
        std::chrono::high_resolution_clock::time_point _startTime;
    };
}