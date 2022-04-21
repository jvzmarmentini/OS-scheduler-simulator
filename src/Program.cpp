#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Program.h"

using namespace std;

void Program::ltrim(string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

Program::Program(string path)
{
    vector<string> c;
    vector<string> d;
    vector<string> tmp;
    ifstream input(path);
    for (string line; getline(input, line);)
    {
        ltrim(line);
        if (string(line) == ".code" || string(line) == ".data")
            continue;
        else if (string(line) == ".endcode")
        {
            code = tmp;
            tmp.clear();
        }
        else if (string(line) == ".enddata")
        {
            for (auto s : tmp)
            {
                struct data tmpdata;
                tmpdata.key = s.substr(0, s.rfind(' '));
                tmpdata.value = stoi(s.substr(s.rfind(' '), s.length()));
                data.push_back(tmpdata);
            }
            tmp.clear();
        }
        else
        {
            tmp.push_back(string(line));
        }
    }

    for (string s : code)
        cout << s << endl;

    for (struct data d : data)
        cout << d.key << ' ' << d.value << endl;
}

Operator Program::op(string com)
{
    if (com == "ADD")
        return ADD;
    if (com == "SUB")
        return SUB;
    if (com == "MULT")
        return MULT;
    if (com == "DIV")
        return DIV;
    if (com == "LOAD")
        return LOAD;
    if (com == "STORE")
        return STORE;
    if (com == "BRANY")
        return BRANY;
    if (com == "BRPOS")
        return BRPOS;
    if (com == "BRZERO")
        return BRZERO;
    if (com == "BRNEG")
        return BRNEG;
    if (com == "SYSCALL")
        return SYSCALL;
}

void Program::run()
{
    // TODO: implementar modo endereçamento diretor (#1 => Variable)
    // TODO: finalizar ops
    while (true)
    {
        string line = code[pc];
        string command = line.substr(0, line.find(' '));
        switch (op(command))
        {
        case ADD:
            string value = line.substr(line.find(' ') + 1);
            if (isdigit(value))
            {
                acc += stoi(value);
            }
            else
            {
                acc += buscavalorvariavel;
            }
            pc += 1;
            break;
        case SUB:
            string value = line.substr(line.find(' ') + 1);
            if (isdigit(value))
            {
                acc -= stoi(value);
            }
            else
            {
                acc -= buscavalorvariavel;
            }
            pc += 1;
            break;
        case MULT:
            string value = line.substr(line.find(' ') + 1);
            if (isdigit(value))
            {
                acc *= stoi(value);
            }
            else
            {
                acc *= buscavalorvariavel;
            }
            pc += 1;
            break;
        case DIV:
            string value = line.substr(line.find(' ') + 1);
            if (isdigit(value))
            {
                acc /= stoi(value);
            }
            else
            {
                acc /= buscavalorvariavel;
            }
            pc += 1;
            break;
        case LOAD:

            break;
        case STORE:

            break;
        case BRANY:

            break;
        case BRPOS:

            break;
        case BRZERO:

            break;
        case BRNEG:

            break;
        case SYSCALL:

            break;
        };
    }
}
