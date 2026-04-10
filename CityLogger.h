#ifndef CITYLOGGER_H
#define CITYLOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <vector>
#include <stdexcept>
#include <memory>

using namespace std;

/**
 * Generic template class for city logging
 * Can log any type of data with a timestamp
 */
template<typename T>
class CityLogger {
private:
    string logName;
    ofstream logFile;
    vector<pair<string, T>> logEntries;
    
    // Get current timestamp as string
    string getCurrentTimestamp() const {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return string(buf);
    }
    
public:
    // Constructor
    CityLogger(const string& name, const string& filename = "") 
        : logName(name) {
        if (!filename.empty()) {
            openLogFile(filename);
        }
    }
    
    // Destructor
    ~CityLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    // Open log file
    void openLogFile(const string& filename) {
        logFile.open(filename, ios::app);
        if (!logFile.is_open()) {
            throw runtime_error("Could not open log file: " + filename);
        }
        log("Log file opened");
    }
    
    // Close log file
    void closeLogFile() {
        if (logFile.is_open()) {
            log("Log file closed");
            logFile.close();
        }
    }
    
    // Log an entry
    void log(const T& data) {
        string timestamp = getCurrentTimestamp();
        logEntries.push_back(make_pair(timestamp, data));
        
        if (logFile.is_open()) {
            logFile << "[" << timestamp << "] " << data << endl;
        }
    }
    
    // Get all log entries
    const vector<pair<string, T>>& getLogEntries() const {
        return logEntries;
    }
    
    // Display all log entries
    void displayAllLogs() const {
        cout << "===== " << logName << " Logs =====" << endl;
        for (const auto& entry : logEntries) {
            cout << "[" << entry.first << "] " << entry.second << endl;
        }
        cout << "==========================" << endl;
    }
    
    // Find logs containing a specific substring
    vector<pair<string, T>> findLogs(const string& searchTerm) const {
        vector<pair<string, T>> results;
        
        for (const auto& entry : logEntries) {
            // Convert T to string using to_string for numeric types
            string entryStr = to_string(entry.second);
            if (entryStr.find(searchTerm) != string::npos) {
                results.push_back(entry);
            }
        }
        
        return results;
    }
    
    // Clear all entries from memory (not file)
    void clearEntries() {
        logEntries.clear();
    }
};

// Template specialization for std::string to handle findLogs properly
template<>
vector<pair<string, string>> CityLogger<string>::findLogs(const string& searchTerm) const {
    vector<pair<string, string>> results;
    for (const auto& entry : logEntries) {
        if (entry.second.find(searchTerm) != string::npos) {
            results.push_back(entry);
        }
    }
    return results;
}

#endif // CITYLOGGER_H