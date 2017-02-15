#include "serial.h"

int main()
{
    Serial gps(4800,"/dev/ttyUSB0",false);
    gps.serialRead(41);
    std::cout << gps.getData() << std::endl;
}
