#include "pixelcore/colors.h"
using namespace px;

#include <iostream>
using std::cout, std::endl;

int main()
{
    Color color = pack(0, 0, 137);
    RGBA rgba = unpack(color);
    nRGBA normalized = nunpack(color);

    cout << color << endl;
    cout << rgba << endl;
    cout << normalized << endl;
    cout << hex(color) << endl;
    cout << hex(rgba) << endl;
    cout << hex(normalized) << endl;

    return 0;
}
