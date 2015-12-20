#ifndef SIMULATIONRESULT_H
#define SIMULATIONRESULT_H

#include "DLLDefines.h"

enum class ResultStatus {
    PENDING,
    QUEUED,
    INPROGRESS,
    FAILED,
    CANCELLED,
    FINISHED
};

class FAHBENCH_EXPORT SimulationResult {
private:
    float _score;
    int _n_atoms;
    static const int dhfr_n_atoms = 23558;
    ResultStatus _status;
public:

    SimulationResult(ResultStatus status = ResultStatus::PENDING);
    SimulationResult(float score, int n_atoms, ResultStatus status = ResultStatus::FINISHED);
    float score() const;
    float scaled_score() const;
    int n_atoms() const;
    ResultStatus status() const;
};

#endif // SIMULATIONRESULT_H
