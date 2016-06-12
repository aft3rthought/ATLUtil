// ATLUtil, Copyright © 2016 Maxwell Robinson. All rights reserved.

#pragma once

#include "ATLUtil/debug_break.h"
#include <initializer_list>

namespace atl
{
    template <typename element_type, size_t num_elements>
    struct fixed_vector
    {
    public:
        using iterator = element_type*;
        using const_iterator = const element_type*;
        
        fixed_vector() noexcept
        :
        internal_count(0)
        {}
        
        template <class input_iterator>
        fixed_vector(input_iterator in_first, input_iterator in_last)
        :
        fixed_vector()
        {
            insert(in_first, in_last);
        }
        
        fixed_vector(const fixed_vector & in_copy_array) noexcept
        :
        internal_count(in_copy_array.internal_count)
        {
            std::copy(std::begin(in_copy_array.internal_storage), std::end(in_copy_array.internal_storage), std::begin(internal_storage));
        }
        
        fixed_vector(fixed_vector && in_move_array) noexcept
        :
        internal_count(in_move_array.internal_count)
        {
            std::copy(std::begin(in_move_array.internal_storage), std::end(in_move_array.internal_storage), std::begin(internal_storage));
        }

        fixed_vector(std::initializer_list<element_type> in_initializer_list)
        :
        fixed_vector(in_initializer_list.begin(), in_initializer_list.end())
        {}
        
        template <typename ...emplace_arguments_list>
        void emplace_back(emplace_arguments_list... in_arguments)
        {
            atl_fatal_assert(internal_count < num_elements, "emplace_back called when space is full");
            new (end()) element_type(std::forward<emplace_arguments_list>(in_arguments)...);
            ++internal_count;
        }
        
        void pop_back()
        {
            atl_fatal_assert(internal_count > 0, "pop_back called on empty array");
            end()->~element_type();
            --internal_count;
        }
        
        void clear() noexcept
        {
            for(auto & l_entry : *this)
                l_entry.~element_type();
            internal_count = 0;
        }
        
        size_t count() const noexcept
        {
            return internal_count;
        }
        
        bool empty() const noexcept
        {
            return internal_count == 0;
        }
        
        template <typename iterator_type>
        ssize_t insert(const iterator_type & in_input_begin, const iterator_type & in_input_end)
        {
            auto l_current_end = begin() + internal_count;
            auto l_internal_max_end = internal_max_end();
            auto l_input = in_input_begin;
            while(l_input != in_input_end)
            {
                atl_fatal_assert(l_current_end != l_internal_max_end, "insert contained too many elements for storage");
                new (end()) element_type(*l_input);
                ++internal_count;
                l_current_end++;
                l_input++;
            }
            return (in_input_end - in_input_begin) - (l_input - in_input_begin);
        }
        
        iterator erase(iterator in_element_to_remove)
        {
            auto l_end = end();
            atl_fatal_assert(in_element_to_remove >= begin() && in_element_to_remove < l_end, "erase iterator out of bounds");
            auto l_next = in_element_to_remove + 1;
            in_element_to_remove->~element_type();
            internal_count--;
            std::move(l_next, l_end, in_element_to_remove);
            return iterator(in_element_to_remove);
        }
        
        iterator erase_fast(iterator in_element_to_remove)
        {
            auto l_end = end();
            atl_fatal_assert(in_element_to_remove >= begin() && in_element_to_remove < l_end, "erase_fast iterator out of bounds");
            auto l_last = l_end - 1;
            in_element_to_remove->~element_type();
            internal_count--;
            if(l_last != in_element_to_remove)
                *in_element_to_remove = *l_last;
            return iterator(in_element_to_remove);
        }
        
        iterator begin() noexcept { return reinterpret_cast<element_type*>(internal_storage); }
        iterator end() noexcept { return begin() + internal_count; }
        
        const_iterator begin() const noexcept { return reinterpret_cast<const element_type*>(internal_storage); }
        const_iterator end() const noexcept { return begin() + internal_count; }

    private:
        element_type const * internal_max_end() const { return begin() + num_elements; }
        const static size_t internal_element_size = sizeof(element_type);
        const static size_t internal_num_storage_bytes = internal_element_size * num_elements;

        size_t internal_count;
        uint8_t internal_storage[internal_num_storage_bytes];
    };
}
