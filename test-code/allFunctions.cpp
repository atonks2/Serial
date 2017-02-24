#include "serial.h"

void printConfig(termios &config)
{
    std::cout << "\n* Configuration: \n";
    std::cout << "c_cflag: " << std::hex << config.c_cflag << std::endl;
    std::cout << "c_iflag: " << std::hex << config.c_iflag << std::endl;
    std::cout << "c_oflag: " << std::hex << config.c_oflag << std::endl;
    std::cout << "c_lflag: " << std::hex << config.c_lflag << std::endl;
}

void readGPS(Serial &gps)
{
    for (int i = 0; i < 5; i++) {
        std::cout << "\nData: " << gps.serialRead();
    }
    std::cout << std::endl;
}   


int main()
{
    Serial gps(4800,"/dev/ttyUSB0");

    termios config = gps.getConfig();
    printConfig(config);

    // Set baudrate
    std::cout << "Setting baud to 4800: " << gps.setBaud(4800) << std::endl;
    std::cout << "Apply Configuration: " << gps.applyNewConfig() << std::endl;
    std::cout << "Current baud: " << gps.getBaud() << std::endl;
    readGPS(gps);

    // Attempt to change baudrate
    std::cout << "Changing baudrate on GPS $PSRF100,1,9600,8,1,0*0D: " << gps.serialWrite("$PSRF100,1,9600,8,1,0*0D\r\n") << std::endl;
    std::cout << "Setting serial port baudrate: " << gps.setBaud(9600) << " Applying: " << gps.applyNewConfig() << std::endl;
    std::cout << "Port baudrate: " << gps.getBaud() << std::endl;
    readGPS(gps);

    // Restore baudrate
    std::cout << "Changing baudrate on GPS $PSRF100,1,4800,8,1,0*0E: " << gps.serialWrite("$PSRF100,1,4800,8,1,0*0E\r\n") << std::endl;
    std::cout << "Setting serial port baudrate: " << gps.setBaud(4800) << " Applying: " << gps.applyNewConfig() << std::endl;
    std::cout << "Port baudrate: " << gps.getBaud() << std::endl;
    readGPS(gps);

    return 0;
}
