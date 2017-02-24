#include "serial.h"

//  This file is for testing each of the constructors in serial.h

void printConfig(termios config)
{
	std::cout << "\nc_cflag: " << std::hex << config.c_cflag << std::endl;
	std::cout << "c_iflag: " << std::hex << config.c_iflag << std::endl;
	std::cout << "c_oflag: " << std::hex << config.c_oflag << std::endl;
	std::cout << "c_lflag: " << std::hex << config.c_lflag << std::endl;
}

void testBaud(Serial &dev)
{
    std::cout << "Testing Baudrate...\n";
    std::cout << "  Current baud: " << dev.getBaud() << std::endl;
    std::cout << "  Setting baud to 115200. Exit status: " << dev.setBaud(115200) << std::endl;
    std::cout << "      New baud: " << std::hex << dev.getBaud() << std::endl;
    std::cout << "  Restoring to 4800. Exit status: " << dev.setBaud(4800) << std::endl;
    std::cout << "      New baud: " << std::hex << dev.getBaud() << std::endl;
}

void runTest(Serial &dev)
{
	std::cout << "Port Configuration: ";
	termios config = dev.getConfig();
	printConfig(config);
	std::cout << std::endl;

    testBaud(dev);

	dev.serialRead();
	std::cout << "NMEA Sentance: " << dev.getData() << std::endl;

    std::cout << "Test Completed.\n\n";
}

int main()
{

	// Default constructor
	Serial def_const;
	std::cout << "-------------------------------Default Constructor--------------------------------\n";
	runTest(def_const);

	// Serial(int baud, std::string port)
	Serial target_one(4800, "/dev/ttyUSB0");
	std::cout << "------------------------Serial(int baud, std::string port)------------------------\n";
	runTest(target_one);

	// Serial(int baud, std::string port, bool canon)
	Serial canon(4800, "/dev/ttyUSB0", true);
	std::cout << "------------------Serial(int baud, std::string port, bool canon)------------------\n";
	runTest(canon);

	// Serial(int baud, std::string port, bool canon)
	Serial noCanon(4800, "/dev/ttyUSB0", false);
	std::cout << "------------------Serial(int baud, std::string port, bool canon)------------------\n";
	runTest(canon);
    std::cout << std::endl;

    return 0;
}
