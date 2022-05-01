#include <iostream>
#include "Scheduler.h"

Scheduler::Scheduler(std::string policy)
{
    if (policy == "PCP")
        this->policy = PCP;
    else if (policy == "RR")
        this->policy = RR;
    else
        this->policy = PSP;
}
bool Scheduler::preemption(Program candidate, Program current)
{
    return candidate.getPriority() < current.getPriority();
}
void Scheduler::swap()
{

    Program ready = qready.top();
    Program running = qrunning.front();
    if (constants::S_DEBBUG)
        std::cout << "swapping pid " << ready.getPid() << " with pid " << running.getPid() << std::endl;
    qready.pop();
    qrunning.pop_front();
    qready.push(running);
    qrunning.push_back(ready);
}
void Scheduler::spawn(Program p)
{
    if (constants::S_DEBBUG)
        std::cout << "spawning pid " << p.getPid() << std::endl;
    qnew.push(p);
    Program top = qnew.top();
};
void Scheduler::admit(int currenttime)
{
    if (!qnew.empty())
    {
        if (qnew.top().getArrivaltime() == currenttime)
        {
            if (constants::S_DEBBUG)
                std::cout << "admiting pid " << qnew.top().getPid() << std::endl;
            qready.push(qnew.top());
            qnew.pop();
        }
    }
};
void Scheduler::dispatch()
{
    if (!qready.empty())
    {
        if (constants::S_DEBBUG)
            std::cout << "dispatching pid " << qready.top().getPid() << std::endl;
        qrunning.push_back(qready.top());
        qready.pop();
    }
};
void Scheduler::timeout()
{
    if (constants::S_DEBBUG)
        std::cout << "timeout for pid " << qready.top().getPid() << std::endl;
    if (!qready.empty())
    {
        qrunning.push_back(qready.top());
        qready.pop();
    }
};
void Scheduler::eventwait()
{
    if (constants::S_DEBBUG)
        std::cout << "event started for pid " << qready.top().getPid() << std::endl;
    if (!qrunning.empty())
    {
        qblocked.push_back(qrunning.front());
        qrunning.pop_front();
    }
};
void Scheduler::eventoccurs()
{
    if (constants::S_DEBBUG)
        std::cout << "event ended for pid " << qready.top().getPid() << std::endl;
    if (!qblocked.empty())
    {
        qready.push(qblocked.front());
        qblocked.pop_front();
    }
};
void Scheduler::release()
{
    if (constants::S_DEBBUG)
        std::cout << "releasing pid " << qrunning.front().getPid() << std::endl;
    if (!qrunning.empty())
    {
        qexit.push_back(qrunning.front());
        qrunning.pop_front();
    }
};

void Scheduler::printQueue(std::deque<Program> q)
{
    for (Program &p : q)
    {
        std::cout << p.getPid() << " ";
    }
    std::cout << std::endl;
};