#include <iostream>
#include <queue>
#include "Program.h"
#include "Scheduler.h"
using namespace std;

int main()
{
    int systemclock = 0;

    Scheduler scheduler("FIFO");

    Program p0("../test-cases/0.txt", 1, 0);
    Program p1("../test-cases/1.txt", 0, 1);
    scheduler.spawn(p0);
    scheduler.spawn(p1);
    while (true)
    {
        std::cout << "system clock: " << systemclock << std::endl;
        // admit new programs based on arrival time
        scheduler.admit(systemclock);
        // if nothing is running, dispatch new program
        if (scheduler.getQrunning().empty())
            scheduler.dispatch();
        // if program is runing, keep it running
        if (!scheduler.getQrunning().empty())
            // if program syscall 0 then releas it
            if (scheduler.getQrunning().front().run())
                scheduler.release();
        // if every work is done, stop loop
        if (scheduler.getQnew().empty() && scheduler.getQready().empty() && scheduler.getQrunning().empty() && scheduler.getQblocked().empty())
            break;
        std::string tmp;
        std::cin >> tmp; 
        systemclock++;
    }

    return 0;
}