#include <iostream>
#include <vector>
#include <map>
#ifndef PROGRAM_H
#define PROGRAM_H

namespace constants
{
    const bool DEBBUG = false;
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

struct data
{
    std::string key;
    int value;
};

class Program
{
public:
    Program(std::string iofile);
    int run();
    Operator op(std::string com);

private:
    int pid;
    int acc;
    int pc;
    std::vector<std::string> code;
    std::vector<data> datasegment;
    std::map<std::string, int> labels;

    void ltrim(std::string &s);
    int getDataValueByKey(std::string key);
    int setDataValueByKey(std::string key);
    int getDataValueByPos(int pos);
};

#endif
