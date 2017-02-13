/*
MIT License

Copyright (c) 2017 Alan Tonks

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "serial.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

Serial::Serial() {
	PORT = "/dev/ttyUSB0";
	Serial(4800, PORT, true);
}

Serial::Serial(speed_t baud, std::string port)
{
	Serial(baud, port, true);
}

Serial::Serial(speed_t baud, std::string port, bool canon)
{
	// Make a copy of the current configuration so it can be
	// restored in destructor.
	tcgetattr(dev_fd, &oldConfig);
	isCanonical = canon;
	if ((baud % 2400) == 0) BAUDRATE = baud;  // baudrate must be multiple of 2400
	isOpen = false;
	struct stat stat_buf;  // See if specified port exists
	if (stat(port.c_str(), &stat_buf) == 0) PORT = port;
	else {  // Port doesn't exist
		std::cout << "Device not found." << std::endl;
		exit(-1);
	}
	init();
}

// Open and configure the port
void Serial::init()
{
	// open port for read and write, not controlling, ignore DCD line
	dev_fd = open(PORT.c_str(), O_RDWR | O_NOCTTY);
	if (dev_fd < 0) {
		perror("Failed to open device: ");
		exit(-1);
	}
	else isOpen = true;

	memset(&terminalConfiguration, 0, sizeof(terminalConfiguration));  // Clear junk from location of terminalConfiguration to start with clean slate
	tcgetattr(dev_fd, &terminalConfiguration);

	// TERMIOS CONFIGURATION
	
	// BAUDRATE: Integer multiple of 2400
	// CRTSCTS: Hardware flow control
	// CS8: 8N1
	// CLOCAL: No modem control. (local device)
	// CREAD: Receive chars
	terminalConfiguration.c_cflag |= (BAUDRATE | CS8 | CLOCAL | CREAD);

    // IGNPAR: Ignore parity errors
	terminalConfiguration.c_iflag |= IGNPAR;

	// 0 for raw output
	terminalConfiguration.c_oflag = 0;

	// Setting input mode
	if (isCanonical)  terminalConfiguration.c_lflag |= (ICANON | ECHO | ECHOE);  // Canonical input
	else {
		//Configure non-canonical mode
		terminalConfiguration.c_lflag &= ~(ICANON | ECHO | ECHOE);  // Disable canonical mode and echo
		terminalConfiguration.c_cc[VMIN] = 1;  // Minimum number of chars to read before returning
		terminalConfiguration.c_cc[VTIME] = 0;  // Timeout in deciseconds. 0 to disregard timing between bytes
	}
	tcflush(dev_fd, TCIFLUSH);
	applyNewConfig();
}

int Serial::setBaud(speed_t baud)
{
	if (!isOpen) return -1;
	int status_i = -1;
    int status_o = -1;

	switch (baud) {
	case 2400:
		status_i = cfsetispeed(&terminalConfiguration, B2400);
        status_o = cfsetospeed(&terminalConfiguration, B2400);
		break;
	case 4800:
		status_i = cfsetispeed(&terminalConfiguration, B4800);
        status_o = cfsetospeed(&terminalConfiguration, B4800);
		break;
	case 9600:
		status_i = cfsetispeed(&terminalConfiguration, B9600);
        status_o = cfsetospeed(&terminalConfiguration, B9600);
		break;
	case 19200:
		status_i = cfsetispeed(&terminalConfiguration, B19200);
        status_o = cfsetospeed(&terminalConfiguration, B19200);
		break;
	case 38400:
		status_i = cfsetispeed(&terminalConfiguration, B38400);
        status_o = cfsetospeed(&terminalConfiguration, B38400);
		break;
	case 57600:
		status_i = cfsetispeed(&terminalConfiguration, B57600);
        status_o = cfsetospeed(&terminalConfiguration, B57600);
		break;
	case 115200:
		status_i = cfsetispeed(&terminalConfiguration, B115200);
        status_o = cfsetospeed(&terminalConfiguration, B115200);
		break;
	case 230400:
		status_i = cfsetispeed(&terminalConfiguration, B230400);
        status_o = cfsetospeed(&terminalConfiguration, B230400);
		break;
	default:
		std::cout << "Invalid baudrate requested.\n";
		return -1;
	}
	if (status_i < 0 || status_o < 0) {
		perror("Failed to set requested baudrate: ");
		return -1;
	}
	else return status_i;
}

int Serial::applyNewConfig()
{
	if (isOpen) {
		if (tcsetattr(dev_fd, TCSANOW, &terminalConfiguration) < 0) perror("Could not apply configuration: ");
		else return 0;
    }
	else return -1;
}

speed_t Serial::getBaud()
{
    return cfgetispeed(&terminalConfiguration);
	}

termios Serial::getConfig()
{
	return terminalConfiguration;
}

// Helper function checks if port is open,
// then flushes the serial line.
int Serial::setupRead()
{
	if (!isOpen) return -1;
	if (tcflush(dev_fd, TCIOFLUSH < 0)) {
		perror("Could not flush line: ");
		return -1;
	}
    else return 0;
}

int Serial::serialRead()
{
	if (setupRead() < 0) return -1;
    int buf_size = 255; // 82 is longest NMEA Sentence
	char buf[buf_size];
	bytesReceived = read(dev_fd, buf, buf_size);
	if (bytesReceived < 0) perror("Read failed: ");
	else buf[bytesReceived] = '\0';  // Null terminate the string
	serialData.assign(buf);  // store serialData as std::string
	return bytesReceived;
}

int Serial::serialRead(int bytes)
{
	if (setupRead() < 0) return -1;
    int buf_size = bytes;
	char buf[buf_size];
	bytesReceived = read(dev_fd, buf, bytes);
	if (bytesReceived < 0) perror("Read failed: ");
	else buf[bytesReceived] = '\0';  // Null terminated
	serialData.assign(buf);  // Store as std::string
	return bytesReceived;
}

int Serial::flush()
{
    return tcflush(dev_fd, TCIOFLUSH);
}

int Serial::serialWrite(std::string str)
{
    if (!isOpen) return -1;
    int write_status =  write(dev_fd, str.c_str(), str.length());
    if (write_status < 0) {
        perror("Failed to write to port: ");
        return -1;
    }
    else return write_status;
}

std::string Serial::getData()
{
	if (isOpen) return serialData;
	else return "Open serial port first!\n";
}

Serial::~Serial()
{
	tcsetattr(dev_fd, TCSANOW, &oldConfig); /* Leave port how we found it. */
	close(dev_fd); /* close the port */
}