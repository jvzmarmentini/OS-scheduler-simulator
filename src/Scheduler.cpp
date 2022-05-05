#include <iostream>
#include "Scheduler.h"

using namespace std;

Scheduler::Scheduler(string policy)
{
    if (policy == "PCP")
        this->policy = PCP;
    if (policy == "RR")
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
        if (policy == PCP && qrunning.front() > qready.top())
            return swap();
    }
};
void Scheduler::process()
{
    if (qrunning.empty())
        return;
    if (constants::S_DEBBUG)
        cout << "running pid " << qrunning.front().getPid() << endl;
    int res = qrunning.front().run();

    if (policy == RR)
        localquantum++;
    if (!res)
        return release();
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
            cout << "timeout for pid " << qready.top().getPid() << endl;
    }
};
void Scheduler::listenQblocked()
{
    if (!qblocked.empty())
        for (Program &p : qblocked)
        {
            cout << "listen pid: " << p.getPid() << ", still has " << p.getBlockedtime() << " t.u." << endl;
            if (!p.getBlockedtime())
                return eventoccurs();
            p.decrementBlockedtime();
        }
}
void Scheduler::eventwait()
{
    if (constants::S_DEBBUG)
        cout << "event started for pid " << qready.top().getPid() << endl;
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
        cout << "event ended for pid " << qready.top().getPid() << endl;
    if (qblocked.empty())
        return;
    if (policy == PSP)
        qrunning.push_back(qblocked.front());
    else
        qready.push(qblocked.front());
    qblocked.pop_front();
};
void Scheduler::release()
{
    if (constants::S_DEBBUG)
        cout << "releasing pid " << qrunning.front().getPid() << endl;
    if (qrunning.empty())
        return;
    if (policy == PSP)
        sem = false;
    if (policy == RR)
        localquantum = 0;
    qexit.push_back(qrunning.front());
    qrunning.pop_front();
};

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
    cout << "new: " << printPQueueA(qnew) << endl;
    cout << "ready: " << printPQueueP(qready) << endl;
    cout << "running: " << printQueue(qrunning) << endl;
    cout << "blocked: " << printQueue(qblocked) << endl;
    cout << "exit: " << printQueue(qexit) << endl;
}

string Scheduler::printQueue(deque<Program> q)
{
    string o;
    for (Program &p : q)
    {
        o += to_string(p.getPid()) + " ";
    }
    return o;
};
string Scheduler::printPQueueA(priority_queue<Program, deque<Program>, arrivaltimecomparator> q)
{
    string o;
    while (!q.empty())
    {
        o += to_string(q.top().getPid()) + " ";
        q.pop();
    }
    return o;
};
string Scheduler::printPQueueP(priority_queue<Program, deque<Program>, prioritycomparator> q)
{
    string o;
    while (!q.empty())
    {
        o += to_string(q.top().getPid()) + " ";
        q.pop();
    }
    return o;
};