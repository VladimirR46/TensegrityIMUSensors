#ifndef __PACKAGE_HPP
#define __PACKAGE_HPP

#define SIZE_BUFF 1024

#define SIZE_SEARCH_ARRAY 50

#include <cstdlib>
#include <iostream>
#include "string.h"

//#include <boost/bind.hpp>
//#include <boost/asio.hpp>
//#include <boost/array.hpp>


class Package
{
public:

    boost::recursive_mutex cs;

    Package()
    {
        buff[0] = 0;
        set_item_index = 1;
        get_item_index = 1;
        current_index  = 0;

        for (int i = 0; i < SIZE_SEARCH_ARRAY; i++)
        {
            address_array[i] = 0;
            index_array[i] = 0;
        }
    }
    //-----------------------------------------------------------------------------------
    void SetLength(uint16_t ln)
    {
        boost::recursive_mutex::scoped_lock lk(cs);
        set_item_index = ln;
    }
    //-----------------------------------------------------------------------------------
    uint16_t length()
    {
        boost::recursive_mutex::scoped_lock lk(cs);
        return set_item_index;
    }
    //-----------------------------------------------------------------------------------
    template < typename T >
    uint8_t init(const char *str)
    {
        boost::recursive_mutex::scoped_lock lk(cs);

        buff[0] = buff[0] + 1;
        uint8_t header_size = strlen(str);
        buff[set_item_index] = header_size;
        for (int i = 0; i < header_size; i++) buff[set_item_index + i + 1] = str[i];

        uint8_t value_size = sizeof(T);
        buff[set_item_index + header_size + 1] = value_size;

        uint8_t value_index = set_item_index + header_size + 2;

        for (int i = 0; i < value_size; i++) buff[value_index + i] = 0;

        set_item_index += header_size + value_size + 2;			
        return value_index;
    }
    //-----------------------------------------------------------------------------------
    template < typename T >
    void set(uint8_t handle, T value)
    {
        boost::recursive_mutex::scoped_lock lk(cs);

        uint8_t value_size = sizeof(T);
        for (int i = 0; i < value_size; i++) buff[handle + i] = ((uint8_t*)&value)[i];
    }
    //-----------------------------------------------------------------------------------
    template < typename T >
    void set(const char *str, T& value)
    {
        boost::recursive_mutex::scoped_lock lk(cs);

        uint16_t index = GetIndexData(str);
        if (index != 0)
        {
            uint8_t value_size = sizeof(T);
            for (int i = 0; i < value_size; i++) buff[index+i] = ((uint8_t*)&value)[i];
            return;
        }						

        buff[0] = buff[0] + 1;
        uint8_t header_size = strlen(str);
        buff[set_item_index] = header_size;
        for (int i = 0; i < header_size; i++) buff[set_item_index + i + 1] = str[i];

        uint8_t value_size = sizeof(T);
        buff[set_item_index + header_size + 1] = value_size;
        for (int i = 0; i < value_size; i++) buff[set_item_index + header_size + i + 2] = ((uint8_t*)&value)[i];

        set_item_index += header_size + value_size + 2;
    }
    
    //-----------------------------------------------------------------------------------
    uint16_t GetIndexData(const char *str)
    {
        //boost::recursive_mutex::scoped_lock lk(cs);

        uint8_t find_header_size = strlen(str);
        
        // ×òåíèå 
        for (int i = 0; i < buff[0]; i++)
        {
            uint8_t header_size = buff[get_item_index];
            uint8_t value_size = buff[get_item_index + header_size + 1];

            if (header_size == find_header_size)
            {
                // Ñðàâíèâàåì
                bool is = false;
                for (int h = 0; h < header_size; h++)
                {
                    if (buff[get_item_index + h + 1] == str[h]) is = true;
                    else 
                    {	
                        is = false;
                        break;
                    }
                }

                if (is)
                {
                    uint16_t ind = get_item_index + header_size + 2;
                    get_item_index = 1;
                    return ind;
                }
            }

            get_item_index += header_size + value_size + 2;
        }
        get_item_index = 1;
        return 0;
    }
    //-----------------------------------------------------------------------------------
    template < typename T >
    T getValue(uint16_t index)
    {
        //boost::recursive_mutex::scoped_lock lk(cs);

        uint8_t value_size = buff[index - 1];
        uint8_t read_buffer[8];
        for (int j = 0; j < value_size; j++) read_buffer[j] = buff[index + j];
        return *(T*)read_buffer;
    }
    //-----------------------------------------------------------------------------------
    template < typename T >
    T get(const char *str, T default_)
    {
       boost::recursive_mutex::scoped_lock lk(cs);
       
        for (int i = 0; i < buff[0]; i++)
        {
            if (address_array[i] == str)
            {
                return getValue<T>(index_array[i]);
            }
        }
                    
        uint16_t index = GetIndexData(str);
        if (index != 0)
        {
            address_array[current_index] = str;
            index_array[current_index] = index;
            current_index++;

            return getValue<T>(index);
        }

        return default_;
    }
    //-----------------------------------------------------------------------------------
    ~Package()
    {

    }
    char buff[SIZE_BUFF];
private:
    const char *address_array[SIZE_SEARCH_ARRAY];
    uint16_t  index_array[SIZE_SEARCH_ARRAY];
    uint16_t  current_index;

    uint16_t set_item_index;
    uint16_t get_item_index;
};

#endif  