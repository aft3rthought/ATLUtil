

#pragma once

#include <vector>
#include <algorithm>

#define atl_begin_end(container) container.begin(), container.end()

namespace atl
{
    
    template <typename VectorType, typename IteratorType, typename T>
    struct base_vector_search_result
    {
    public:
        base_vector_search_result(VectorType & vector, IteratorType itr)
        :
        internal_itr(itr),
        internal_vector(vector)
        {}
        
        T& at () {
            return *internal_itr;
        }
        
        IteratorType itr() {
            return internal_itr;
        }
        
        operator bool () const {
            return internal_itr != internal_vector.end();
        }
        
    private:
        IteratorType internal_itr;
        VectorType & internal_vector;
    };
    
    template <typename T>
    struct vector_search_result : public base_vector_search_result<std::vector<T>, typename std::vector<T>::iterator, T>
    {
        vector_search_result(std::vector<T> & vector, typename std::vector<T>::iterator itr) : base_vector_search_result<std::vector<T>, typename std::vector<T>::iterator, T>(vector, itr) {};
    };

    template <typename T>
    struct const_vector_search_result : public base_vector_search_result<const std::vector<T>, typename std::vector<T>::const_iterator, const T>
    {
        const_vector_search_result(const std::vector<T> & vector, typename std::vector<T>::const_iterator itr) : base_vector_search_result<const std::vector<T>, typename std::vector<T>::const_iterator, const T>(vector, itr) {};
    };
    
    template <typename T, typename Comparator>
    vector_search_result<T> vector_search(std::vector<T> & vector, const Comparator & matching)
    {
        auto itr = vector.begin();
        while(itr != vector.end())
        {
            if(*itr == matching)
                break;
            itr++;
        }
        return {vector, itr};
    }

    template <typename T, typename Predicate>
    vector_search_result<T> vector_search_if(std::vector<T> & vector, Predicate predicate)
    {
        auto itr = std::find_if(vector.begin(), vector.end(), predicate);
        return {vector, itr};
    }
    
    template <typename T, typename Comparator>
    const_vector_search_result<T> vector_search(const std::vector<T> & vector, const Comparator & matching)
    {
        auto itr = vector.begin();
        while(itr != vector.end())
        {
            if(*itr == matching)
                break;
            itr++;
        }
        return {vector, itr};
    }
    
    template <typename T, typename Predicate>
    const_vector_search_result<T> vector_search_if(const std::vector<T> & vector, Predicate predicate)
    {
        auto itr = std::find_if(vector.begin(), vector.end(), predicate);
        return {vector, itr};
    }
    
    /*
     * vector_contains: Return true if value is in vector. Passes value by value, and compares using ==, so ideal for numeric types.
     */
    template <typename T>
    bool vector_contains(const std::vector<T> & vector, const T & value)
    {
        return std::find(vector.begin(), vector.end(), value) != vector.end();
    }
    
    /*
     vector_erase_all_matching: Call erase on all iterators which pass a condition. Returns true if the array was modified.
     */
    template <typename T, typename Function>
    bool vector_erase_all_matching(std::vector<T> & vector, Function function)
    {
        bool result = false;
        auto itr = vector.begin();
        while(itr != vector.end())
        {
            if(function(*itr))
            {
                itr = vector.erase(itr);
                result = true;
            }
            else
            {
                itr++;
            }
        }
        return result;
    }
    
    /*
     vector_erase_all_equal_to: Call erase on all iterators where == returns true. Returns true if the array was modified.
     */
    template <typename T, typename Comparator, typename Function>
    bool vector_erase_all_matching(std::vector<T> & vector, Comparator comparator)
    {
        bool result = false;
        auto itr = vector.begin();
        while(itr != vector.end())
        {
            if(*itr == comparator)
            {
                itr = vector.erase(itr);
                result = true;
            }
            else
            {
                itr++;
            }
        }
        return result;
    }
    
    /*
     for_single_value_matching: Call "function" on the first element in "vector" matching "value". Returns true if any object was matched.
     */
    template <typename T, typename Comparator, typename Function>
    bool for_single_element_matching(std::vector<T> & vector, Comparator comparator, Function function)
    {
        for(auto & element : vector)
        {
            if(element == comparator)
            {
                function(element);
                return true;
            }
        }
        return false;
    }
    
    /*
     const_for_single_element_matching: Const version of for_single_value_matching. Call "function" on the first element in "vector" matching "value". Returns true if any object was matched.
     */
    template <typename T, typename Comparator, typename Function>
    bool const_for_single_element_matching(const std::vector<T> & vector, Comparator comparator, Function function)
    {
        for(auto & element : vector)
        {
            if(element == comparator)
            {
                function(element);
                return true;
            }
        }
        return false;
    }
}
