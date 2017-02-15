#include "serial.h"

//  This file is for testing each of the constructors in serial.h

void printConfig(termios config)
{
	std::cout << "c_cflag: " << std::hex << config.c_cflag << std::endl;
	std::cout << "c_iflag: " << std::hex << config.c_iflag << std::endl;
	std::cout << "c_oflag: " << std::hex << config.c_oflag << std::endl;
	std::cout << "c_lflag: " << std::hex << config.c_lflag << std::endl;
}

void runTest(Serial dev)
{
	std::cout << "Port Configuration: ";
	termios config = dev.getConfig();
	printConfig(config);
	std::cout << std::endl;
	dev.serialRead();
	std::cout << "NMEA Sentance: " << dev.getData();
}

int main()
{
	// Default constructor
	Serial def_const();
	// Serial(int baud, std::string port)
	Serial target_one(4800, "/dev/ttyUSB0");
	// Serial(int baud, std::string port, bool canon)
	Serial canon(4800, "/dev/ttyUSB0", true);
	// Serial(int baud, std::string port, bool canon)
	Serial noCanon(4800, "/dev/ttyUSB0", false);

	std::cout << "-------------------------------Default Constructor--------------------------------\n";
	runTest(def_const);

	std::cout << "------------------------Serial(int baud, std::string port)------------------------\n";
	runTest(target_one);

	std::cout << "------------------Serial(int baud, std::string port, bool canon)------------------\n";
	runTest(canon);

	std::cout << "------------------Serial(int baud, std::string port, bool canon)------------------\n";
	runTest(canon);
}