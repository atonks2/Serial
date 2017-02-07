#ifndef SERAIL_H
#define SERIAL_H
 
#include <termios.h>
#include <iostream>
#include <string.h>
 
class Serial
{
public:
        Serial();
        Serial(speed_t baud, std::string port);
 
        int setBaud(speed_t baud);
	int applyNewConfig();
        speed_t getBaud();
        termios getConfig();

        int serialRead();
        std::string getData();
         
        ~Serial();
private:
        void init();
        struct termios old_config, new_config;
        int dev_fd, bytes_received;
        speed_t BAUDRATE;
        std::string PORT, data;
        bool isOpen;
};
 
#endif
