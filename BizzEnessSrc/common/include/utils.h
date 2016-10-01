#ifndef UTILS_H
#define UTILS_H
#include <string>

#define STRINGIFY(x) (#x)

using namespace std;

string intToString(int );
string doubleToString(double );
string uintToString(unsigned int );
unsigned int  hashCode(string text);
#endif // UTILS_H
