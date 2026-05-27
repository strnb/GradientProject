#pragma once
#include <vector>

// »нтерпол€ци€ по набору точек
double splineInterpolate(
    const std::vector<double>& x,
    const std::vector<double>& y,
    double x_query
);