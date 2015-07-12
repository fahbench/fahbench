
#include <iostream>
#include <sstream>
#include <functional>
#include <boost/program_options.hpp>
#include <boost/function.hpp>
#include <boost/format.hpp>

#include "../FAHBenchVersion.h"
#include "../Simulation.h"
#include "../SimulationResult.h"
#include "../WorkUnit.h"
#include "../GPUInfo.h"
#include "../Utils.h"
#include "CommandLineUpdater.h"

namespace po = boost::program_options;
using std::string;
using std::map;

string getGpuDesc() {
    std::stringstream ss;

    try {
        // OpenCL
        auto opencl_devices = GPUInfo::getOpenCLDevices();
        for (auto const & kv : opencl_devices) {
            ss << boost::format("OpenCL Device:\t%1%\tDevice id: %3%\tPlatform id: %2%\tPlatform version: %4%\tDevice version: %5%\t")
               % kv.device() % kv.platform_id() % kv.device_id() % kv.platform_version % kv.device_version << std::endl;
        }
    } catch (const std::exception & err) {
        ss << err.what() << std::endl;
    }

    try {
        // Cuda
        auto cuda_devices = GPUInfo::getCUDADevices();
        for (auto const & kv : cuda_devices) {
            ss << boost::format("CUDA Device:\t%1%\tDevice id: %2%") % kv.device() % kv.device_id() << std::endl;
        }
    } catch (const std::exception & err) {
        ss << err.what() << std::endl;
    }

    return ss.str();
}

string listWus() {
    std::stringstream ss;
    for (auto & wu : WorkUnit::available_wus()) {
        ss << boost::format("%1$-16s %2$-30s %3$7d %4%")
           % wu.codename() %  wu.fullname() % wu.n_steps() % wu.description() << std::endl;
    }
    return ss.str();
}

int main(int argc, char ** argv) {
    Simulation simulation;

    po::options_description desc("FAHBench options");
    desc.add_options()
    ("help,h",
     "produce help message")
    ("version,v",
     "version information")
    ("device-info,d",
     "List GPU device information")
    ("list-wus,l",
     "List available work units (WUs)")
    ("device-id",
     po::value<int>(&simulation.deviceId)->default_value(0),
     "GPU Device index")
    ("platform-id",
     po::value<int>(&simulation.platformId)->default_value(0),
     "Platform index (OpenCL only)")
    ("platform",
     po::value<string>(&simulation.platform)->default_value("OpenCL"),
     "Platform name (CPU, OpenCL, or CUDA)")
    ("precision",
     po::value<string>(&simulation.precision)->default_value("single"),
     "Precision (single or double)")
    ("workunit,w",
     po::value<string>()->default_value("dhfr"),
     "Work unit (WU) name.")
    ("system",
     po::value<string>()->default_value(""),
     "Path to system xml file")
    ("state",
     po::value<string>()->default_value(""),
     "Path to state xml file")
    ("integrator",
     po::value<string>()->default_value(""),
     "Path to integrator xml file")
    ("steps",
     po::value<int>(),
     "Number of steps to take")
    ("disable-accuracy-check",
     "Don't check against the reference platform")
    ("enable-accuracy-check",
     "Check against the reference platform (default)")
    ("nan-check",
     po::value<int>(&simulation.nan_check_freq)->default_value(0),
     "Frequency to perform NaN checks during benchmark. 0 - disable.")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error & e) {
        std::cerr << "Invalid command line arguments:" << std::endl;
        std::cerr << e.what() << std::endl;
        return -1;
    }

    int show_info_instead = 0;

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        show_info_instead += 1;
    }

    if (vm.count("version")) {
        std::cout << "FAHBench version " << getVersion() << std::endl;
        std::cout << "OpenMM version " << getOpenMMVersion() << std::endl;
        std::cout << std::endl;
        show_info_instead += 1;
    }

    if (vm.count("device-info")) {
        std::cout << "Available devices" << std::endl;
        std::cout << string(80, '-') << std::endl;
        std::cout << getGpuDesc() << std::endl;
        show_info_instead += 1;
    }

    if (vm.count("list-wus")) {
        std::cout << boost::format("%1$-16s %2$-30s %3$7d %4%")
                  % "WU" % "Full name" % "Steps" % "Description" << std::endl;
        std::cout << string(80, '-') << std::endl;
        std::cout << listWus() << std::endl;
        show_info_instead += 1;
    }

    if (show_info_instead > 0) {
        return show_info_instead;
    }

    simulation.verifyAccuracy = true; // default

    if (vm.count("disable-accuracy-check")) {
        simulation.verifyAccuracy = false;
    }

    if (vm.count("enable_accuracy-check")) {
        // override
        simulation.verifyAccuracy = true;
    }

    if (
        vm["system"].as<string>() != "" ||
        vm["integrator"].as<string>() != "" ||
        vm["state"].as<string>() != ""
    ) {
        if (!(
                    vm["system"].as<string>() != "" &&
                    vm["integrator"].as<string>() != "" &&
                    vm["state"].as<string>() != "" &&
                    vm.count("steps")
                )) {
            std::cerr << "Please specify all of system, integrator, state, steps" << std::endl;
            return -2;
        }
        simulation.work_unit = new WorkUnit(vm["system"].as<string>() ,
                                            vm["integrator"].as<string>() ,
                                            vm["state"].as<string>(),
                                            vm["steps"].as<int>()
                                           );
    } else {
        auto wu =  new WorkUnit(vm["workunit"].as<string>());
        if (vm.count("steps")) {
            wu->set_n_steps(vm["steps"].as<int>());
        }
        simulation.work_unit = wu;
    }

    std::cout << simulation.summary() << std::endl;

    CommandLineUpdater updater;
    SimulationResult result = simulation.run(updater);
    updater.message(boost::format(
                        "Final score:  %1$8.4f\n"
                        "Scaled score: %2$8.4f (%3% atoms)\n"
                    ) % result.score() % result.scaled_score() % result.n_atoms());
}
