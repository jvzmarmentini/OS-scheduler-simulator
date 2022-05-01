#include <iostream>
#include <queue>
#include "Program.h"
#include "Scheduler.h"
using namespace std;

int main()
{
    int systemclock = 0;
    bool inputbreak = true;

    Scheduler scheduler("PSP");

    Program p0("../test-cases/0.txt", 5, 0, 0);
    Program p1("../test-cases/1.txt", 0, 1, 1);
    scheduler.spawn(p0);
    scheduler.spawn(p1);
    while (true)
    {
        std::cout << "system clock: " << systemclock << std::endl;
        // admit new programs based on arrival time
        scheduler.admit(systemclock);
        // if nothing is running, dispatch new program
        if (!scheduler.getQready().empty())
        {
            if (scheduler.getQrunning().empty())
                scheduler.dispatch();
            else
            {
                if (scheduler.getPolicy() == PCP)
                {
                    if (scheduler.getQready().top().getPriority() < scheduler.getQrunning().front().getPriority())
                    {
                        scheduler.swap();
                    }
                }
            }
        }

        // if program is runing, keep it running
        if (!scheduler.getQrunning().empty())
            // if program syscall 0 then releas it
            if (scheduler.getQrunning().front().run())
                scheduler.release();
        // if every work is done, stop loop
        if (scheduler.getQnew().empty() && scheduler.getQready().empty() && scheduler.getQrunning().empty() && scheduler.getQblocked().empty())
            break;
        while (inputbreak)
        {
            std::string tmp;
            std::cin >> tmp;
            if (tmp == "exit" || tmp == "e")
                return 0;
            if (tmp == "next" || tmp == "n")
                break;
            if (tmp == "finish" || tmp == "f")
                inputbreak = false;
            if (tmp == "debbug" || tmp == "d")
                scheduler.printAll();
        }
        systemclock++;
    }

    return 0;
}