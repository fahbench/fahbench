#include "WorkUnit.h"
#include "Utils.h"

#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>



const fs::path WorkUnit::_data_dir =  getExecutableDir() / fs::path("../share/fahbench/workunits");


WorkUnit::WorkUnit(const fs::path & wu_path)
    : _system_fn(wu_path / "system.xml")
    , _integrator_fn(wu_path / "integrator.xml")
    , _state_fn(wu_path / "state.xml")
    , _codename(wu_path.filename().native())
    , _user_n_steps(0)

{
    auto meta_path = wu_path / "wu.json";
    pt::ptree tree;
    pt::read_json(meta_path.native(), tree);
    _fullname = tree.get<std::string>("protein.name", "No full name provided");
    _description = tree.get<std::string>("protein.description", "No description provided");
    _n_steps = tree.get<int>("steps", 1000);
}

WorkUnit::WorkUnit(const string & wu_name)
    : WorkUnit(_data_dir / fs::path(wu_name)) {
}

WorkUnit::WorkUnit(const string & system, const string & integrator, const string & state, int steps)
    : _system_fn(system)
    , _integrator_fn(integrator)
    , _state_fn(state)
    , _codename("custom")
    , _fullname("Custom WU")
    , _description("User-specified xml files.")
    , _n_steps(steps)

{ }

void WorkUnit::set_n_steps(int steps) {
    _user_n_steps = steps;
}


int WorkUnit::n_steps() const {
    if (_user_n_steps > 0) {
        return _user_n_steps;
    } else {
        return _n_steps;
    }
}

bool WorkUnit::user_n_steps() const {
    return _user_n_steps > 0;
}


const string WorkUnit::state_fn() const {
    return fs::canonical(_state_fn).string();
}
const string WorkUnit::system_fn() const {
    return fs::canonical(_system_fn).string();
}
const string WorkUnit::integrator_fn() const {
    return fs::canonical(_integrator_fn).string();
}
const string & WorkUnit::codename() const {
    return _codename;
}
const string & WorkUnit::description() const {
    return _description;
}
const string & WorkUnit::fullname() const {
    return _fullname;
}


vector<WorkUnit> WorkUnit::available_wus() {
    fs::path data_path(_data_dir);
    std::vector<WorkUnit> wus;

    if (fs::exists(data_path)) {
        if (fs::is_directory(data_path)) {
            for (auto p = fs::directory_iterator(data_path); p != fs::directory_iterator(); ++p) {
                if (fs::is_directory(p->path())) {
                    WorkUnit wu(p->path());
                    wus.push_back(wu);
                }
            }
        } else
            throw std::runtime_error(boost::str(boost::format("%1% exists, but is not a directory.") % data_path));
    } else
        throw std::runtime_error(boost::str(boost::format("%1% does not exist.") % data_path));

    return wus;

}
