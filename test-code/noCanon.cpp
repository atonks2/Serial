#include "serial.h"

int main()
{
    Serial gps(4800,"/dev/ttyUSB0",false);
	std::cout << gps.serialRead(41) << std::endl;
	return 0;
}
