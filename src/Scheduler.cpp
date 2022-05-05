#include <iostream>
#include <deque>
#include <queue>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(string policy)
{
    if (policy == "PCP")
        this->policy = PCP;
    if (policy == "RR")
        this->policy = RR;
    if (policy == "PSP")
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
        cout << "swapping pid " << ready.getPid() << " with pid " << running.getPid() << endl;
    qready.pop();
    qrunning.pop_front();
    qready.push(running);
    qrunning.push_back(ready);
}
void Scheduler::spawn(Program p)
{
    if (constants::S_DEBBUG)
        cout << "spawning pid " << p.getPid() << endl;
    qnew.push(p);
};
void Scheduler::admit(int currenttime)
{
    while (true)
    {
        if (!qnew.empty())
            if (qnew.top().getArrivaltime() == currenttime)
            {
                if (constants::S_DEBBUG)
                    cout << "admiting pid " << qnew.top().getPid() << endl;
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
                cout << "dispatching pid " << qready.top().getPid() << endl;
            qrunning.push_back(qready.top());
            return qready.pop();
        }
        if (policy == PCP && qready.top() > qrunning.front())
            return swap();
    }
};
void Scheduler::process(int currenttime)
{
    if (qrunning.empty())
        return;
    if (constants::S_DEBBUG)
        cout << "running pid " << qrunning.front().getPid() << endl;
    int res = qrunning.front().run();

    if (policy == RR)
        localquantum++;
    if (!res)
        return release(currenttime);
    else if (res == 1)
        return eventwait();
    return timeout();
}
void Scheduler::timeout()
{
    if (policy == RR)
    {
        if (localquantum % quantum)
            return;
        localquantum = 0;
        qready.push(qrunning.front());
        qrunning.pop_front();
        if (constants::S_DEBBUG)
            cout << "timeout for pid " << qrunning.front().getPid() << endl;
    }
};
void Scheduler::listenQblocked()
{
    if (!qblocked.empty())
        for (Program &p : qblocked)
        {
            cout << "listen pid: " << p.getPid() << ", still has " << p.getBlockedtime() << " t.u." << endl;
            if (!p.getBlockedtime())
                eventoccurs();
            else
                p.decrementBlockedtime();
        }
}
void Scheduler::eventwait()
{
    if (constants::S_DEBBUG)
        cout << "event started for pid " << qrunning.front().getPid() << endl;
    if (qrunning.empty())
        return;
    if (policy == RR)
        localquantum = 0;
    qblocked.push_back(qrunning.front());
    qrunning.pop_front();
};
void Scheduler::eventoccurs()
{
    if (constants::S_DEBBUG)
        cout << "event ended for pid " << qblocked.front().getPid() << endl;
    if (qblocked.empty())
        return;
    if (policy == PSP)
        qrunning.push_back(qblocked.front());
    else
        qready.push(qblocked.front());
    qblocked.pop_front();
};
void Scheduler::release(int currenttime)
{
    if (constants::S_DEBBUG)
        cout << "releasing pid " << qrunning.front().getPid() << endl;
    if (qrunning.empty())
        return;
    if (policy == PSP)
        sem = false;
    if (policy == RR)
        localquantum = 0;
    Program p = qrunning.front();
    p.setTurnarroundtime(currenttime);
    qexit.push_back(p);
    qrunning.pop_front();
};
bool Scheduler::isFinished()
{
    return (qnew.empty() && qready.empty() && qrunning.empty() && qblocked.empty());
}

void Scheduler::printTimes()
{
    for (Program &p : qexit)
    {
        std::cout << "pid " << p.getPid() << std::endl;
        std::cout << "processing time: " << p.getProcessingtime() << std::endl;
        std::cout << "turn arround time: " << p.getTurnarroundtime() - p.getArrivaltime() << std::endl;
    }
}
void Scheduler::printAll()
{
    priority_queue<Program, deque<Program>, arrivaltimecomparator> tmpqnew = qnew;
    priority_queue<Program, deque<Program>, prioritycomparator> tmpqready = qready;
    cout << "new: ";
    while (!tmpqnew.empty())
    {
        cout << to_string(tmpqnew.top().getPid()) + " ";
        tmpqnew.pop();
    }
    cout << endl
         << "ready: ";
    while (!tmpqready.empty())
    {
        cout << to_string(tmpqready.top().getPid()) + " ";
        tmpqready.pop();
    }
    cout << endl
         << "running: ";
    for (Program &p : qrunning)
    {
        cout << to_string(p.getPid()) + " ";
    }
    cout << endl
         << "blocked: ";
    for (Program &p : qblocked)
    {
        cout << to_string(p.getPid()) + " ";
    }
    cout << endl
         << "exit: ";
    for (Program &p : qexit)
    {
        cout << to_string(p.getPid()) + " ";
    }
    cout << endl;
}