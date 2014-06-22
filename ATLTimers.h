

#pragma once

#include "ATLArrayUtil.h"
#include "ATLBasicMath.h"
#include <cmath>

namespace atl
{
    class timer
    {
    private:
        float _value;
        
    public:
        timer() :
        _value(1.f)
        {}
        
        void begin() { _value = 0.f; }
        void reset() { _value = 1.f; }
        bool active() const { return _value < 1.f; }
        bool done() const { return _value >= 1.f; }
        
        void advance(float in_amt)
        {
            _value += in_amt;
            if(_value > 1.f)
                _value = 1.f;
        }
        
        void tick(float in_amt, float in_duration)
        {
            advance(in_amt * 1.f / in_duration);
        }
        
        void advance_and_loop(float in_amt)
        {
            _value += in_amt;
            _value -= floorf(_value);
        }
        
        void tick_and_loop(float in_amt, float in_duration)
        {
            advance_and_loop(in_amt * 1.f / in_duration);
        }
        
        float map(float in_valAtBegin, float in_valAtEnd) const
        {
            return mapf_unclamped(_value, 0.f, 1.f, in_valAtBegin, in_valAtEnd);
        }
        
        float map_range(float in_begin, float in_end, float in_valAtBegin, float in_valAtEnd) const
        {
            return mapf(_value, in_begin, in_end, in_valAtBegin, in_valAtEnd);
        }
        
        float map_sub(float in_begin, float in_end) const
        {
            return mapf(_value, in_begin, in_end, 0.f, 1.f);
        }
        
        float timer_value() const
        {
            return _value;
        }
        
        float map_step(float in_about, float in_ifBelow, float in_ifAbove) const
        {
            return atl::map_step(_value, in_about, in_ifBelow, in_ifAbove);
        }
        
        bool is_past(float in_value) {
            return _value > in_value;
        }
        
        int to_int(int in_frames) const
        {
            return active() ? _value * in_frames : 0;
        }
        
        template <typename T, size_t N>
        T & pick(T(&in_array)[N]) const
        {
            return in_array[to_int(c_array_len(in_array))];
        }
    };
    
    class cyclic_timer
    {
    private:
        float _value;
        int _count;
        
    public:
        cyclic_timer() :
        _value(1.f),
        _count(-1)
        {}
        
        void reset()
        {
            _value = 1.f;
            _count = -1;
        }
        
        bool has_run() const
        {
            return _count > -1;
        }
        
        bool is_on_run(int in_run) const
        {
            return _count == in_run;
        }
        
        bool is_past_run(int in_run) const
        {
            return _count > in_run;
        }
        
        void advance(float in_amt)
        {
            _value += in_amt;
            while(_value >= 1.f)
            {
                _value -= 1.f;
                _count++;
            }
        }
        
        void tick(float in_amt, float in_duration)
        {
            advance(in_amt * (1.f / in_duration));
        }
        
        float map_for_run(int in_run, float in_valAtBegin, float in_valAtEnd) const
        {
            if(_count < in_run)
                return in_valAtBegin;
            if(_count > in_run)
                return in_valAtEnd;
            return atl::mapf_unclamped(_value, 0.f, 1.f, in_valAtBegin, in_valAtEnd);
        }
        
        float map_any_run(float in_valAtBegin, float in_valAtEnd) const
        {
            return mapf_unclamped(_value, 0.f, 1.f, in_valAtBegin, in_valAtEnd);
        }
        
        float timer_value() const
        {
            return _value;
        }
        
        int to_int(int in_frames) const
        {
            return has_run() ? _value * in_frames : 0;
        }
        
        template <typename T, size_t N>
        T & pick(T(&in_array)[N]) const
        {
            return in_array[to_int(c_array_len(in_array))];
        }
    };
}