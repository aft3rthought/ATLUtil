

#pragma once

#include <vector>
#include <string>

namespace atl
{
    std::vector<std::string> split_string(const std::string & in_string, const std::string & in_separator)
    {
        std::vector<std::string> result;
        
        auto chunkBeginItr = in_string.begin();
        auto chunkEndItr = in_string.begin();
        auto sepItr = in_separator.begin();
        bool matchingSeparator = false;
        for(auto stringItr = in_string.begin(); stringItr != in_string.end(); stringItr++)
        {
            if(*stringItr == *sepItr)
            {
                if(!matchingSeparator)
                    matchingSeparator = true;
                sepItr++;
                if(sepItr == in_separator.end())
                {
                    if(chunkBeginItr != chunkEndItr)
                        result.emplace_back(chunkBeginItr, chunkEndItr);
                    
                    chunkBeginItr = chunkEndItr = (stringItr+1);
                }
            }
            else
            {
                matchingSeparator = false;
                sepItr = in_separator.begin();
                chunkEndItr = stringItr+1;
            }
        }
        
        if(chunkBeginItr != chunkEndItr)
            result.emplace_back(chunkBeginItr, chunkEndItr);
        
        return result;
    }
    
    std::string trim_string(const std::string & in_string)
    {
        auto chunkBeginItr = in_string.end();
        auto chunkEndItr = in_string.end();
        for(auto stringItr = in_string.begin(); stringItr != in_string.end(); stringItr++)
        {
            char val = *stringItr;
            if((val == ' ' || val == '\t' || val == '\n') == false)
            {
                if(chunkBeginItr == in_string.end())
                    chunkBeginItr = stringItr;
                chunkEndItr = stringItr+1;
            }
        }
        return std::string(chunkBeginItr, chunkEndItr);
    }
    
    std::string num_string(int32_t number)
    {
        char buff[16];
        snprintf(buff, sizeof(buff), "%d", number);
        return buff;
    }
}