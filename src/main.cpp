#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring> 


// Project Configurations

const char* PORT_NAME = "COM9";                     // Set the serial port device name (replace with your Arduino port)
const char* OUTPUT_FILE_NAME = "out.csv";           // Will store the serial data into this csv file
DWORD baudRate = CBR_115200;                        // Set the baud rate (must match the baud rate of your Arduino)





void replaceChar(std::string& str, char oldChar, char newChar) {
    size_t pos = 0;

    // Loop through the string and replace occurrences
    while ((pos = str.find(oldChar, pos)) != std::string::npos) {
        str.replace(pos, 1, 1, newChar);
        pos += 1; // Move to the next character to avoid infinite loop for consecutive occurrences
    }
}

int indexOfChar(const std::string& str, char targetChar) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == targetChar) {
            return static_cast<int>(i);
        }
    }

    // Return -1 if 'targetChar' is not found
    return -1;
}

bool isCSVData(const std::string &str, char targetChar, int total_occurance) {
    
    int count = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == targetChar) {
            count++;
        }
    }
    bool result = count == total_occurance;
    return result;
}

int main() {

    std::ofstream outputfile;  
    outputfile.open(OUTPUT_FILE_NAME,std::ios_base::app);


    // Open the serial port
    HANDLE serialHandle = CreateFileA(
        PORT_NAME,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (serialHandle == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port. Error code: " << GetLastError() << std::endl;
        return 1;
    }

    // Set the serial port parameters
    DCB serialParams = { 0 };
    serialParams.DCBlength = sizeof(serialParams);
    if (!GetCommState(serialHandle, &serialParams)) {
        std::cerr << "Error getting serial port state. Error code: " << GetLastError() << std::endl;
        CloseHandle(serialHandle);
        return 1;
    }

    serialParams.BaudRate = baudRate;  // Baud rate: 9600
    serialParams.ByteSize = 8;         // Data bits: 8
    serialParams.StopBits = ONESTOPBIT; // Stop bits: 1
    serialParams.Parity = NOPARITY;    // Parity: None

    if (!SetCommState(serialHandle, &serialParams)) {
        std::cerr << "Error setting serial port state. Error code: " << GetLastError() << std::endl;
        CloseHandle(serialHandle);
        return 1;
    }

    // Read data from the serial port
    char buffer[256];
    DWORD bytesRead;

    while (1) {
        if (ReadFile(serialHandle, buffer, sizeof(buffer), &bytesRead, NULL)) {
            // Process or print the received data
            // std::cout.write(buffer, bytesRead);
            std::string result(buffer, bytesRead);
            if (isCSVData(result,'%',1) && result.length() > 3) {

                std::string clean_data = result.substr(5,sizeof(result)/sizeof(char)-1); 
                replaceChar(clean_data,';',',');
                std::cout << clean_data << std::endl;


                
                outputfile << clean_data << std::endl;
                // std::memset(buffer, 0, sizeof(buffer));
            }
        }
        else {
            std::cerr << "Error reading from serial port. Error code: " << GetLastError() << std::endl;
        }
    }

    // Close the serial port
    CloseHandle(serialHandle);

    return 0;
}
