#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring> 
#include <ctime>
#include <iomanip>

// Project Configurations

const char* PORT_NAME = "COM9";                     // Set the serial port device name (replace with your Arduino port)
const char* OUTPUT_FILE_NAME = "out.csv";           // Will store the serial data into this csv file
DWORD BAUD_RATE = CBR_115200;                        // Set the baud rate (must match the baud rate of your Arduino)



void replaceChar(std::string& str, char oldChar, char newChar);
int indexOfChar(const std::string& str, char targetChar);
bool isCSVData(const std::string &str, char targetChar, int total_occurance);
std::string getCurrentTimeStamp();
std::string trim(const std::string& str);

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

    serialParams.BaudRate = BAUD_RATE;  // Baud rate: 9600
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
            result = trim(result);

            if (result.front() == '{' && result.back() == '}') {


                std::string current_timestamp = getCurrentTimeStamp();

                std::string clean_data = result.substr(1,sizeof(result)/sizeof(char)-1); 
                
                replaceChar(clean_data,';',',');
                replaceChar(clean_data,'}',' ');
                clean_data = trim(clean_data);


                clean_data += ", " + current_timestamp;
                std::cout << clean_data << std::endl;

                outputfile << clean_data << std::endl;

                std::memset(buffer, 0, sizeof(buffer));
            }
                std::cout << result << std::endl;

        }
        else {
            std::cerr << "Error reading from serial port. Error code: " << GetLastError() << std::endl;
        }
    }

    // Close the serial port
    CloseHandle(serialHandle);

    return 0;
}


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

std::string getCurrentTimeStamp() {

    
    auto currentTime = std::chrono::system_clock::now();
    
    // Convert the time to a time_t type
    std::time_t timeT = std::chrono::system_clock::to_time_t(currentTime);
    
    // Convert the time_t to a struct tm in local time
    std::tm* tmPtr = std::localtime(&timeT);

    // Get the milliseconds part of the current time
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime.time_since_epoch() % std::chrono::seconds(1)
    ).count();

    // Format the time as "YYYY-MM-DD HH:MM:SS:MS"
    char buffer[24];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmPtr);

    // Convert the formatted time to a string and append milliseconds
    std::string timestamp_str(buffer);
    timestamp_str += "." + std::to_string(milliseconds);

    return timestamp_str;

}

std::string trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    // Trim spaces from the start
    while (start < end && std::isspace(str[start])) {
        start++;
    }

    // Trim spaces from the end
    while (end > start && std::isspace(str[end - 1])) {
        end--;
    }

    // Return the trimmed substring
    return str.substr(start, end - start);
}