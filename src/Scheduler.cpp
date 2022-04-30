#include <iostream>
#include "Scheduler.h"

Scheduler::Scheduler(std::string policy){
    this->policy = policy;
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
        Program top = qnew.top();
        if (top.getArrivaltime() == currenttime)
        {
            if (constants::S_DEBBUG)
                std::cout << "admiting pid " << top.getPid() << std::endl;
            qready.push_back(qnew.top());
            qnew.pop();
        }
    }
};
void Scheduler::dispatch()
{
    if (!qready.empty())
    {
        if (constants::S_DEBBUG)
            std::cout << "dispatching pid " << qready.front().getPid() << std::endl;
        qrunning.push_back(qready.front());
        qready.pop_front();
    }
};
void Scheduler::timeout()
{
    if (constants::S_DEBBUG)
        std::cout << "timeout for pid " << qready.front().getPid() << std::endl;
    if (!qready.empty())
    {
        qrunning.push_back(qready.front());
        qready.pop_front();
    }
};
void Scheduler::eventwait()
{
    if (constants::S_DEBBUG)
        std::cout << "event started for pid " << qready.front().getPid() << std::endl;
    if (!qrunning.empty())
    {
        qblocked.push_back(qrunning.front());
        qrunning.pop_front();
    }
};
void Scheduler::eventoccurs()
{
    if (constants::S_DEBBUG)
        std::cout << "event ended for pid " << qready.front().getPid() << std::endl;
    if (!qblocked.empty())
    {
        qready.push_back(qblocked.front());
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
    for (Program &p: q) {
        std::cout << p.getPid() <<  " ";
    }
    std::cout << std::endl;
};