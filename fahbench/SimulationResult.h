#ifndef SIMULATIONRESULT_H
#define SIMULATIONRESULT_H

class SimulationResult {
private:
    float _score;
    static const int dhfr_n_atoms = 20000; // TODO: figure out
public:
    SimulationResult(float score);
    float score() const;
    float scaled_score();
    
};

#endif // SIMULATIONRESULT_H
