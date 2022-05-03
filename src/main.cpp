#include <iostream>
#include "Program.h"
#include "Scheduler.h"

int main()
{
    int systemclock = 0;
    bool inputbreak = true;

    Scheduler scheduler("PSP");

    Program p0("../test-cases/0.txt", 0, 0);
    Program p1("../test-cases/1.txt", 5, 1);
    scheduler.spawn(p0);
    scheduler.spawn(p1);
    while (true)
    {
        std::cout << "system clock: " << systemclock << std::endl;
        scheduler.admit(systemclock);
        scheduler.dispatch();
        if (!scheduler.getQrunning().empty())
        {
            int res = scheduler.getQrunning().front().run();
            if (!res)
            {
                scheduler.release();
            }
            else if (res == 1)
            {
                scheduler.eventwait();
            }
            else
            {
                scheduler.timeout();
            }
        }
        scheduler.listenQblocked();
        // if program syscall 0 then releas it
        // if every work is done, stop loop
        if (scheduler.getQnew().empty() && scheduler.getQready().empty() && scheduler.getQrunning().empty() && scheduler.getQblocked().empty())
            break;
        while (inputbreak)
        {
            std::string tmp;
            std::cout << "> ";
            std::cin >> tmp;
            std::system("clear");

            if (tmp == "exit" || tmp == "e")
                return 0;
            if (tmp == "next" || tmp == "n")
                break;
            if (tmp == "finish" || tmp == "f")
                inputbreak = false;
            if (tmp == "debbug" || tmp == "d")
            {
                scheduler.printAll();
                break;
            }
        }
        systemclock++;
    }

    return 0;
}