#include <iostream>
#include <cmath>

using namespace std;

int main(int argc, const char *argv[])
{
    double num = 2.3;
    num = (int)(num * 100000) / 100000.0;

    int len = 0;

    if (num < 0)
    {
        ++len;
        num = -num;
    }
    int integer = round(num);
    
    do
    {
        ++len;
        integer /= 10;
    }
    while (integer > 0);
    
    double decimals = num - round(num);
    decimals = fabs(decimals);
    if (decimals > 1e-5)
    {
        ++len;
        do
        {
            ++len;
            decimals *= 10;
            decimals -= round(decimals);
            decimals = fabs(decimals);
        }
        while (decimals > 1e-4);
    }

    cout << len << endl;
    return 0;
}