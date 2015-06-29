#include "WorkUnit.h"
#include "Utils.h"

#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>


const fs::path WorkUnit::_data_dir =  getExecutableDir() / fs::path("../share/fahbench/workunits");


WorkUnit::WorkUnit() {
}

void WorkUnit::set_by_name(const string & sys_fn) {

}
void WorkUnit::set_n_steps(int n_steps) {

}
void WorkUnit::set_system_fn(const string & sys_fn) {

}
void WorkUnit::set_integrator_fn(const string & int_fn) {

}
void WorkUnit::set_state_fn(const string & state_fn) {

}
int WorkUnit::n_steps() const {

}
string & WorkUnit::state_fn() const {

}
string & WorkUnit::system_fn() const {

}
string & WorkUnit::integrator_fn() const {

}
string & WorkUnit::codename() const {

}
string & WorkUnit::description() const {

}
string & WorkUnit::fullname() const {

}


vector< string > WorkUnit::available_wus() {
    fs::path data_path(_data_dir);
    std::vector<std::string> wus;

    if (fs::exists(data_path)) {
        if (fs::is_directory(data_path)) {
            for (auto p = fs::directory_iterator(data_path); p != fs::directory_iterator(); ++p) {
                if (fs::is_directory(p->path())) {
                    wus.push_back(p->path().filename().native());
                }
            }
        } else
            throw std::runtime_error(boost::str(boost::format("%1% exists, but is not a directory.") % data_path));
    } else
        throw std::runtime_error(boost::str(boost::format("%1% does not exist.") % data_path));

    return wus;

}
