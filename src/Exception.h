#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <cstdarg>
#include <cstdio>
#include <string>
#include <stdexcept>

class Exception : public std::exception
{
public:
    // Constructor
    Exception(const char* format, ...)
        : m_error_msg(format)
    {
        // Init va_list and use to determine length
        va_list args;
        va_start(args, format);
        int length = vsnprintf(nullptr, 0, format, args) + 1;
        va_end(args);

        // Dynamically allocate memory for buffer
        char* buffer = new char[length];

        // Restart va_list for actual use
        va_start(args, format);
        vsnprintf(buffer, length, format, args);
        va_end(args);

        // Save message and free dynamically allocated buffer
        m_error_msg = buffer;
        delete[] buffer; 
    }

    // Destructor
    ~Exception() _NOEXCEPT {}

    /*
     * @brief Retrieve 'what' the exception is
     */
    const char* what() const _NOEXCEPT override
    {
        return m_error_msg.c_str();
    }

private:
    /*
     * @brief Stores exception string
     */
    std::string m_error_msg; 
};

#endif // EXCEPTION_H