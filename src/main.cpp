#include <iostream>
#include "Program.h"
#include "Scheduler.h"

int main()
{
    int systemclock = 0;
    bool inputbreak = true;

    std::cout << "scheduler policy" << std::endl
              << ": ";
    std::string policy;
    std::cin >> policy;
    Scheduler scheduler(policy);
    if (policy == "RR")
    {
        std::cout << "quantum" << std::endl
                  << ": ";
        int quantum;
        std::cin >> quantum;
        scheduler.setQuantum(quantum);
    }
    int pid = 1, arrivaltime, priority = 0;
    std::string path, opc;
    while (true)
    {
        std::cout << "program " << pid << " path" << std::endl
                  << ": ";
        std::cin >> path;
        std::cout << "arrival time" << std::endl
                  << ": ";
        std::cin >> arrivaltime;
        if (policy == "PCP")
        {
            std::cout << "priority" << std::endl
                      << ": ";
            std::cin >> priority;
        }
        Program p(path, arrivaltime, pid, priority);
        scheduler.spawn(p);
        pid++;
        std::cout << "want to add one more? (n/y) " << std::endl
                  << ": ";
        std::cin >> opc;
        if (opc == "n" || opc == "N")
            break;
    }
    while (true)
    {
        std::cout << "system clock: " << systemclock << std::endl;
        scheduler.admit(systemclock);
        scheduler.dispatch();
        scheduler.process();
        scheduler.listenQblocked();
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