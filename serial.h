#ifndef SERAIL_H
#define SERIAL_H
 
#include <termios.h>
#include <iostream>
#include <string.h>
 
class Serial
{
public:
        Serial();
	Serial(speed_t baud, std::string port); /* Assume canonical input */
	Serial(speed_t baud, std::string port, bool canon);

        int setBaud(speed_t baud);
	int applyNewConfig();
        int serialRead(); /* Canonical read (line) */
	int serialRead(int bytes); /* non-canonical read */

	speed_t getBaud();
        termios getConfig();
        std::string getData();
         
        ~Serial();
private:
        void init();
        struct termios old_config, new_config;
        int dev_fd, bytes_received;
        speed_t BAUDRATE;
        std::string PORT, data;
        bool isOpen, isCanonical;
};
 
#endif
