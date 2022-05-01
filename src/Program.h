#include <iostream>
#include <vector>
#include <map>
#ifndef PROGRAM_H
#define PROGRAM_H

namespace constants
{
    const bool P_DEBBUG = false;
}

enum Operator
{
    ADD,
    SUB,
    MULT,
    DIV,
    LOAD,
    STORE,
    BRANY,
    BRPOS,
    BRZERO,
    BRNEG,
    SYSCALL
};

class Program
{
public:
    Program(std::string iofile, int arrivaltime, int pid);
    Program(std::string iofile, int arrivaltime, int pid, int priority);
    friend bool operator>(Program const &a, Program const &b);
    int getPid() const { return pid; };
    int getPc() { return pc; };
    int getPriority() const { return priority; };
    int getArrivaltime() const { return arrivaltime; };
    int run();

private:
    int pid;
    int acc = 0;
    int pc = 0;
    int priority;
    int arrivaltime;
    std::vector<std::string> code;
    std::map<std::string, int> datasegment;
    std::map<std::string, int> labels;

    void ltrim(std::string &s);
    void mount(std::string path);
    Operator op(std::string com);
};

#endif
