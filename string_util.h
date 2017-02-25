

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
        auto sepEnd = in_separator.end();
        bool matchingSeparator = false;
        for(auto stringItr = in_string.begin(); stringItr != in_string.end(); stringItr++)
        {
            if(*stringItr == *sepItr)
            {
                if(!matchingSeparator)
                    matchingSeparator = true;
                sepItr++;
                if(sepItr == sepEnd)
                {
                    matchingSeparator = false;
                    sepItr = in_separator.begin();

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

    template <typename integer_type>
    constexpr size_t log10(integer_type number) { return number > 10 ? 1 + log10(number / 10) : 0; }
    
    template <typename integer_type>
    constexpr size_t max_digits_for_printing_integer_type() { return (std::is_signed<integer_type>::value ? 1 : 0) + log10(std::numeric_limits<integer_type>::max()) + 1; }
    
    namespace detail
    {
        template <typename integer_type>
        char * print_sign(integer_type, char * buffer_ptr, typename std::enable_if<std::is_unsigned<integer_type>::value>::type* = 0)
        {
            return buffer_ptr;
        }
        
        template <typename integer_type>
        char * print_sign(integer_type number, char * buffer_ptr, typename std::enable_if<std::is_signed<integer_type>::value>::type* = 0)
        {
            if(number < 0)
            {
                *buffer_ptr = '-';
                buffer_ptr++;
            }
            return buffer_ptr;
        }
    }
    
#warning TODO: support size_t digits before zero, a decimal character, and a flag for always show below zero/only show if non-zero
    template <typename integer_type, size_t separator_spacing, char separator_character>
    struct printed_integer
    {
        static constexpr size_t digits_for_type_alone() { return max_digits_for_printing_integer_type<integer_type>(); }
        char buffer[digits_for_type_alone() + ((separator_spacing > 0) ? (digits_for_type_alone() / separator_spacing) : 0) + 1];
        
        printed_integer(integer_type number)
        {
            print(number);
        }
        
        void print(integer_type in_number)
        {
            char * buffer_ptr = buffer;
            buffer_ptr = detail::print_sign(in_number, buffer_ptr);
            auto stop = buffer_ptr;
            size_t digits_used = 0;
            {
                auto digits_counter = in_number;
                do
                {
                    digits_counter /= 10;
                    digits_used++;
                } while(digits_counter != 0);
            }
            digits_used = (separator_spacing > 0) ? (digits_used + (digits_used - 1) / separator_spacing) : digits_used;
            buffer_ptr += digits_used;
            *buffer_ptr = 0;
            size_t comma_counter = separator_spacing;
            do
            {
                buffer_ptr--;
                if(separator_spacing > 0)
                {
                    if(comma_counter == 0)
                    {
                        comma_counter = separator_spacing;
                        *buffer_ptr = separator_character;
                        continue;
                    }
                    comma_counter--;
                }
                const static char digit_table[] = { '9', '8', '7', '6', '5', '4', '3', '2', '1', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
                *buffer_ptr = digit_table[(in_number % 10) + 9];
                in_number /= 10;
            } while(buffer_ptr != stop);
        }
    };
    
    template <char separator_character, size_t separator_spacing>
    struct print_integer_with_separator
    {
        template <typename integer_type>
        printed_integer<integer_type, separator_spacing, separator_character> operator()(integer_type number) { return printed_integer<integer_type, separator_spacing, separator_character>(number); }
    };
    
    template <typename integer_type>
    printed_integer<integer_type, 0, ','> print_integer(integer_type number) { return printed_integer<integer_type, 0, ','>(number); }
    
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
