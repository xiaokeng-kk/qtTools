#include "dataGen.h"
#include <vector>
#include <cmath>

static const double PI = 3.14159265358979323846;

dataGen::dataGen()
{
}

std::vector<double> dataGen::generateSin(double amplitude, double angleDeg, double phaseDeg)
{
    const int N = 1024;
    std::vector<double> out;
    out.reserve(N);
    for (int i = 0; i < N; ++i) {
        double deg = i * angleDeg + phaseDeg;
        double rad = deg * PI / 180.0;
        out.push_back(amplitude * std::sin(rad));
    }
    return out;
}
