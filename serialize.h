

#pragma once

#include <vector>
#include <fstream>
#include <string>

namespace atl
{
    class bits_common
    {
    public:
        uint8_t bits_required_for_uint32(uint32_t in_numValuesInRange)
        {
            uint8_t bitsRequired = 0;
            while(in_numValuesInRange > 0)
            {
                in_numValuesInRange = in_numValuesInRange >> 1;
                bitsRequired++;
            }
            return bitsRequired;
        }
        
        uint8_t bits_required_for_uint64(uint64_t in_numValuesInRange)
        {
            uint8_t bitsRequired = 0;
            while(in_numValuesInRange > 0)
            {
                in_numValuesInRange = in_numValuesInRange >> 1;
                bitsRequired++;
            }
            return bitsRequired;
        }
    };
    
    class bits_in : public bits_common
    {
    public:
        bits_in(const unsigned char * in_data, int in_length) : bits_in() { attach_bytes(in_data, in_length); }
        bits_in() : _dataPointer(nullptr), _bytesRemaining(0), _bitOffset(0) {}
        
        void attach_bytes(const unsigned char * in_data, int in_length)
        {
            _bitOffset = 0;
            _dataPointer = in_data;
            _bytesRemaining = in_length;
        }
        
        bool has_bytes() const { return _bytesRemaining > 0; }
        
        void read_bits(unsigned char * in_data, int32_t in_numBits)
        {
            int32_t maxBitsRemaining = _bytesRemaining * 8 - _bitOffset;
            if(in_numBits > maxBitsRemaining)
                throw std::logic_error("out of bounds read");
            
            int32_t bitsRead = 0;
            while(bitsRead < in_numBits)
            {
                int32_t offsetInRead = (bitsRead % 8);
                
                if(offsetInRead == 0)
                    *in_data = 0;
                
                int32_t bitsRemaining = in_numBits - bitsRead;
                int32_t bitsToNextDataByte = std::min(8 - offsetInRead, bitsRemaining);
                int32_t bitsAvailableInByte = 8 - _bitOffset;
                int32_t bitsToRead = std::min(bitsToNextDataByte, bitsAvailableInByte);
                
                *in_data |= ((*_dataPointer >> _bitOffset) & ((unsigned char)0xFF >> (8 - bitsToRead))) << offsetInRead;
                
                _bitOffset += bitsToRead;
                bitsRead += bitsToRead;
                if(bitsRead % 8 == 0)
                    in_data++;
                
                if(_bitOffset == 8)
                {
                    _dataPointer++;
                    _bytesRemaining--;
                    _bitOffset = 0;
                 }
            }
        }
        
        void read_bytes(unsigned char * in_data, int32_t in_numBytes)
        {
            read_bits(in_data, in_numBytes * 8);
        }

        unsigned char read_byte()
        {
            unsigned char result;
            read_bytes(&result, sizeof(unsigned char));
            return result;
        }
        
        int16_t read_int16()
        {
            int16_t result;
            read_bytes((unsigned char *)&result, sizeof(int16_t));
            return result;
        }
        
        int32_t read_int32()
        {
            int32_t result;
            read_bytes((unsigned char *)&result, sizeof(int32_t));
            return result;
        }
        
        int64_t read_int64()
        {
            int64_t result;
            read_bytes((unsigned char *)&result, sizeof(int64_t));
            return result;
        }
        
        uint16_t read_uint16()
        {
            uint16_t result;
            read_bytes((unsigned char *)&result, sizeof(uint16_t));
            return result;
        }
        
        uint32_t read_uint32()
        {
            uint32_t result;
            read_bytes((unsigned char *)&result, sizeof(uint32_t));
            return result;
        }
        
        uint64_t read_uint64()
        {
            uint64_t result;
            read_bytes((unsigned char *)&result, sizeof(uint64_t));
            return result;
        }
        
