#ifndef STATE_TESTS_H_
#define STATE_TESTS_H_

class OpenMM::State;

// Single Precision Tolerances
static double const DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM = 5;
static double const DEFAULT_ENERGY_TOL_KJ_PER_MOL = 10.0;

//static double const DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM = 0.3;
//static double const DEFAULT_ENERGY_TOL_KJ_PER_MOL = 3.0;

// Double Precision Tolerances
//static double const DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM = 0.251; // 0.06 kcal/mol/nm
//static double const DEFAULT_ENERGY_TOL_KJ_PER_MOL = 0.418; // 0.1 kcal/mol/nm
// Usage lets us compare the state internals of two platforms to test their differences.
namespace StateTests {

void checkForNans(const OpenMM::State& a);
void checkForDiscrepancies(const OpenMM::State &a);
void compareEnergies(const OpenMM::State& a, const OpenMM::State& b, double tolerance = DEFAULT_ENERGY_TOL_KJ_PER_MOL);
void compareForces(const OpenMM::State& a, const OpenMM::State& b, double tolerance = DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM);
void compareForcesAndEnergies(const OpenMM::State& a, const OpenMM::State &b,
                              double forceTolerance = DEFAULT_FORCE_TOL_KJ_PER_MOL_PER_NM, double energyTolerance = DEFAULT_ENERGY_TOL_KJ_PER_MOL);

}

#endif
