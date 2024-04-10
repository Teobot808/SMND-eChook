
// C library headers
#include <stdio.h>
#include <string.h>
#include <CppLinuxSerial/SerialPort.hpp>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
//#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <curl/curl.h>


using namespace mn::CppLinuxSerial;

std::string httpGetRequest(const std::string& url) {
    CURL *curl;
    CURLcode res;
    std::string response;

    // Initialize CURL
    curl = curl_easy_init();
    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Set the write callback function
        /*curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](char* ptr, size_t size, size_t nmemb, std::string* data) -> size_t {
            data->append(ptr, size * nmemb);
            return size * nmemb;
        });*/
        // Set the response string
        //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        
        // Perform the request
        std::cout << "Making HTTP GET request to: " << url << std::endl;
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        
        // Get the response code
        long response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        std::cout << "\nResponse code: " << response_code << std::endl;
        
        // Clean up
        curl_easy_cleanup(curl);
    }

    return response;
}

int main(){

    // Base URL
    std::string baseUrl = "https://smnd.sk/ttravnicek/citaj.php?appid=Tajn3hE5lo&data=";
    // Number of times to repeat
    int repeatCount = 100; // Change this to the desired repeat count

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
	std::cout << "Read data = " << readData << std::endl;
    
    
    if(readData != "" || " " || "\n"){
      //  int i = std::stoi(readData);
    std::string url = baseUrl + readData;
        // Perform HTTP GET request
        std::string response = httpGetRequest(url);
        // Print the entire response
        std::cout << "Response from " << url << ":\n" << response << std::endl;
    }
}
	serialPort.Close();
}