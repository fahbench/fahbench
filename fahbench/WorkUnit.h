#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include <vector>

#include "Utils.h"
#include "DLLDefines.h"

using std::string;
using std::vector;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

class FAHBENCH_EXPORT WorkUnit {

private:
    static const fs::path _data_dir;

    fs::path _system_fn;
    fs::path _integrator_fn;
    fs::path _state_fn;
    int _step_chunk;

    string _codename;
    string _fullname;
    string _description;


public:
    WorkUnit(const string & system, const string & integrator, const string & state, int step_chunk);
    WorkUnit(const string & wu_name);
    WorkUnit(const fs::path & wu_path);

    int step_chunk() const;
    void set_step_chunk(const int step_chunk);

    const string system_fn() const;
    const string integrator_fn() const;
    const string state_fn() const;

    const string & codename() const;
    const string & fullname() const;
    const string & description() const;

    static vector<WorkUnit> available_wus();

};

#endif // WORKUNIT_H
