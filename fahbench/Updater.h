#ifndef UPDATER_H
#define UPDATER_H

#include <string>
#include <boost/format.hpp>

class Updater {
public:
    virtual void progress(int) = 0;
    virtual void message(std::string) = 0;
    virtual void message(boost::format) = 0;
};

#endif // UPDATER_H
