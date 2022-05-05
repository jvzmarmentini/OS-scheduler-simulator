#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <algorithm>

#include "Program.h"

using namespace std;

Program::Program(string path, int arrivaltime, int pid, int priority)
{
    this->arrivaltime = arrivaltime;
    this->pid = pid;
    this->priority = priority;
    mount(path);
}
void Program::ltrim(string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

bool operator>(Program const &lhs, Program const &rhs)
{
    return lhs.priority > rhs.priority;
}

void Program::mount(string path)
{
    vector<string> tmp;
    ifstream input(path);
    for (string line; getline(input, line);)
    {
        ltrim(line);
        if (string(line) == ".code" || string(line) == ".data")
        {
            tmp.clear();
            continue;
        }
        else if (string(line) == ".endcode")
        {
            for (int i = 0; i < tmp.size(); i++)
            {
                string s = tmp[i];
                if (s.find(':') != string::npos)
                {
                    labels[s.substr(0, s.find(':'))] = i;
                    s.erase(0, s.find(':') + 2);
                }
                code.push_back(s);
            }
            tmp.clear();
        }
        else if (string(line) == ".enddata")
        {
            for (string s : tmp)
            {
                string key = s.substr(0, s.rfind(' '));
                int value = stoi(s.substr(s.rfind(' ') + 1));
                datasegment[key] = value;
            }
            tmp.clear();
        }
        else
        {
            tmp.push_back(string(line));
        }
    }

    if (constants::P_DEBBUG)
    {
        std::cout << "code segment" << std::endl;
        for (string line : code)
            std::cout << line << std::endl;
        std::cout << "data segment" << std::endl;
        for (map<string, int>::iterator itr = datasegment.begin(); itr != datasegment.end(); ++itr)
            std::cout << itr->first << "=>" << itr->second << std::endl;
        std::cout << "label segment" << std::endl;
        for (map<string, int>::iterator itr = labels.begin(); itr != labels.end(); ++itr)
            std::cout << itr->first << "=>" << itr->second << std::endl;
    }
}

Operator Program::op(string com)
{
    std::transform(com.begin(), com.end(), com.begin(), ::toupper);
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
    return SYSCALL;
}

/**
 * Run the program for 1 time unit
 *
 * @return 0 if 'SYSCALL 0' was executed.
 *         1 if IO
 *         -1 otherwise
 *
 * @throw exception-object exception description
 */
int Program::run()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> gen(10, 20);
    string line = code[pc];
    string command = line.substr(0, line.find(' '));
    string target = line.substr(line.find(' ') + 1);
    int value;

    if (op(command) != BRANY || op(command) != BRPOS || op(command) != BRZERO || op(command) != BRNEG)
    {
        if (target[0] == '#')
            value = stoi(target.erase(0, 1));
        else if (isdigit(target[0]))
            value = stoi(target);
        else
            value = datasegment[target];
    }

    processingtime++;

    switch (op(command))
    {
    case ADD:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; ADD " << target << endl;
        acc += value;
        break;
    case SUB:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; SUB " << target << endl;
        acc -= value;
        break;
    case MULT:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; MULT " << target << endl;
        acc *= value;
        break;
    case DIV:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; DIV " << target << endl;
        acc /= value;
        break;
    case LOAD:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; LOAD " << target << endl;
        acc = value;
        break;
    case STORE:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; STORE " << target << endl;
        datasegment[target] = acc;
        break;
    case BRANY:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; BRANY " << target << endl;
        pc = labels[target];
        return -1;
    case BRPOS:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; BRPOS " << target << endl;
        if (acc > 0)
        {
            pc = labels[target];
            return -1;
        }
        break;
    case BRZERO:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; BRZERO " << target << endl;
        if (acc == 0)
        {
            pc = labels[target];
            return -1;
        }
        break;
    case BRNEG:
        if (constants::P_DEBBUG)
            cout << "pc: " << pc << "; BRNEG " << target << endl;
        if (acc < 0)
        {
            pc = labels[target];
            return -1;
        }
        break;
    case SYSCALL:

        if (value == 0)
        {
            if (constants::P_DEBBUG)
                cout << "pc: " << pc << "; SYSCALL " << target << endl;
            return 0;
        }
        else if (value == 1)
        {
            if (constants::P_DEBBUG)
                cout << "pc: " << pc << "; SYSCALL " << target << endl;
            std::cout << "ACC: " << acc << std::endl;
            blockedtime = gen(rng);
            pc += 1;
            return 1;
        }
        else
        {
            int tmp;
            if (constants::P_DEBBUG)
                cout << "pc: " << pc << "; SYSCALL " << target << endl;
            std::cout << "input: " << std::endl;
            cin >> tmp;
            acc = tmp;
            blockedtime = gen(rng);
            pc += 1;
            return 1;
        }
        break;
    };
    pc += 1;
    return -1;
}