

#pragma once

#include "file.h"
#include "map_util.h"
#include "numeric_casts.h"
#include "string_util.h"
#include <map>
#include <string>

namespace atl
{
    class tool_args
    {
    public:
        tool_args(int inArgc, const char * inArgv[])
        {
            args["DEFAULT_CWD"] = {atl::get_directory(inArgv[0])};
            for(int l_argIdx = 1; l_argIdx < inArgc; l_argIdx++)
                parse_arg(inArgv[l_argIdx], "");
        }
        
        bool has_arg(const std::string & inArg) const
        {
            return atl::map_has(args, inArg);
        }
        
        bool parse_arg(const std::string & inArg, const std::string & inCwd)
        {
            const std::string lfileToken("file:");
            auto lFilePos = inArg.find(lfileToken);
            if(lFilePos == 0)
            {
                auto lFileNameBegin = lFilePos + lfileToken.size();
                auto lFileName = inArg.substr(lFileNameBegin, inArg.size() - lFileNameBegin);
                lFileName = atl::trim_quotes(atl::trim_whitespace(lFileName));
                lFileName = inCwd + lFileName;
                load_config_file(lFileName);
                return true;
            }

            auto lEqPos = inArg.find_first_of('=');
            if(lEqPos != inArg.npos && lEqPos > 0)
            {
                auto lKeyLength = lEqPos;
                bool lAdd = false;
                if(inArg[lEqPos-1] == '+')
                {
                    lAdd = true;
                    lKeyLength--;
                }
                auto lKey = inArg.substr(0, lKeyLength);
                auto lVal = inArg.substr(lEqPos + 1, inArg.size());
                add_arg(lKey, lVal, lAdd);
                return true;
            }
            return false;
        }
        
        void load_config_file(const std::string & inFilename)
        {
            auto data = atl::read_bytes(inFilename);
            if(data.empty())
            {
                printf("Couldn't load file: %s\n", inFilename.c_str());
            }
            else
            {
                printf("Loading file: %s\n", inFilename.c_str());
                parse_config_file(data, atl::get_directory(inFilename));
            }
        }
        
        void parse_config_file(const std::vector<uint8_t> & inFile, const std::string & inFileDirectory)
        {
            auto lItr = inFile.begin();
            auto lLineBegin = lItr;
            while(lItr != inFile.end())
            {
                bool lNewline = false;
                while(lItr != inFile.end())
                {
                    char lChar = *lItr++;
                    if(lChar == '\n')
                    {
                        lNewline = true;
                        break;
                    }
                }
                if(lItr > lLineBegin)
                {
                    const char * lArgPtr = (const char *)inFile.data() + (lLineBegin - inFile.begin());
                    parse_arg(std::string(lArgPtr, lItr - lLineBegin - (lNewline ? 1 : 0)), inFileDirectory);
                }
                lLineBegin = lItr;
            }
        }
        
        void add_arg(const std::string & inArg, const std::string & inValue, bool inAddToExisting)
        {
            if(!inAddToExisting || !has_arg(inArg))
                args[inArg] = {inValue};
            else
                args.at(inArg).emplace_back(inValue);
        }
        
        const std::vector<std::string> & get_arg_all(const std::string & inArg) const
        {
            if(has_arg(inArg))
                return args.at(inArg);
            throw std::out_of_range("get_arg_all called with an arg not in the set");
        }
        
        std::string get_arg(const std::string & inArg, uint32_t inIdx = 0) const
        {
            if(has_arg(inArg))
            {
                const auto & lArgList = args.at(inArg);
                if(inIdx < lArgList.size())
                {
                    return lArgList[inIdx];
                }
            }
            return "";
        }
        
        std::map<std::string, std::vector<std::string> > args;
        std::vector<std::string> errors;
    };
}
