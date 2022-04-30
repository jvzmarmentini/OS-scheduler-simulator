#include <iostream>
#include <deque>
#include "Program.h"
#ifndef SCHEDULER_H
#define SCHEDULER_H

namespace constants
{
    const bool S_DEBBUG = true;
}

class Scheduler
{
public:
    Scheduler(std::string policy);
    std::deque<Program>& getQnew() {return qnew;};
    std::deque<Program>& getQready() {return qready;};
    std::deque<Program>& getQrunning() {return qrunning;};
    std::deque<Program>& getQblocked() {return qblocked;};
    std::deque<Program>& getQexit() {return qexit;};
    void spawn(Program p);
    void admit(int currenttime);
    void dispatch();
    void timeout();
    void eventwait();
    void eventoccurs();
    void release();

    void printQueue(std::deque<Program> q);

private:
    std::string policy;
    std::deque<Program> qnew;
    std::deque<Program> qready;
    std::deque<Program> qrunning;
    std::deque<Program> qblocked;
    std::deque<Program> qexit;
};

#endif
