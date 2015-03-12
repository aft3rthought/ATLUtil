

#pragma once

#include <vector>
#include <string>

namespace atl
{
    inline std::vector<std::string> split_string(const std::string & in_string, const std::string & in_separator)
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
    
    inline bool is_whitespace(char c)
    {
        return (c == ' ' || c == '\t' || c == '\n');
    }
    
    inline std::string trim_quotes(const std::string & string)
    {
        auto stringSize = string.size();
        char firstChar = string[0];
        char lastChar = string[stringSize - 1];
        if((firstChar == '\"' && lastChar == '\"') ||
           (firstChar == '\'' && lastChar == '\''))
        {
            return string.substr(1, stringSize - 2);
        }
        return string;
    }

    inline std::string trim_whitespace(const std::string & in_string)
    {
        auto chunkBeginItr = in_string.end();
        auto chunkEndItr = in_string.end();
        for(auto stringItr = in_string.begin(); stringItr != in_string.end(); stringItr++)
        {
            if(!is_whitespace(*stringItr))
            {
                if(chunkBeginItr == in_string.end())
                    chunkBeginItr = stringItr;
                chunkEndItr = stringItr+1;
            }
        }
        return std::string(chunkBeginItr, chunkEndItr);
    }
    
    inline std::string num_string(int32_t number)
    {
        char buff[16];
        snprintf(buff, sizeof(buff), "%d", number);
        return buff;
    }
    
    inline std::string get_directory(const std::string & string)
    {
        auto last_slash_pos = string.find_last_of('/');
        if(last_slash_pos != std::string::npos)
            return string.substr(0, last_slash_pos + 1);
        return "";
    }
    
    inline std::string get_file_name(const std::string & string, bool includeExtension)
    {
        auto last_slash_pos = string.find_last_of('/');
        std::string filename = last_slash_pos != std::string::npos ? string.substr(last_slash_pos + 1, std::string::npos) : string;
        if(!includeExtension)
        {
            auto first_period_pos = filename.find_first_of('.');
            filename = first_period_pos != std::string::npos ? filename.substr(0, first_period_pos) : filename;
        }
        return filename;
    }
}