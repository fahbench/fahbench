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
#include "gpuinfo.h"

#include "FAHBenchVersion.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define CL_CHECK(_expr)                                                         \
   do {                                                                         \
     cl_int _err = _expr;                                                       \
     if (_err == CL_SUCCESS)                                                    \
       break;                                                                   \
     fprintf(stderr, "OpenCL Error: '%s' returned %d!\n", #_expr, (int)_err);   \
     abort();                                                                   \
   } while (0)

using namespace OpenMM;
using namespace std;

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
}

void writeSpoiler() {	
	cout << "                                                                               " << std::endl;
	cout << "                                          O              O                     " << std::endl;
	cout << "   P R O T E N E E R     C--N              \\              \\               N    " << std::endl;
	cout << "                         |                  C              C=O           / \\-C " << std::endl;
	cout << "                         C                 /               |          N-C     \\" << std::endl;
	cout << "  .C-C                 C/                  C               C           |      C" << std::endl;
	cout << " /    \\          O     |                   |               /           N      |" << std::endl;
	cout << "C     C          |     |           O       C              C                 /-C" << std::endl;
	cout << " \\_N_/ \\   N    _C_    C           |      /         O    /                 C   " << std::endl;
	cout << "        C-/ \\_C/   \\N-/ \\    N   /-C-\\   C          |    |           O    /    " << std::endl;
	cout << "        |     |           C-/ \\C/     N-/ \\_   N\\  /C\\  -C      N    |    |    " << std::endl;
	cout << "        O     |           |    |            \\C/  C/   N/  \\_C__/ \\   C-\\  C    " << std::endl;
	cout << "              C           O    |             |   |          |     C-/   N/ \\-C" << std::endl;
	cout << "               \\_C             C             O   |          O     |          | " << std::endl;
	cout << "                  \\             \\-O              C                C          O " << std::endl;
	cout << "                  |                               \\                \\           " << std::endl;
	cout << "                  C    N         Folding@Home      C--N             C          " << std::endl;
	cout << "                   \\   |      Benchmark  (Beta)    |                |          " << std::endl;
	cout << "                    N--C                           O                |          " << std::endl;
	cout << "                        \\        Yutong Zhao                       C=O        " << std::endl;
	cout << "                         N    proteneer@gmail.com                 /           " << std::endl;
	cout << "                                                                 O            " << std::endl;
	cout << "                                   " << getVersion() << "                                      " << std::endl;
	cout << "               for official stats, please visit www.fahbench.com               " << std::endl;
    cout << "                                 OpenMM " << Platform::getOpenMMVersion() << std::endl;
	cout << endl;
}

int main(int argc, char **argv) {

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
        }else if(strcmp(argv[i],"--help") == 0) {
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
		displayCardInfo();	
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

    map<string,string> properties;
    if(simulation.platform.compare("CUDA") == 0) {
        simulation.properties["CudaPrecision"]=precision;
        simulation.properties["CudaDeviceIndex"]=deviceId;
    } else if(simulation.platform.compare("OpenCL") == 0) {
        simulation.properties["OpenCLPrecision"]=precision;
        simulation.properties["OpenCLDeviceIndex"]=deviceId;
        simulation.properties["OpenCLPlatformIndex"]=platformId;
    }

	try{
        SimulationWorker sWorker;
		if(simulation.sysFile != NULL && simulation.stateFile != NULL) {
			cout << "Custom run: " << endl;
			cout << simulation.sysFile << " " << simulation.stateFile << endl;
			sWorker.startSimulation(simulation);	
		} else {
            if(doExplicit) {
                cout << "Explicit: " << endl;
                simulation.sysFile = "./scratch/DHFR_SYSTEM_EXPLICIT.xml"; 
                simulation.stateFile = "./scratch/DHFR_STATE_EXPLICIT.xml";
                simulation.integratorFile = "./scratch/DHFR_INTEGRATOR_EXPLICIT.xml";
                sWorker.startSimulation(simulation);
            }
            if(doImplicit) {
                if(deviceId.find(",")==string::npos) {
                    cout << "Implicit: " << endl;
                    simulation.numSteps = NUMSTEPSIMPLICIT;
                    simulation.sysFile = "./scratch/DHFR_SYSTEM_IMPLICIT.xml";
                    simulation.stateFile = "./scratch/DHFR_STATE_IMPLICIT.xml";
                    simulation.integratorFile = "./scratch/DHFR_INTEGRATOR_IMPLICIT.xml";
                    sWorker.startSimulation(simulation);
                } else {
                    cout << "Implicit not supported on multiple devices." << endl;
                }
            }
		}
        
	} catch (const std::exception &e) {
		cout << e.what();
	}

}