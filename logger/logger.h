//
// Created by yuuki on 5/14/22.
//

#ifndef LOG_LOGGER_H
#define LOG_LOGGER_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <sys/stat.h>
#include <future>
#include <mutex>
#include <filesystem>
#include <cstring>

using namespace std;

enum logLevel {debug = 1, info, warn, error, fatal};
//              year-month-day hour-min-sec    thread level content file:line
#define FORMAT "%04d-%02d-%02d %02d:%02d:%02d\t%d\t%s\t%s:%d\t%s\n"

//                  logfile.year-month-day.hostname.log1
#define FILENAME "logFiles/logfile.%04d-%02d-%02d.%s.log"

#define WARN(content) toFormat(content, __FILE__, __LINE__, warn)
#define INFO(content) toFormat(content, __FILE__, __LINE__, info)
#define DEBUG(content) toFormat(content, __FILE__, __LINE__, debug)
#define FATAL(content) toFormat(content, __FILE__, __LINE__, fatal)
#define ERROR(content) toFormat(content, __FILE__, __LINE__, error)

#define TERMINAL 0
#define LOGFILE 1
#define MAXSIZE (2 * 1024 * 1024) //2m
#define BUFSIZE 2048 //2k

string toFormat(const char *content, const char *file, int line, int logLevel);
void getTime(tm *&now);

class Logger {
private:
    static Logger * volatile logInstance;
    static mutex mx;
    fstream output{};
    int num{};
    char filename[80]{};

    char buf[BUFSIZE]{};
    int logNum{};
    time_t lastFlushedTime{};

    void bufFlush();
    void init();
    void judgeSize();
    void createNewLogFile();
    void flush();
    bool ifFlush(const string& s);
    void log2(const string& s, int type);

    Logger() = default;
    ~Logger();
    Logger(const Logger &);
    Logger &operator=(const Logger &);
public:
    void log(const string& s);
    void log(const string& s, int type);
    void log(const string& s, fstream& _output);
    static Logger* volatile getInstance();
};


#endif //LOG_LOGGER_H
