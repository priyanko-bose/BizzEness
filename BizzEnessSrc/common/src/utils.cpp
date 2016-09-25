#include <string>
#include <sstream>
#define HASHMASK 0x00FF

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

unsigned int  hashCode(std::string text){
    int i;
    unsigned int h;
    int len = text.length();
    h = 1315423911;
    for (i = 0; i < len;  i++) {
           h ^= ((h << 5) + (text[i]) + (h >> 2));
    }
    h &= HASHMASK;
    return h;
}
