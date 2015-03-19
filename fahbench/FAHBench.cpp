#include <OpenMM.h>
#include <openmm/serialization/XmlSerializer.h>
#include <CL/cl.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#include "SimulationWorker.h"

#include <QApplication>
#include "window/window.h"
//#include "gpuinfo.h"

#include <boost/program_options.hpp>

#include "FAHBenchVersion.h"

using namespace OpenMM;
using namespace std;
namespace po = boost::program_options;

/*
void displayCardInfo() {
    try {
        map<string, OpenCLId> ocldmap = GPUInfo::getOpenCLDevices();
        if(ocldmap.size() > 0)  {
            cout << "OpenCLDeviceName: (platformId, deviceId)" << endl;
            for(auto it=ocldmap.begin(); it!=ocldmap.end(); it++) {
                cout << it->first << ": (" << it->second.platformId << "," << it->second.deviceId << ")" << endl;
            }
        }
        cout << endl;
        map<string, int> cudamap = GPUInfo::getCUDADevices();
        if(cudamap.size() > 0)  {
            cout << "CUDADeviceName: deviceId" << endl;
            for(auto it=cudamap.begin(); it!=cudamap.end(); it++) {
                cout << it->first << ": " << it->second << endl;
            }
        }
    } catch(const std::exception &err) {
        cout << err.what() << endl;
    }
}*/

int main(int argc, char **argv) {
    Simulation simulation;

    po::options_description desc("FAHBench options");
    desc.add_options()
    ("help", "produce help message")
    ("device-id", po::value<int>()->default_value(0), "GPU Device index")
    ("platform", po::value<string>(&simulation.platform)->default_value("OpenCL"), "Platform name (OpenCL or CUDA)")
    ("platform-id", po::value<int>()->default_value(0), "Platform index (OpenCL only)")
    ("precision", po::value<string>(&simulation.precision)->default_value("single"), "Precision (single or double)")
    //("system", po::value<string>(), "System path")
    ("steps", po::value<int>(&simulation.numSteps)->default_value(9000), "Number of steps to take")
    ("solvent", po::value<string>()->default_value("explicit"), "Use explicit or implicit solvent")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    try {
        SimulationWorker sWorker;
        if(simulation.sysFile != "" && simulation.stateFile != "") {
            cout << "Custom run: " << endl;
            cout << simulation.sysFile << " " << simulation.stateFile << endl;
            sWorker.startSimulation(simulation);
        } else {
            if(vm["solvent"].as<string>() == "explicit") {
                cout << "Explicit: " << endl;
                cout << simulation.numSteps << " steps" << endl;
                simulation.sysFile = "../../openmm_data/DHFR_SYSTEM_EXPLICIT.xml";
                simulation.stateFile = "../../openmm_data/DHFR_STATE_EXPLICIT.xml";
                simulation.integratorFile = "../../openmm_data/DHFR_INTEGRATOR_EXPLICIT.xml";
                sWorker.startSimulation(simulation);
            }
            if(vm["solvent"].as<string>() == "implicit") {
                cout << "Implicit: " << endl;
                simulation.numSteps = NUMSTEPSIMPLICIT; // TODO: move this logic
                simulation.sysFile = "../../openmm_data/DHFR_SYSTEM_IMPLICIT.xml";
                simulation.stateFile = "../../openmm_data/DHFR_STATE_IMPLICIT.xml";
                simulation.integratorFile = "../../openmm_data/DHFR_INTEGRATOR_IMPLICIT.xml";
                sWorker.startSimulation(simulation);

            }
        }
    } catch (const std::exception &e) {
        cout << e.what();
    }


}

void writeSpoiler() {
    cout << "OpenMM " << Platform::getOpenMMVersion() << std::endl;
}

