
// C library headers
#include <stdio.h>
#include <string.h>
#include <CppLinuxSerial/SerialPort.hpp>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
//#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <iostream>
#include<algorithm>
using namespace mn::CppLinuxSerial;
int main(){
//int serial_port = open("/dev/ttyACM0", O_RDWR);

// Check for errors
/*if (serial_port < 0) {
    printf("Error %i from open: %s\n", errno, strerror(errno));
}*/
SerialPort serialPort("/dev/ttyACM0", BaudRate::B_9600, NumDataBits::EIGHT, Parity::NONE, NumStopBits::ONE);
	// Use SerialPort serialPort("/dev/ttyACM0", 13000); instead if you want to provide a custom baud rate
	serialPort.SetTimeout(100); // Block for up to 100ms to receive data
	serialPort.Open();
while(true){
std::string readData;

	serialPort.Read(readData);
	readData.erase(std::remove(readData.begin(), readData.end(), '\n'), readData.end());
		readData.erase(std::remove(readData.begin(), readData.end(), ' '), readData.end());
				//readData.erase(std::remove(readData.begin(), readData.end(), ''), readData.end());


	/*if(readData == "" || "\n"){
		std::cout << "shit" << std::endl;
	}*/
	std::cout << "Read data = " << readData <<  std::endl;

}
	serialPort.Close();
}