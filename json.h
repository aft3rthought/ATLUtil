

#pragma once

#include "file.h"
#include "serialize.h"
#include "map_util.h"
#include "numeric_casts.h"
#include <map>

namespace atl
{
    class command_line_parameters
    {
    public:
        command_line_parameters(int in_argc, const char * in_argv[], const std::vector<std::string> & in_expectedConfigs)
        {
            std::map<std::string, std::string> args;
            {
                std::string arg0(in_argv[0]);
                args["DEFAULT_CWD"] = arg0.substr(0, arg0.find_last_of('/')+1);
            }
            for(int argIdx = 1; argIdx < in_argc; argIdx++)
                gParseArg(in_argv[argIdx], &args);
            
            if(atl::map_has(args, {"CONFIG"}))
            {
                std::string configFile = args.at("CONFIG");
                std::string directory, file, outputFile;
                auto data = atl::read_bytes(configFile);
                if(data.empty())
                {
                    printf("Couldn't load CONFIG file: %s\n", configFile.c_str());
                }
                else
                {
                    args["CONFIG_DIRECTORY"] = configFile.substr(0, configFile.find_last_of('/')+1);
                    gParseConfig(data, &args);
                }
            }
            if(atl::map_has(args, {"OP"}))
            {
                
            }
            else
            {
                printf("Missing required parameter \"OP\"\n");
            }
        }
        
        bool parse_arg(const std::string & arg)
        {
            auto firstItr = arg.find_first_of('=');
            if(firstItr != arg.npos)
            {
                auto key = arg.substr(0, firstItr);
                auto val = arg.substr(firstItr + 1, arg.size());
                (*parsed)[key] = val;
                return true;
            }
            return false;
        }
        
        void parse_config_file(const std::vector<uint8_t> & file, std::map<std::string, std::string> * parsed)
        {
            bool readingKey = true;
            std::string nextKey;
            std::string nextValue;
            atl::bits_in bytes(file.data(), atl::narrow_size_t_to_int(file.size()));
            while(bytes.has_bytes())
            {
                char next = bytes.read_byte();
                if(next == '=')
                {
                    readingKey = false;
                }
                else if(next == '\n')
                {
                    if(!nextKey.empty() && !nextValue.empty())
                        (*parsed)[nextKey] = nextValue;
                    
                    readingKey = true;
                    nextKey = "";
                    nextValue = "";
                }
                else
                {
                    if(readingKey)
                        nextKey += next;
                    else
                        nextValue += next;
                }
            }
        }
        
        std::map<std::string, std::string> parameters;
        std::vector<std::string> errors;
    };
}