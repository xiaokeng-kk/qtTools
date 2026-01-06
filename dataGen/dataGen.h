#pragma once
#include <vector>

class dataGen {
public:
    dataGen();
    static std::vector<double> generateSin(double amplitude, double angleDeg, double phaseDeg);
};
