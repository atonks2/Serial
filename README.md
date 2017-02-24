# Serial class for Linux
- Linux serial communication using the termios API

##Build using g++ with c++11
- Example:
`g++ -std=c++11 serial.h serial.cpp client-code.cpp

##Constructors:
- `Serial()`and `Serial(speed_t baud, std::string port)` assume canonical input mode.
  - Both delegate to `Serial(speed_t baud, std::string port, bool canon)`
- `Serial(speed_t baud, std::string port, bool canon)`
  - Client specifies baudrate port and input mode
