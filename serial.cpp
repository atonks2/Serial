#include "serial.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

// Testing Windows Git
Serial::Serial() {
        PORT = "/dev/ttyUSB0"; /* A default to use if no port is specified */
        if (setBaud(4800) == 0) BAUDRATE = 4800; /* Use 4800 baud if no rate is specified */
        isOpen = false; /* Port isn't open yet */
        isCanonical = true;
	init();
}
 
Serial::Serial(speed_t baud, std::string port) /* Recommended constructor */
{
    if (setBaud(baud) == 0) BAUDRATE = baud; /* Check for valid baudrate and set the flag if it is valid */
    isOpen = false; /* Port isn't open yet */
    isCanonical = true;
    struct stat stat_buf; /* Used in checking if specified port exists */
    if (stat(port.c_str(), &stat_buf) == 0) /* Make sure the specified port exists */
        PORT = port; // port is valid
    else {
        std::cout << "Device not found." << std::endl; /* Port not found */
        exit(-1);
    }
    init();
}

Serial::Serial(speed_t baud, std::string port, bool canon)
{
	isCanonical = canon;
	if (setBaud(baud) == 0) BAUDRATE = baud;
	isOpen = false;
	struct stat stat_buf;
	if (stat(port.c_str(), &stat_buf) == 0) PORT = port;
	else {
		std::cout << "Device not found." << std::endl;
		exit(-1);
	}
	init();
} 

void Serial::init() /* Open and configure the port */
{
    /* open port for read and write, not controlling */
    dev_fd = open(PORT.c_str(), O_RDWR | O_NOCTTY); /* open port for read/write, not controlling */
    if (dev_fd < 0) {
        perror("Failed to open device: ");
        exit(-1);
    }
    else if (dev_fd >= 0)
        isOpen = true;
 
    tcgetattr(dev_fd, &old_config); /* save current port settings */
 
    // define configuration vars for termios struct
    memset(&new_config, 0, sizeof(new_config)); /* clear mem first */
    tcgetattr(dev_fd, &new_config); /* Per GNU C, get current config and modify the flags you need. Don't start from scratch */
    /*
       BAUDRATE: Integer multiple of 2400
       CRTSCTS: Hardware flow control
       CS8: 8N1
       CLOCAL: No modem control. (local device)
       CREAD: Receive chars
    */
    new_config.c_cflag |= (BAUDRATE | CS8 | CLOCAL | HUPCL | CREAD);
    /*
       IGNPAR: Ignore parity errors
    */
    new_config.c_iflag |= IGNPAR;
    new_config.c_oflag = 0; /* use raw output */
    if (isCanonical)  new_config.c_lflag |= ICANON; /* Canonical input */
    else {
        /* Configure for non-canonical mode */
        new_config.c_lflag &= ~(ICANON | ECHO);
        new_config.c_cc[VMIN] = 1;
        new_config.c_cc[VTIME] = 0;
    }
    tcflush(dev_fd, TCIFLUSH); /* flush serial line */
    applyNewConfig();
}
 
int Serial::setBaud(speed_t baud)
{
	if (!isOpen) return -1;
	int status = -1;
	switch(baud) {
		case 2400:
			status = cfsetspeed(&new_config,B2400);
			break;
		case 4800:
			status = cfsetspeed(&new_config,B4800);
			break;
		case 9600:
			status = cfsetspeed(&new_config,B9600);
			break;
		case 19200:
			status = cfsetspeed(&new_config,B19200);
			break;
		case 38400:
			status = cfsetspeed(&new_config,B38400);
			break;
		case 57600:
			status = cfsetspeed(&new_config,B57600);
			break;
		case 115200:
			status = cfsetspeed(&new_config,B115200);
			break;
		case 230400:
			status = cfsetspeed(&new_config,B230400);
			break;
		default: 
			std::cout << "Invalid baudrate requested.\n";
			return -1;
	}
	if (status < 0) {
		perror("Failed to set requested baudrate: ");
		return -1;
	}
	else return status;
}

int Serial::applyNewConfig()
{
	if (isOpen) {
		if (tcsetattr(dev_fd, TCSANOW, &new_config) < 0) perror("Could not apply configuration: ");
		else return 0;
	}
	else return -1;
}
 
/* return current line speed. 
*  Line speeds are defined as hexidecimal
*  constants. cfgetispeed() returns 
*  speed_t (unsigned int). 
*   12 = 4800, 13 = 9600, etc
*/
speed_t Serial::getBaud()
{
    if (isOpen) return cfgetispeed(&new_config);
    else return 1;
}
 
/* return current port coniguration flags */
termios Serial::getConfig()
{
    return new_config;
}
 
int Serial::serialRead()
{
    if (!isOpen) return -1;
    int buf_size = 255; // 82 is longest NMEA Sentence
    char buf[buf_size];
    if (tcflush(dev_fd, TCIOFLUSH < 0)) perror("Could not flush line: ");
    bytes_received = read(dev_fd, buf, buf_size);
    if (bytes_received < 0) perror("Read failed: ");
    else buf[bytes_received] = '\0'; /* Null terminate the string */
    data.assign(buf); /* store data as a c++ string */
    return bytes_received;
}

int Serial::serialRead(int bytes)
{
    if (!isOpen) return -1;
    int buf_size = 255;
    char buf[buf_size];
    if (tcflush(dev_fd, TCIOFLUSH <0)) perror("Could not flush line: ");
    if (bytes > buf_size) bytes = buf_size;
    bytes_received = read(dev_fd, buf, bytes);
    if (bytes_received < 0) perror("Read failed: ");
    else buf[bytes_received] = '\0';
    data.assign(buf);
    return bytes_received;
}
 
std::string Serial::getData()
{
    if (isOpen) return data;
    else return "Open serial port first!\n";
}
 
Serial::~Serial()
{
    tcsetattr(dev_fd, TCSANOW, &old_config); /* Leave port how we found it. */
    close(dev_fd); /* close the port */
}