        float read_float()
        {
            float result;
            read_bytes((unsigned char *)&result, sizeof(float));
            return result;
        }
        
        double read_double()
        {
            double result;
            read_bytes((unsigned char *)&result, sizeof(double));
            return result;
        }
        
        bool read_bool()
        {
            bool result = false;
            read_bits((unsigned char *)&result, 1);
            return result;
        }
        
        std::string read_string()
        {
            std::string result;
            unsigned char nextByte = read_byte();
            while(nextByte != 0)
            {
                result += nextByte;
                nextByte = read_byte();
            }
            return result;
        }
        
        int32_t read_ranged_int(int32_t in_min, int32_t in_max)
        {
            if(in_min == in_max)
            {
                return in_min;
            }
            else
            {
                if(in_max < in_min)
                    throw std::logic_error("invalid range for int");
                
                uint32_t numValuesInRange = in_max - in_min;
                uint32_t bitsRequired = bits_required_for_uint32(numValuesInRange);
                
                int32_t result = 0;
                read_bits((unsigned char *)&result, bitsRequired);
                return result + in_min;
            }
        }
        
        uint32_t read_uint32_var_bit(uint8_t in_stride)
        {
            uint32_t result = 0;
            uint32_t chunk;
            uint8_t curBit = 0;
            while(read_bool())
            {
                chunk = 0;
                read_bits((unsigned char *)&chunk, in_stride);
                result += chunk << curBit;
                curBit += in_stride;
            }
            return result;
        }
        
        uint64_t read_uint64_var_bit(uint8_t in_stride)
        {
            uint64_t result = 0;
            uint64_t chunk;
            uint8_t curBit = 0;
            while(read_bool())
            {
                chunk = 0;
                read_bits((unsigned char *)&chunk, in_stride);
                result += chunk << curBit;
                curBit += in_stride;
            }
            return result;
        }
        
        template<typename EnumType>
        EnumType read_enum()
        {
            return (EnumType)read_ranged_int((int32_t)EnumType::MinValue, (int32_t)EnumType::MaxValue);
        }
        
        void skip_to_next_byte()
        {
            if(_bitOffset != 0)
            {
                _dataPointer++;
                _bytesRemaining--;
                _bitOffset = 0;
            }
        }
        
        const unsigned char * current_data_pointer() const
        {
            if(_bitOffset != 0)
                throw std::logic_error("getting data pointer when stream is not on a byte boundary");
            return _dataPointer;
        }
        
        void skip_bits(int32_t in_amount)
        {
            auto new_offset = _bitOffset + in_amount;
            int32_t num_bytes = new_offset / 8;
            if(_bytesRemaining < num_bytes)
                throw std::logic_error("out of range read");
            _dataPointer += num_bytes;
            _bitOffset = new_offset % 8;
            
        }
        
        void advance_data_pointer(int32_t in_amount)
        {
            if(_bytesRemaining < in_amount)
                throw std::logic_error("out of range read");
            _dataPointer += in_amount;
            _bytesRemaining -= in_amount;
        }
        
    private:
        const unsigned char * _dataPointer;
        int32_t _bytesRemaining;
        int32_t _bitOffset;
    };
    
    class bits_out : public bits_common
    {
    public:
        bits_out() :
        _bitOffset(8)
        {}
        
        void append_bits(unsigned char * in_data, int32_t in_numBits)
        {
            int32_t bitsWritten = 0;
            while(bitsWritten < in_numBits)
            {
                if(_data.empty() || _bitOffset == 8)
                {
                    _data.emplace_back(0x00);
                    _bitOffset = 0;
                }
                
                int32_t offsetInWrite = (bitsWritten % 8);
                int32_t bitsRemaining = in_numBits - bitsWritten;
                int32_t bitsToNextDataByte = std::min(8 - offsetInWrite, bitsRemaining);
                int32_t bitsAvailableInByte = 8 - _bitOffset;
                int32_t bitsToWrite = std::min(bitsToNextDataByte, bitsAvailableInByte);
                
                _data.back() |= (((*in_data) >> offsetInWrite) << _bitOffset);
                
                _bitOffset += bitsToWrite;
                bitsWritten += bitsToWrite;
                if(bitsWritten % 8 == 0)
                    in_data++;
            }
        }
        
