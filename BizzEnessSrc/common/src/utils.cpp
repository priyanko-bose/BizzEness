#include <string>
#include <sstream>

std::string uintStr = "";
std::string intStr = "";
std::string doubleStr = "";

std::string uintToString(unsigned int i)
{
    std::stringstream ss;
    ss << i;
    uintStr = ss.str();
    return uintStr;
}
std::string intToString(int i)
{
    std::stringstream ss;
    ss << i;
    intStr = ss.str();
    return intStr;
}
std::string doubleToString(double d)
{
    std::stringstream ss;
    ss << d;
    doubleStr = ss.str();

    return doubleStr;
}
