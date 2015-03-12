

#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace atl
{
    std::vector<uint8_t> read_bytes(const std::string & filename)
    {
        std::vector<uint8_t> result;
        std::ifstream file(filename.c_str(), std::ios::in);
        
        if(file.good())
        {
            std::streamsize size = 0;
            if(file.seekg(0, std::ios::end).good())
                size = file.tellg();
            
            if(file.seekg(0, std::ios::beg).good())
                size -= file.tellg();
            
            result.resize(size);
            file.read((char *)result.data(), size);
        }
        return result;
    }
}
