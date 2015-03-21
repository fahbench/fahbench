
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include "FAHBenchVersion.h"
#include "SimulationWorker.h"
#include "GPUInfo.h"

namespace po = boost::program_options;
namespace mm = OpenMM;
using std::string;
using std::map;

const static string openmm_data_dir = "openmm_data/";

string getGpuDesc() {
    try {
        std::stringstream ss;

        // OpenCL
        map<string, OpenCLId> opencl_devices = GPUInfo::getOpenCLDevices();
        for (auto const & kv : opencl_devices){
            ss << boost::format("OpenCL Device:\t%1%\tDevice id: %3%\tPlatform id: %2%") % kv.first % kv.second.platformId % kv.second.deviceId << std::endl;
        }

        // Cuda
        map<string, int> cuda_devices = GPUInfo::getCUDADevices();
        for (auto const & kv : cuda_devices){
            ss << boost::format("CUDA Device:\t%1%\tDevice id: %2%") % kv.first % kv.second << std::endl;
        }

        return ss.str();
    } catch(const std::exception & err) {
        std::stringstream ss;
        ss << err.what() << std::endl;
        return ss.str();
    }
}

int main(int argc, char **argv) {
    Simulation simulation;

    po::options_description desc("FAHBench options");
    desc.add_options()
    ("help,h", "produce help message")
    ("version,v", "version information")
    ("device-id", po::value<int>()->default_value(0), "GPU Device index")
    ("platform", po::value<string>(&simulation.platform)->default_value("OpenCL"), "Platform name (OpenCL or CUDA)")
    ("platform-id", po::value<int>()->default_value(0), "Platform index (OpenCL only)")
    ("precision", po::value<string>(&simulation.precision)->default_value("single"), "Precision (single or double)")
    ("system", po::value<string>(&simulation.sysFile)->default_value(""), "Path to system xml file")
    ("state", po::value<string>(&simulation.stateFile)->default_value(""), "Path to state xml file")
    ("integrator", po::value<string>(&simulation.integratorFile)->default_value(""), "Path to integrator xml file")
    ("steps", po::value<int>(&simulation.numSteps)->default_value(9000), "Number of steps to take")
    ("solvent", po::value<string>()->default_value("explicit"), "Use explicit or implicit solvent")
    ("disable-accuracy-check", "Don't check against the reference platform")
    ("device-info,d", "List GPU device information")
    ;

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (const po::error & e) {
        std::cerr << "Invalid command line arguments:" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("version")){
        std::cout << "FAHBench version " << getVersion() << std::endl;
        std::cout << "OpenMM version " << mm::Platform::getOpenMMVersion() << std::endl;
        return 1;
    }

    if (vm.count("device-info")){
        std::cout << "Available devices" << std::endl;
        std::cout << "-----------------" << std::endl << std::endl;
        std::cout << getGpuDesc() << std::endl;
        return 1;
    }

    if (vm.count("disable-accuracy-check")) {
        simulation.verifyAccuracy = false;
    }



    SimulationWorker sWorker;
    if(simulation.sysFile != "" && simulation.stateFile != "") {
        std::cout << "Custom run: " << std::endl;
        std::cout << simulation.sysFile << " " << simulation.stateFile << std::endl;
        sWorker.startSimulation(simulation);
    } else {
        if(vm["solvent"].as<string>() == "explicit") {
            std::cout << "Explicit: " << std::endl; // TODO: Move logic
            std::cout << simulation.numSteps << " steps" << std::endl;
            simulation.sysFile = openmm_data_dir + "dhfr.system.explicit.xml";
            simulation.stateFile = openmm_data_dir + "dhfr.state.explicit.xml";
            simulation.integratorFile = openmm_data_dir + "dhfr.integrator.explicit.xml";
            sWorker.startSimulation(simulation);
        }
        if(vm["solvent"].as<string>() == "implicit") {
            std::cout << "Implicit: " << std::endl;
            simulation.numSteps = NUMSTEPSIMPLICIT; // TODO: move this logic
            simulation.sysFile = openmm_data_dir + "dhfr.system.implicit.xml";
            simulation.stateFile = openmm_data_dir + "dhfr.state.implicit.xml";
            simulation.integratorFile = openmm_data_dir + "dhfr.integrator.implicit.xml";
            sWorker.startSimulation(simulation);

        }
    }

}
