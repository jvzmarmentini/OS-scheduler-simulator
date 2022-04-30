#include <iostream>
#include <queue>
#include "Program.h"
#include "Scheduler.h"
using namespace std;

int main()
{
    int systemclock = 0;

    Scheduler scheduler("FIFO");

    Program p0("../test-cases/0.txt", 0, 0);
    Program p1("../test-cases/1.txt", 10, 1);
    scheduler.spawn(p0);
    scheduler.spawn(p1);
    while (true)
    {
        std::cout << "clock: " << systemclock << std::endl;
        scheduler.admit(systemclock);
        scheduler.dispatch();
        if (!scheduler.getQrunning().empty())
        {
            if (scheduler.getQrunning().front().run())
                scheduler.release();
        }
        if (scheduler.getQnew().empty() && scheduler.getQready().empty() && scheduler.getQrunning().empty() && scheduler.getQblocked().empty())
            break;
        // std::string tmp;
        // std::cin >> tmp; 
        systemclock++;
    }

    return 0;
}