

#pragma once

#include <random>

namespace atl
{
    class rand
    {
    public:
        rand() :
        generator(device())
        {}

        int32_t rand_int(int32_t min, int32_t max)
        {
            return std::uniform_int_distribution<int32_t>(min, max)(generator);
        }

        uint32_t rand_uint(uint32_t min, uint32_t max)
        {
            return std::uniform_int_distribution<uint32_t>(min, max)(generator);
        }

        float rand_float(float min, float max)
        {
            return std::uniform_real_distribution<float>(min, max)(generator);
        }

        double rand_double(double min, double max)
        {
            return std::uniform_real_distribution<double>(min, max)(generator);
        }
        
        std::random_device device;
        std::mt19937 generator;
    };
}
