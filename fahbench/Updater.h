#ifndef UPDATER_H
#define UPDATER_H

#include <string>
#include <boost/format.hpp>

class Updater {
public:
    virtual void progress(int step, int total_step, float score) const = 0;
    virtual void message(std::string) const = 0;
    virtual void message(boost::format) const = 0;
    virtual bool cancelled() const = 0;
};

#endif // UPDATER_H
