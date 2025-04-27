#pragma once
#include <iostream>
#include <string>


//template<typename T>
void Print_Debug(const std::string& str, const double& val=0)
{
    std::cout << str <<": " << val << std::endl;
}