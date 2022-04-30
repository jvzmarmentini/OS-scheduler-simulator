#include <iostream>
#include <vector>
#include <map>
#ifndef PROGRAM_H
#define PROGRAM_H

namespace constants
{
    const bool P_DEBBUG = true;
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
    int run();
    Operator op(std::string com);
    int getPid() {return pid;};
    int getPc() {return pc;};
    int getArrivaltime() {return arrivaltime;};

private:
    int pid = 0;
    int acc = 0;
    int pc = 0;
    int arrivaltime = 0;
    std::vector<std::string> code;
    std::map<std::string, int> datasegment;
    std::map<std::string, int> labels;

    void ltrim(std::string &s);
};

#endif
