#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

#include "Program.h"

using namespace std;

void Program::ltrim(string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// TODO: replace this if struct function object for find_if
int Program::getDataValueByKey(string key)
{
    for (data data : datasegment)
        if (data.key == key)
            return data.value;
    return -1;
}
int Program::setDataValueByKey(string key)
{
    for (data data : datasegment)
        if (data.key == key)
            data.value = acc;
    return -1;
}
int Program::getDataValueByPos(int pos)
{
    if (pos < datasegment.size())
        return datasegment[pos].value;
    return -1;
}

Program::Program(string path)
{
    vector<string> tmp;
    ifstream input(path);
    for (string line; getline(input, line);)
    {
        ltrim(line);
        if (string(line) == ".code" || string(line) == ".data")
            continue;
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
                struct data tmpdata;
                tmpdata.key = s.substr(0, s.rfind(' '));
                tmpdata.value = stoi(s.substr(s.rfind(' ') + 1));
                datasegment.push_back(tmpdata);
            }
            tmp.clear();
        }
        else
        {
            tmp.push_back(string(line));
        }
    }

    if (constants::DEBBUG)
    {
        std::cout << "code segment" << std::endl;
        for (string line : code)
            std::cout << line << std::endl;
        std::cout << "data segment" << std::endl;
        for (data data : datasegment)
            std::cout << data.key << "=>" << data.value << std::endl;
        std::cout << "label segment" << std::endl;
        for (map<string, int>::iterator itr = labels.begin(); itr != labels.end(); ++itr)
            std::cout << itr->first << "=>" << itr->second << std::endl;
    }
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
        string target = line.substr(line.find(' ') + 1);
        int value;

        if (op(command) != BRANY || op(command) != BRPOS || op(command) != BRZERO || op(command) != BRNEG)
        {
            if (target[0] == '#')
                value = getDataValueByPos(stoi(target.erase(0, 1)));
            else if (isdigit(target[0]))
                value = stoi(target);
            else
                value = getDataValueByKey(target);
        }

        switch (op(command))
        {
        case ADD:
            if (constants::DEBBUG)
                cout << "pc: " << pc << "; ADD " << target << endl;
            acc += value;
            break;
        case SUB:
            if (constants::DEBBUG)
                cout << "pc: " << pc << "; SUB " << target << endl;
            acc -= value;
            break;
        case MULT:
            if (constants::DEBBUG)
                cout << "pc: " << pc << "; MULT " << target << endl;
            acc *= value;
            break;
        case DIV:
            if (constants::DEBBUG)
                cout << "pc: " << pc << "; DIV " << target << endl;
            acc /= value;
            break;
        case LOAD:
            if (constants::DEBBUG)
                cout << "pc: " << pc << "; LOAD " << target << endl;
            acc = value;
            break;
        case STORE:
            if (constants::DEBBUG)
                cout << "pc: " << pc << "; STORE " << target << endl;
            setDataValueByKey(target);
            break;
        case BRANY:
            pc = labels[target];
            continue;
        case BRPOS:
            if (acc > 0)
            {
                pc = labels[target];
                continue;
            }
            break;
        case BRZERO:
            if (acc == 0)
            {
                pc = labels[target];
                continue;
            }
            break;
        case BRNEG:
            if (acc < 0)
            {
                pc = labels[target];
                continue;
            }
            break;
        case SYSCALL:
            if (value == 0)
            {
                if (constants::DEBBUG)
                    cout << "pc: " << pc << "; SYSCALL 0 " << target << endl;
                return;
            }
            else if (value == 1)
            {
                if (constants::DEBBUG)
                    cout << "pc: " << pc << "; SYSCALL 1 " << target << endl;
                std::cout << "ACC: " << acc << std::endl;
            }
            else
            {
                int tmp;
                if (constants::DEBBUG)
                    cout << "pc: " << pc << "; SYSCALL 2 " << target << endl;
                cin >> tmp;
                return;
            }
            break;
        };
        pc += 1;
    }
}