//
// Created by yuuki on 5/14/22.
//

#include "logger.h"

Logger * volatile Logger::logInstance = nullptr;
mutex Logger::mx{};

void getTime(tm *&now) {
    time_t t = time(nullptr);
    now = localtime(&t);
}

//                year-month-day hour-min-sec    thread level content file:line
//#define FORMAT "%04d-%02d-%02d %02d:%02d:%02d   %-6d %s  %s  %s:%d\n"
string toFormat(const char *content, const char *file, int line, int logLevel) {
    tm *now{};
    getTime(now);
    string level;
    switch (logLevel) {
        case info:
            level = "INFO";
            break;
        case debug:
            level = "DEBUG";
            break;
        case error:
            level = "ERROR";
            break;
        case fatal:
            level = "FATAL";
            break;
        case warn:
            level = "WARN";
            break;
    }
    char s[80];
    sprintf(s, FORMAT, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min,
            now->tm_sec, gettid(), level.c_str(), strrchr(file, '/') + 1, line, content);
    string ss(s);
    return ss;
}

void Logger::init() {
    int res = access("logFiles/", F_OK); //0 -> existed  -1 -> nonexistent
    if (res == -1) {
        mkdir("logFiles/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    tm *now{};
    getTime(now);
    char hostname[20];
    gethostname(hostname, 20);
    sprintf(filename, FILENAME, now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, hostname);
    for (int i = 0; ; i++) {
        string s = string(filename) + to_string(i + 1);
        if (access(s.c_str(), F_OK) == -1) { //nonexistent
            string s2 = string(filename) + to_string(i);
            num = i;
            this->output.open(s2.c_str(), ios::app);
            break;
        }
    }
}

void Logger::judgeSize() {
    lock_guard<mutex> lock(mx);
    output.seekg(0, ios::end);
    long length = output.tellg();
    if (length >= MAXSIZE) {
        createNewLogFile();
    }
}

void Logger::createNewLogFile() {
    string s = string(filename) + to_string(num + 1);
    num++;
    output.close();
    output.clear();
    output.open(s, ios::app);
}

Logger * volatile Logger::getInstance() {
    if (logInstance == nullptr) {
        lock_guard<mutex> lock(mx);
        if (logInstance == nullptr) {
            auto * volatile temp = new Logger;
            logInstance = temp;
        }
    }
    return logInstance;
}

void Logger::log(const string& s) {
    cout << s << endl;
}

void Logger::log(const string& s, int type) {
    if (type == TERMINAL) {
        log(s);
    } else if (type == LOGFILE) {
        if (!output.is_open()) {
            init();
        }
        log(s, output);
        judgeSize();
    }
}

void Logger::log(const string &s, fstream &_output) {
    lock_guard<mutex> lock(mx);
    _output << s << endl;
}

Logger::~Logger() {
    output.close();
}

void Logger::flush() {
    lock_guard<mutex> lock(mx);
    output << buf << endl;
}

