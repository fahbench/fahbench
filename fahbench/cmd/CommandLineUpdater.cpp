#include <iostream>
#include "CommandLineUpdater.h"

void CommandLineUpdater::progress(int i)
{
    message(boost::format("Progress: %1%") % i);
}

void CommandLineUpdater::message(std::string s)
{
    std::cout << s << std::endl;
}

void CommandLineUpdater::message(boost::format f)
{
    std::cout << f << std::endl;

}