int old_main(int argc, char **argv) {

    // no arguments starts the GUI
    if(argc == 1) {
        // frees the Win32 console for the user
#ifdef _WIN32
        FreeConsole();
#endif
        QApplication app(argc, argv);
        Window window;
        window.show();
        return app.exec();
    }

    bool splashScreen = true;
    bool displaydevices = false;

    // otherwise we use the CLI
    Simulation simulation;

    string precision = "single";
    string deviceId;
    string platformId;

    simulation.verifyAccuracy = true;

    double stepSizeInPs = 0.002;

    simulation.numSteps = NUMSTEPSEXPLICIT;
    //simulation.numSteps = 10;

    bool doExplicit = true;
    bool doImplicit = true;

    // crude way of parsing argc, arc
    for(int i=1; i < argc; i++) {
        if(strcmp(argv[i],"-platform") == 0) {
            simulation.platform = argv[i+1];
        } else if(strcmp(argv[i],"-deviceId") == 0) {
            deviceId = argv[i+1];
        } else if(strcmp(argv[i],"-platformId") == 0) {
            platformId = argv[i+1];
        } else if(strcmp(argv[i],"-precision") == 0) {
            precision = argv[i+1];
        } else if(strcmp(argv[i],"--disable-accuracy-check") == 0) {
            simulation.verifyAccuracy = false;
        } else if(strcmp(argv[i],"--disable-splash") == 0) {
            splashScreen = false;
        } else if(strcmp(argv[i],"--display-devices") == 0) {
            displaydevices = true;
        } else if(strcmp(argv[i],"--system") == 0) {
            simulation.sysFile = argv[i+1];
        } else if(strcmp(argv[i],"--state") == 0) {
            simulation.stateFile = argv[i+1];
        } else if(strcmp(argv[i],"-steps") == 0) {
            stringstream ss;
            ss << argv[i+1];
            ss >> simulation.numSteps;
        } else if(strcmp(argv[i],"--noexplicit") == 0 ) {
            doExplicit = false;
        } else if(strcmp(argv[i],"--noimplicit") == 0 ) {
            doImplicit = false;
        } else if(strcmp(argv[i],"--help") == 0) {
            cout << "Command Line Interface Usage: " << endl;
            cout << "Required: \n"
                 << "-deviceId: " << "integer set (0-indexed), set to x,y if you want to use both device x and device y\n"
                 << "-platform: " << "CUDA or OpenCL\n"
                 << "-precision: " << "single or double\n"
                 << "Optional: \n"
                 << "-steps: " << "sets the number of steps for the simulation, defaults to 9000\n"
                 << "-platformId: " << "integer (OpenCL only) set the platform, defaults to 0 \n"
                 << "--disable-accuracy-check " << "disables accuracy check (on by default)\n"
                 << "--disable-splash " << "disables the splash text (on by default)\n"
                 << "--display-devices " << "display compatible devices (off by default)\n" << endl;
            cout << "ex1: FAHBench.exe -deviceId 0 -platform CUDA -precision single\n"
                 << "ex2: FAHBench.exe -deviceId 0,1 -platform OpenCL -platformId 0 -precision single\n" << endl;
            return 0;
        }
    }

    if(splashScreen) {
        writeSpoiler();
    }
    if(displaydevices) {
        //displayCardInfo();
    }

    //cout << platform << " " << deviceId << " " << precision << " " << verifyAccuracy << " " << splashScreen << endl;
    if(simulation.platform.compare("OpenCL") != 0 && simulation.platform.compare("CUDA") != 0) {
        cout << "Invalid Platform (please use either OpenCL or CUDA)\n";
        return 0;
    }
    if(precision.compare("single") != 0 && precision.compare("double") != 0) {
        cout << "Defaulting to single precision";
    }
    if(deviceId.size() == 0) {
        cout << "Invalid Device (please set valid deviceId 0-10)\n";
        return 0;
    }
    if((simulation.platform.compare("CUDA") == 0) && platformId.size() > 0) {
        cout << "Note: Using CUDA, -platformId ignored.\n";
    }
    cout << simulation.numSteps << " steps." << endl;

    simulation.window = NULL;



}
