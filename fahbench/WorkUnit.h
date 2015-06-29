#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <vector>

#include "Utils.h"

using std::string;
using std::vector;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

class WorkUnit {

private:
    static const fs::path _data_dir;

    const string _system_fn;
    const string _integrator_fn;
    const string _state_fn;
    int _n_steps;

    const string _codename;
    string _fullname;
    string _description;


public:
    WorkUnit();
    WorkUnit(const fs::path & wu_path);


    const string & system_fn() const;
    const string & integrator_fn() const;
    const string & state_fn() const;

    int n_steps() const;

    const string & codename() const;
    const string & fullname() const;
    const string & description() const;

    static vector<WorkUnit> available_wus();

};

#endif // WORKUNIT_H
