// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <sstream>
#include <bits/stdc++.h>
#include <curl/curl.h>

// Function to perform HTTP GET request
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


std::string convertToString(char* a, int size)
{
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}
int count(std::string s, char c)
{
    // Count variable
    int res = 0;
 
    for (int i=0;i<s.length();i++)
 
        // checking character in string
        if (s[i] == c)
            res++;
 
    return res;
}
char c = 'g';

int main() {
  // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
  int serial_port = open("/dev/ttyACM0", O_RDWR);

  std::string baseUrl = "https://smnd.sk/ttravnicek/citaj.php?appid=Tajn3hE5lo&data=";
    // Number of times to repeat
    int repeatCount = 100; // Change this to the desired repeat count


  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  // Write to serial port
  unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
  write(serial_port, msg, sizeof(msg));
int size = 4096;
  // Allocate memory for read buffer, set size according to your needs
  char read_buf [size];
std::string gec = "";
while (true)
  {

        //gec = "";


  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME

  
    
  
  
  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
    int a_size = sizeof(read_buf) / sizeof(char);
    std::string s_a = convertToString(read_buf, a_size);
  // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
  if (num_bytes < 0) {
      printf("Error reading: %s", strerror(errno));
      return 1;
  }

  // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
  // print it to the screen like this!)
  //printf(" %i bytes. %s   ", num_bytes, read_buf);
  
  
  /*if(s_a[s_a.length()-1] == 'g') {
    std::cout << gec << std::endl;
    gec = "";
  } else {
    gec += s_a;
  } 
    */
  // 
  
    gec += s_a;
   
   if(count(s_a, c) > 0){
     
       /* gec.erase(' ');
                gec.erase('\n');
 */
   gec.erase(remove(gec.begin(), gec.end(), '\n'), gec.end()); //remove A from string
 std::cout << gec << std::endl;
        std::cout << "*" << gec << "*" << std::endl;
      std::string url = baseUrl + gec;
        // Perform HTTP GET request
        std::string response = httpGetRequest(url);
        // Print the entire response
        std::cout << "Response from " << url << ":\n" << response << std::endl;
   
gec = "";
   }
  //  std::cout << s_a << std::endl;
  



  
  }
  close(serial_port);
  return 0; // success
};