#pragma once
#include <exception>


    void register_for_os_exceptions();

    struct null_pointer_exception : public std::exception
    {
        const char* what() 
        {
            return "OS exception: null pointer!";
        }
    };

    struct division_by_zero_exception : public std::exception
    {
        const char* what() 
        {
            return "OS exception: division by zero!";
        }
    };


