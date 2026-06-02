#include "gradient.h"
#include <cmath>

double computeGradient(const std::string& func, double x) {
    if (func == "x^2")
        return 2 * x;

    if (func == "sin")
        return cos(x);

    return 0;
}

double gradientDescent(const std::string& func, double start, double learning_rate, int iterations) {
    double x = start;

    for (int i = 0; i < iterations; i++)
    {
        double grad = computeGradient(func, x);
        x = x - learning_rate * grad;
    }

    return x;
}

std::vector<double> gradientPath(
    const std::string& func,
    double start,
    double learning_rate,
    int iterations)
{
    std::vector<double> path;

    double x = start;

    path.push_back(x);

    for (int i = 0; i < iterations; i++)
    {
        double grad =
            computeGradient(
                func,
                x
            );

        x =
            x -
            learning_rate * grad;

        path.push_back(x);
    }

    return path;
}