#include <iostream>
#include "CommandLineUpdater.h"

void CommandLineUpdater::progress(int i, int numSteps, double ns_per_day) {
    double percent = (100.0 * i) / numSteps;
    message(boost::format("Progress: %1$2d%%\testimate: %2$.2f ns/day") % percent % ns_per_day);
}


void CommandLineUpdater::message(std::string s) {
    std::cout << s << std::endl;
}

void CommandLineUpdater::message(boost::format f) {
    std::cout << f << std::endl;

}
