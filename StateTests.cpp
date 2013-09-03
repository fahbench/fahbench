#include <math.h>
#include <OpenMM.h>
#include "StateTests.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

#include <math.h>

#ifdef _WIN32
#include <float.h>
#define isnan(x) _isnan(x)
#endif

using namespace OpenMM;
using namespace std;

void StateTests::checkForNans(const State& state) {
    const vector<Vec3> &positions = state.getPositions();
    const vector<Vec3> &velocities = state.getVelocities();
    const vector<Vec3> &forces = state.getForces();
    for(unsigned i=0;i<positions.size();i++) {
        for(int j=0;j<3;j++) {
            if(isnan(positions[i][j])) {
                stringstream ss;
                ss << i << " " << j << "\n";
                throw(std::runtime_error("NaNs detected in positions." + ss.str()));
            }
        }
    }
    for(unsigned i=0;i<velocities.size();i++) {
        for(int j=0;j<3;j++) {
            if(isnan(velocities[i][j])) {
                stringstream ss;
                ss << i << " " << j << "\n";
                throw(std::runtime_error("NaNs detected in velocities." + ss.str()));
            }
        }
    }
    for(unsigned i=0;i<forces.size();i++) {
        for(int j=0;j<3;j++) {
            if(isnan(forces[i][j])) {
                stringstream ss;
                ss << i << " " << j << "\n";
                throw(std::runtime_error("NaNs detected in forces." + ss.str()));
            }
        }
    }
}

void StateTests::checkForDiscrepancies(const State &state) {
   const vector<Vec3> &positions = state.getPositions();
    const vector<Vec3> &velocities = state.getVelocities();
    const vector<Vec3> &forces = state.getForces();
    Vec3 a,b,c;
    state.getPeriodicBoxVectors(a,b,c);
    double maxDim[3];
    maxDim[0] = a[0];
    maxDim[1] = b[1];
    maxDim[2] = c[2];
    /*
    for(unsigned i=0;i<positions.size();i++) {
        for(int j=0;j<3;j++) {
            if(fabs(positions[i][j]) > 3*maxDim[j]) {
                stringstream ss;
                ss << i << " " << j << "\n";
                throw(std::runtime_error("Discrepancy: Positions are blowing up!" + ss.str()));
            }
        }
    }
    */
    int zeroVelocityCount = 0;

    // velocities in a Boltzmann-Maxwell distribution has standard deviation 
    // equal to roughly sqrt(0.00831451121 * temperature / mass)
    // so if temperature is around 370 Kelvin, mass is 1.008 (for H, the lightest atom)
    // we have a Gaussian distribution with a standard deviation of 1.747
    // a 10 sigma events,
    // ie. 10*1.747=17.47, which should occur once in 390.68 billion times
    
    for(unsigned i=0;i<velocities.size();i++) {
        for(int j=0;j<3;j++) {
            if(fabs(velocities[i][j]) > 17.47) {
                stringstream ss;
                ss << i << " " << j << " " << velocities[i][j] << "\n";
                throw(std::runtime_error("Discrepancy: Velocities are blowing up!" + ss.str()));
            }
        }
        for(int j=0;j<3;j++) {
            if(velocities[i][j] == 0) {
                zeroVelocityCount++;
            }
        }
    }
    if(zeroVelocityCount > (3*velocities.size()/2) ) {
        throw(std::runtime_error("Discrepancy: More than half of all velocities set exactly to zero!"));
    }
    for(unsigned i=0;i<forces.size();i++) {
        for(int j=0;j<3;j++) {
            if(fabs(forces[i][j]) > 50000) {
                stringstream ss;
                ss << i << " " << j << "\n";
                throw(std::runtime_error("Discrepancy: Forces are blowing up!" + ss.str()));
            }
        }
    }
}

void StateTests::compareEnergies(const State& first, const State& b, double tolerance) {
    double potentialEnergyA = first.getPotentialEnergy();
    double potentialEnergyB = b.getPotentialEnergy();
    std::cout.precision(10);
    //cout << "Energy Reporter, Ref: " << potentialEnergyA << " " << "Core: " << potentialEnergyB << endl;
    double diff = fabs(potentialEnergyA-potentialEnergyB);
    if(diff > tolerance) {
        stringstream ss;
        ss << "Potential energy error of " << diff << ", threshold of " << tolerance << endl;
        ss << "Reference Potential Energy: " << potentialEnergyA << " | Given Potential Energy: " << potentialEnergyB << endl; 
        throw(std::runtime_error( ss.str()));
    }
    double kineticEnergyA = first.getKineticEnergy();
    double kineticEnergyB = b.getKineticEnergy();
    diff = fabs(kineticEnergyA-kineticEnergyB);
    if(diff > tolerance) {
        stringstream ss;
        ss << "Kinetic energy error of " << diff << ", threshold of " << tolerance << endl;
        ss << "Reference Kinetic Energy: " << kineticEnergyA << " | Given Kinetic Energy: " << kineticEnergyB << endl; 
        throw(std::runtime_error( ss.str()));
    }
}

void StateTests::compareForces(const State& first, const State& b, double tolerance) {
    const vector<Vec3> &forcesA = first.getForces();
    const vector<Vec3> &forcesB = b.getForces();
    int nAtoms = forcesA.size();
    double mse = 0;
    for(int i=0; i<nAtoms; i++) {
        double magnitudeA = sqrt(forcesA[i][0]*forcesA[i][0]+forcesA[i][1]*forcesA[i][1]+forcesA[i][2]*forcesA[i][2]);
        double magnitudeB = sqrt(forcesB[i][0]*forcesB[i][0]+forcesB[i][1]*forcesB[i][1]+forcesB[i][2]*forcesB[i][2]);
        double error = magnitudeA - magnitudeB;
        mse += error*error;
    }
    mse = sqrt(mse/nAtoms);
    //cout << "Force Error Reporter, MSE: " << mse << endl;
    if(mse > tolerance) {
        stringstream ss;
        ss << "Force RMSE error of " << mse << " with threshold of " << tolerance << endl;
        throw(std::runtime_error( ss.str()));
    }
}

void StateTests::compareForcesAndEnergies(const State& a, const State& b, double forceTolerance, double energyTolerance) {
    compareForces(a,b);
    compareEnergies(a,b);
}