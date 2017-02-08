#include "serial.h"

speed_t baud = 4800;
std::string port = "/dev/ttyUSB0";
Serial gps(baud, port);

void readGPS(int n)
{
	for(int i = 0; i < n; ++i) {
		gps.serialRead();
		std::cout << gps.getData();
	}
}

void printConfig()
{
	termios cur_config = gps.getConfig();
	std::cout << "c_cflag: " << cur_config.c_cflag << std::endl;
	std::cout << "c_iflag: " << cur_config.c_iflag << std::endl;
	std::cout << "c_oflag: " << cur_config.c_oflag << std::endl;
	std::cout << "c_lflag: " << cur_config.c_lflag << std::endl;
}

int main()
{
	std::cout << "Current configuration: " << "\n--------------------------------\n";
	printConfig();
	std::cout << "\nGPS Data: " << "\n--------------------------------\n";
	readGPS(200);
	
	return 0;
}
