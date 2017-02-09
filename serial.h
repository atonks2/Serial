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

#ifndef SERAIL_H
#define SERIAL_H

#include <termios.h>
#include <iostream>
#include <string.h>

class Serial
{
public:
	// Default constructor assumes canonical input,
	// PORT = /dev/ttyUSB0 and BAUDRATE = 4800
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

#endif // SERIAL_H
