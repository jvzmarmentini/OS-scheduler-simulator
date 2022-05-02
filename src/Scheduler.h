#include <iostream>
#include <deque>
#include <queue>
#include <vector>
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
    Scheduler(std::string policy, int quantum);
    std::priority_queue<Program, std::deque<Program>, arrivaltimecomparator> &getQnew() { return qnew; };
    std::priority_queue<Program, std::deque<Program>, prioritycomparator> &getQready() { return qready; };
    std::deque<Program> &getQrunning() { return qrunning; };
    std::deque<Program> &getQblocked() { return qblocked; };
    std::deque<Program> &getQexit() { return qexit; };
    Policy getPolicy() { return policy; };
    bool preemption(Program candidate, Program current);
    void swap();
    void spawn(Program p);
    void admit(int currenttime);
    void dispatch();
    void timeout();
    void listenQblocked();
    void eventwait();
    void eventoccurs();
    void release();

    void printAll();
    std::string printQueue(std::deque<Program> q);
    std::string printPQueueA(std::priority_queue<Program, std::deque<Program>, arrivaltimecomparator> q);
    std::string printPQueueP(std::priority_queue<Program, std::deque<Program>, prioritycomparator> q);

private:
    Policy policy;
    int quantum = 0;
    std::priority_queue<Program, std::deque<Program>, arrivaltimecomparator> qnew;
    std::priority_queue<Program, std::deque<Program>, prioritycomparator> qready;
    std::deque<Program> qrunning;
    std::deque<Program> qblocked;
    std::deque<Program> qexit;
};

#endif
