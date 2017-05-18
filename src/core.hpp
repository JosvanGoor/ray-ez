#ifndef BBM_OBJECT_HPP
#define BBM_OBJECT_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <typeinfo>
#include <exception>

#include "math/vector3.hpp"
#include "math/vector4.hpp"
using math::Vector3d;
using math::Vector4d;

class Object
{
public:
    virtual std::string to_string() const = 0;
    virtual std::string class_name() const;
};

std::ostream& operator<<(std::ostream &os, const Object &obj);

class Exception : public std::exception
{
public:
    Exception(const std::string &source, const std::string &msg);

    virtual const char* what() const noexcept;
    virtual std::string message() const;

protected:
    Exception();
    std::string m_message;
};

std::string trim(const std::string &str);
std::string trim_front(const std::string &str);
std::string trim_back(const std::string &str);

std::vector<std::string> split(const std::string &str, char s);

std::string get_line(std::istream &is);
std::vector<unsigned char> read_file(std::string file);

#endif