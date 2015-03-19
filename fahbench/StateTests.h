/******************************************************************************\

                    Copyright 2013-2015. Stanford University.
                              All Rights Reserved.

                                Original Author
                         Yutong Zhao <proteneer@gmail.com>

\******************************************************************************/

#ifndef STATE_TESTS_H
#define STATE_TESTS_H

namespace OpenMM {
class State;
}


// Single Precision Tolerances
static double const DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM = 5;
static double const DEFAULT_ENERGY_TOL_KJ_PER_MOL = 10.0;

// Usage lets us compare the state internals of two platforms to test their
// differences.
namespace StateTests {
void checkForNans(const OpenMM::State &a);
void checkForDiscrepancies(const OpenMM::State &a);
void compareEnergies(const OpenMM::State &a, const OpenMM::State &b,
                     double tolerance = DEFAULT_ENERGY_TOL_KJ_PER_MOL);
void compareForces(const OpenMM::State &a, const OpenMM::State &b,
                   double tolerance = DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM);
void compareForcesAndEnergies
(const OpenMM::State &a, const OpenMM::State &b,
 double forceTolerance = DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM,
 double energyTolerance = DEFAULT_ENERGY_TOL_KJ_PER_MOL);
}

#endif // STATE_TESTS_H
