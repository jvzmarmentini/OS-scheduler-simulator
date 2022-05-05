#include <iostream>
#include <deque>
#include <queue>
#include "Program.h"
#ifndef SCHEDULER_H
#define SCHEDULER_H

namespace constants
{
    const bool S_DEBBUG = true;
}

enum Policy
{
    PSP,
    PCP,
    RR,
    ND
};

struct arrivaltimecomparator
{
    bool operator()(Program i, Program j)
    {
        return i.getArrivaltime() > j.getArrivaltime();
    }
};
struct prioritycomparator
{
    bool operator()(Program i, Program j)
    {
        return i.getPriority() > j.getPriority();
    }
};

class Scheduler
{
public:
    Scheduler(std::string policy);
    void setQuantum(int q) { quantum = q; };

    void spawn(Program p);
    void admit(int currenttime);
    void dispatch();
    void process(int currenttime);
    void listenQblocked();
    bool isFinished();

    void printTimes();
    void printAll();

private:
    Policy policy;
    int quantum;
    int localquantum = 0;
    bool sem = false;
    std::priority_queue<Program, std::deque<Program>, arrivaltimecomparator> qnew;
    std::priority_queue<Program, std::deque<Program>, prioritycomparator> qready;
    std::deque<Program> qrunning;
    std::deque<Program> qblocked;
    std::deque<Program> qexit;

    bool preemption(Program candidate, Program current);
    void swap();
    void timeout();
    void eventwait();
    void eventoccurs();
    void release(int currenttime);
};

#endif
