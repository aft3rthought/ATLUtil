

#pragma once

#include "string_util.h"

namespace atl
{
    class json_tree_node
    {
    public:
        json_tree_node(const std::string & in_text) :
        internal_is_valid(false)
        {
            auto l_headItr = in_text.begin();
            auto l_tailItr = in_text.rbegin();
            while(l_headItr != in_text.end() && atl::is_whitespace(*l_headItr)) { l_headItr++; }
            while(l_tailItr != in_text.rend() && atl::is_whitespace(*l_tailItr)) { l_tailItr++; }
            
            if(l_headItr != in_text.end() && l_tailItr != in_text.rend())
            {
                char l_firstChar = *l_headItr;
                char l_lastChar = *l_tailItr;
                if(l_firstChar == '{' && l_lastChar == '}')
                {
                    l_tailPtr--;
                    l_headPtr++;
                    
                    // parse list of k:v pairs
                }
                else if(l_firstChar == '[' && l_lastChar == ']')
                {
                    l_tailPtr--;
                    l_headPtr++;
                    
                    // parse comma separated list
                }
            }
        }
        
    private:
        bool internal_is_valid;
        
        struct internal_parse_node
        {
            enum class Token {
                
            };
        };
    };
    
    class json_leaf_node
    {
    public:
        json_leaf_node(const std::string & in_text)
        {
            // 
        }
        
    private:
        bool internal_checked_string;
        bool internal_checked_numeric;
        bool internal_checked_boolean;
        
        bool internal_valid_string;
        bool internal_valid_numeric;
        bool internal_valid_boolean;
    };
}