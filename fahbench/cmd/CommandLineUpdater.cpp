#include <iostream>
#include "CommandLineUpdater.h"

void CommandLineUpdater::progress(int i, int numSteps, double ns_per_day) {
    if (i == numSteps){
        std::cout << std::string(80, ' ') << std::endl;
        return;
    }
    double percent = (100.0 * i) / numSteps;
    std::cout << boost::format("Progress: %1$5.1f%%  estimate: %2$6.2f ns/day") % percent % ns_per_day << "\r" << std::flush;
}


void CommandLineUpdater::message(std::string s) {
    std::cout << s << std::endl << std::flush;
}

void CommandLineUpdater::message(boost::format f) {
    std::cout << f << std::endl << std::flush;

}
