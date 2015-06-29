#include "WorkUnit.h"
#include "Utils.h"

#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>



const fs::path WorkUnit::_data_dir =  getExecutableDir() / fs::path("../share/fahbench/workunits");


WorkUnit::WorkUnit(const fs::path & wu_path)
    : _system_fn("system.xml")
    , _integrator_fn("integrator.xml")
    , _state_fn("state.xml")
    , _codename(wu_path.filename().native())

{
    auto meta_path = wu_path / "wu.json";
    pt::ptree tree;
    pt::read_json(meta_path.native(), tree);
    _fullname = tree.get<std::string>("protein.name", "No full name provided");
    _description = tree.get<std::string>("protein.description", "No description provided");
    _n_steps = tree.get<int>("steps", 1000);
}

WorkUnit::WorkUnit() {

}



int WorkUnit::n_steps() const {
    return _n_steps;
}
const string & WorkUnit::state_fn() const {
    return _state_fn;
}
const string & WorkUnit::system_fn() const {
    return _system_fn;
}
const string & WorkUnit::integrator_fn() const {
    return _integrator_fn;
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
