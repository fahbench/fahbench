#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <boost/filesystem.hpp>
#include <string>
#include <vector>

#include "Utils.h"

using std::string;
using std::vector;
namespace fs = boost::filesystem;

class WorkUnit {

private:
    static const fs::path _data_dir;

    string _system_fn;
    string _integrator_fn;
    string _state_fn;
    int _n_steps;

    string _codename;
    string _fullname;
    string _description;


public:
    WorkUnit();


    void set_by_name(const string & sys_fn);
    void set_system_fn(const string & sys_fn);
    void set_integrator_fn(const string & int_fn);
    void set_state_fn(const string & state_fn);
    void set_n_steps(int n_steps);

    string & system_fn() const;
    string & integrator_fn() const;
    string & state_fn() const;

    int n_steps() const;

    string & codename() const;
    string & fullname() const;
    string & description() const;

    vector<string> available_wus();

};

#endif // WORKUNIT_H
