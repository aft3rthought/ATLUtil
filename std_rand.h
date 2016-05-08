

#pragma once

#include <random>
#include <vector>

namespace atl
{
    class rand
    {
    public:
        rand() :
        generator(device())
        {}
        
        size_t rand_size_t(size_t min, size_t max)
        {
            return std::uniform_int_distribution<size_t>(min, max)(generator);
        }

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
        
        template <typename T, size_t N>
        const T & rand_select(const T(&c_array)[N])
        {
            return c_array[rand_int(0,N-1)];
        }
        
        template <typename T>
        const T & rand_select(const std::vector<T> & vector)
        {
            return vector.at(rand_size_t(0, vector.size() - 1));
        }
        
        template <typename T>
        const T & rand_select(std::initializer_list<T> list)
        {
            return *(list.begin() + rand_size_t(0, list.size() - 1));
        }
        
        std::random_device device;
        std::mt19937 generator;
    };
    
    class pseudo_rand
    {
    public:
        pseudo_rand(std::mt19937::result_type seed) :
        generator(seed)
        {}

        pseudo_rand(std::seed_seq & seed) :
        generator(seed)
        {}
        
        size_t rand_size_t(size_t min, size_t max)
        {
            return std::uniform_int_distribution<size_t>(min, max)(generator);
        }
        
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
        
        template <typename T, size_t N>
        const T & rand_select(const T(&c_array)[N])
        {
            return c_array[rand_int(0,N-1)];
        }
        
        template <typename T>
        const T & rand_select(const std::vector<T> & vector)
        {
            return vector.at(rand_size_t(0, vector.size() - 1));
        }
        
        template <typename T>
        const T & rand_select(std::initializer_list<T> list)
        {
            return *(list.begin() + rand_size_t(0, list.size() - 1));
        }
        
        std::mt19937 generator;
    };
}
