#include "SimulationResult.h"

#include <cmath>

int SimulationResult::n_atoms() const {
    return _n_atoms;
}
float SimulationResult::scaled_score() const {
    return _score * (_n_atoms * std::log(_n_atoms)) / (dhfr_n_atoms * std::log(dhfr_n_atoms));
}
float SimulationResult::score() const {
    return _score;
}
SimulationResult::SimulationResult(ResultStatus status)
    : _score(0.0)
    , _n_atoms(0)
    , _status(status)

{ }
SimulationResult::SimulationResult(float score, int n_atoms, ResultStatus status)
    : _score(score)
    , _n_atoms(n_atoms)
    , _status(status) {
}

ResultStatus SimulationResult::status() const {
    return _status;
}
