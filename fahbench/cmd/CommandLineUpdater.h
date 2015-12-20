#ifndef COMMANDLINEUPDATER_H
#define COMMANDLINEUPDATER_H

#include "../Updater.h"

class CommandLineUpdater : public Updater {
public:
    void progress(int i, int numSteps, float ns_per_day) const;
    void message(std::string) const;
    void message(boost::format) const;
    bool cancelled() const;
};

#endif // COMMANDLINEUPDATER_H
