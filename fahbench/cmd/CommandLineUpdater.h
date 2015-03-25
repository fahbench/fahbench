#ifndef COMMANDLINEUPDATER_H
#define COMMANDLINEUPDATER_H

#include "../Updater.h"

class CommandLineUpdater : public Updater
{
public:
    void progress(int i);
    void message(std::string);
    void message(boost::format);
};

#endif // COMMANDLINEUPDATER_H