        void append_bytes(unsigned char * in_data, int32_t in_numBytes)
        {
            append_bits(in_data, in_numBytes * 8);
        }
        
        void append_byte(unsigned char in_value)
        {
            append_bytes(&in_value, sizeof(unsigned char));
        }
        
        void append_int16(int16_t in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(int16_t));
        }
        
        void append_int32(int32_t in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(int32_t));
        }
        
        void append_int64(int64_t in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(int64_t));
        }
        
        void append_uint16(uint16_t in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(uint16_t));
        }
        
        void append_uint32(uint32_t in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(uint32_t));
        }
        
        void append_uint64(uint64_t in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(uint64_t));
        }
        
        void append_float(float in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(float));
        }
        
        void append_double(double in_value)
        {
            append_bytes((unsigned char *)&in_value, sizeof(double));
        }
        
        void append_bool(bool in_value)
        {
            append_bits((unsigned char *)&in_value, 1);
        }
        
        void append_string(const std::string & in_string)
        {
            append_bytes((unsigned char *)in_string.data(), (uint32_t)in_string.length() * (uint32_t)sizeof(char));
            append_byte(0);
        }
        
        void append_ranged_int(int32_t in_value, int32_t in_min, int32_t in_max)
        {
            if(in_max != in_min)
            {
                if(in_max < in_min)
                    throw std::logic_error("invalid range for int");
                
                if(in_value < in_min || in_value > in_max)
                    throw std::logic_error("value not in range");
                
                uint32_t bitsRequired = bits_required_for_uint32(in_max - in_min);
                int32_t valueOffsetInRange = in_value - in_min;
                append_bits((unsigned char *)&valueOffsetInRange, bitsRequired);
            }
        }
        
        void append_uint32_var_bit(uint32_t in_value, uint8_t in_stride)
        {
            if(in_stride == 0)
                throw std::logic_error("invalid stride");
            
            if(in_stride >= 32)
                throw std::logic_error("invalid stride for type");
            
            uint32_t mask = std::numeric_limits<uint32_t>::max() >> (32 - in_stride);
            while(in_value > 0)
            {
                uint32_t writeVal = in_value & mask;
                
                append_bool(true);
                append_bits((unsigned char *)&writeVal, in_stride);
                
                in_value = in_value >> in_stride;
            }
            append_bool(false);
        }
        
        void append_uint64_var_bit(uint64_t in_value, uint8_t in_stride)
        {
            if(in_stride == 0)
                throw std::logic_error("invalid stride");
            
            if(in_stride >= 64)
                throw std::logic_error("invalid stride for type");
            
            uint64_t mask = std::numeric_limits<uint64_t>::max() >> (64 - in_stride);
            while(in_value > 0)
            {
                uint64_t writeVal = in_value & mask;
                
                append_bool(true);
                append_bits((unsigned char *)&writeVal, in_stride);
                
                in_value = in_value >> in_stride;
            }
            append_bool(false);
        }
        
        template<typename EnumType>
        void append_enum(EnumType in_value)
        {
            append_ranged_int((int32_t)in_value, (int32_t)EnumType::MinValue, (int32_t)EnumType::MaxValue);
        }
        
        void skip_to_next_byte()
        {
            if(_bitOffset != 0)
            {
                _data.emplace_back(0x00);
                _bitOffset = 0;
            }
        }
        
        void write_to_stream(std::ostream & in_stream) const
        {
            in_stream.write((const char *)_data.data(), _data.size());
        }
        
    private:
        std::vector<unsigned char> _data;
        int32_t _bitOffset;
    };
}
