#include <iostream>
#include "logger/logger.h"
#include <thread>

using namespace std;

void foo1();
void foo2();
void foo3();
void foo4();
void foo5();
void foo6();

int main() {
//    Logger log = Logger(LOGFILE);
//    //log.debug("This is a debug");
//    auto f = async(launch::async, &Logger::debug, &log, "async debug test");
//    f = async(launch::async, &Logger::fatal, &log, "async fatal test");
//    f = async(launch::async, &Logger::error, &log, "async error test");
    thread th1(foo1);
    thread th2(foo2);
    thread th3(foo3);
    thread th4(foo4);
    thread th5(foo5);
    thread th6(foo6);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();

    return 0;
}

void foo1() {
    for (int i = 0; i < 10; i++) {
        Logger::getInstance()->log(WARN("terminal"));
    }
}

void foo2() {
    for (int i = 0; i < 1000; i++) {
        Logger::getInstance()->log(DEBUG("debug()"), LOGFILE);
    }
}

void foo3() {
    for (int i = 0; i < 1000; i++) {
        Logger::getInstance()->log(INFO("info()"), LOGFILE);
    }
}

void foo4() {
    for (int i = 0; i < 1000; i++) {
        Logger::getInstance()->log(ERROR("error()"), LOGFILE);
    }
}

void foo5() {
    for (int i = 0; i < 1000; i++) {
        Logger::getInstance()->log(FATAL("fatal()"), LOGFILE);
    }
}

void foo6() {
    for (int i = 0; i < 1000; i++) {
        Logger::getInstance()->log(WARN("warn()"), LOGFILE);
    }
}
