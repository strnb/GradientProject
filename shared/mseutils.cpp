#include "mseutils.h"

double computeMSE(
    const std::vector<double>& y,
    const std::vector<double>& yPred)
{
    if (y.size() != yPred.size())
        throw std::invalid_argument("y and yPred must have the same size");

    if (y.empty())
        return 0.0;

    double sum = 0.0;
    for (size_t i = 0; i < y.size(); i++)
    {
        double diff = y[i] - yPred[i];
        sum += diff * diff;
    }
    return sum / static_cast<double>(y.size());
}
