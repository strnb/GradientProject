#include "spline.h"

double splineInterpolate(
    const std::vector<double>& x,
    const std::vector<double>& y,
    double x_query
)
{
    for (size_t i = 0; i < x.size() - 1; i++)
    {
        if (x_query >= x[i] && x_query <= x[i + 1])
        {
            double t = (x_query - x[i]) / (x[i + 1] - x[i]);
            return y[i] + t * (y[i + 1] - y[i]);
        }
    }

    return 0;
}