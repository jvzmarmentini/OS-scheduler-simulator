#include <iostream>
#include "Scheduler.h"

Scheduler::Scheduler(std::string policy)
{
    if (policy == "PCP")
        this->policy = PCP;
    else
        this->policy = PSP;
}
Scheduler::Scheduler(std::string policy, int quantum)
{
    if (policy == "RR")
        this->policy = RR;
    this->quantum = quantum;
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
    while (true)
    {
        if (!qnew.empty())
            if (qnew.top().getArrivaltime() == currenttime)
            {
                if (constants::S_DEBBUG)
                    std::cout << "admiting pid " << qnew.top().getPid() << std::endl;
                qready.push(qnew.top());
                qnew.pop();
                continue;
            }
        return;
    }
};
void Scheduler::dispatch()
{
    if (!qready.empty())
    {
        if (qrunning.empty())
        {
            if (policy == PSP)
            {
                if (sem)
                    return;
                sem = true;
            }
            if (constants::S_DEBBUG)
                std::cout << "dispatching pid " << qready.top().getPid() << std::endl;
            qrunning.push_back(qready.top());
            return qready.pop();
        }
        if (policy == PCP && qready.top().getPriority() < qrunning.front().getPriority())
            return swap();
    }
};
void Scheduler::timeout()
{
    if (policy == RR)
    {
        if (qrunning.front().getProcessingtime() % quantum)
            return;
        qready.push(qrunning.front());
        qrunning.pop_front();
        if (constants::S_DEBBUG)
            std::cout << "timeout for pid " << qready.top().getPid() << std::endl;
    }
};
void Scheduler::listenQblocked()
{
    if (!qblocked.empty())
        for (Program &p : qblocked)
        {
            std::cout << "listen pid: " << p.getPid() << ", still has " << p.getWaitingtime() << " t.u." << std::endl;
            if (!p.getWaitingtime())
                return eventoccurs();
            p.decrementWaitingtime();
        }
}
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
        if (policy == PSP)
            qrunning.push_back(qblocked.front());
        else
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
        if (policy == PSP)
            sem = false;
        qexit.push_back(qrunning.front());
        qrunning.pop_front();
    }
};

void Scheduler::printAll()
{
    std::cout << "new: " << printPQueueA(qnew) << std::endl;
    std::cout << "ready: " << printPQueueP(qready) << std::endl;
    std::cout << "running: " << printQueue(qrunning) << std::endl;
    std::cout << "blocked: " << printQueue(qblocked) << std::endl;
    std::cout << "exit: " << printQueue(qexit) << std::endl;
}

std::string Scheduler::printQueue(std::deque<Program> q)
{
    std::string o;
    for (Program &p : q)
    {
        o += std::to_string(p.getPid()) + " ";
    }
    return o;
};
std::string Scheduler::printPQueueA(std::priority_queue<Program, std::deque<Program>, arrivaltimecomparator> q)
{
    std::string o;
    while (!q.empty())
    {
        o += std::to_string(q.top().getPid()) + " ";
        q.pop();
    }
    return o;
};
std::string Scheduler::printPQueueP(std::priority_queue<Program, std::deque<Program>, prioritycomparator> q)
{
    std::string o;
    while (!q.empty())
    {
        o += std::to_string(q.top().getPid()) + " ";
        q.pop();
    }
    return o;
};