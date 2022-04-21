#include <iostream>
#include <vector>
#ifndef PROGRAM_H
#define PROGRAM_H

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
    void run();
    Operator op(std::string com);

private:
    int acc;
    int pc;
    std::vector<std::string> code;
    std::vector<data> data;
    int label[50];

    void ltrim(std::string &s);
};

#endif
