#include <gtest/gtest.h>
#include <cmath>
#include <vector>
#include "../shared/spline.h"

// ─── Базовая линейная интерполяция ────────────────────────────────────────────

TEST(SplineInterpolate, ExactNodeLeft) {
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 4.0};
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 0.0), 0.0);
}

TEST(SplineInterpolate, ExactNodeMiddle) {
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 4.0};
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 1.0), 1.0);
}

TEST(SplineInterpolate, ExactNodeRight) {
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 4.0};
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 2.0), 4.0);
}

TEST(SplineInterpolate, MidpointTwoPoints) {
    std::vector<double> x = {0.0, 2.0};
    std::vector<double> y = {0.0, 4.0};
    // t = (1.0-0.0)/(2.0-0.0) = 0.5 → y = 0 + 0.5*4 = 2
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 1.0), 2.0);
}

TEST(SplineInterpolate, LinearFunction) {
    // y = 3*x; точка посередине
    std::vector<double> x = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> y = {0.0, 3.0, 6.0, 9.0};
    EXPECT_NEAR(splineInterpolate(x, y, 1.5), 4.5, 1e-10);
    EXPECT_NEAR(splineInterpolate(x, y, 2.5), 7.5, 1e-10);
}

TEST(SplineInterpolate, QueryAtFirstSegmentStart) {
    std::vector<double> x = {1.0, 3.0, 5.0};
    std::vector<double> y = {2.0, 6.0, 10.0};
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 1.0), 2.0);
}

TEST(SplineInterpolate, QueryInLastSegment) {
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 0.0};
    // Сегмент [1,2]: t=0.5 → y = 1 + 0.5*(0-1) = 0.5
    EXPECT_NEAR(splineInterpolate(x, y, 1.5), 0.5, 1e-10);
}

// ─── Вне диапазона ────────────────────────────────────────────────────────────

TEST(SplineInterpolate, BelowRangeReturnsZero) {
    std::vector<double> x = {1.0, 2.0, 3.0};
    std::vector<double> y = {1.0, 4.0, 9.0};
    // x_query=0 < x[0] → ни один сегмент не подходит → 0
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 0.0), 0.0);
}

TEST(SplineInterpolate, AboveRangeReturnsZero) {
    std::vector<double> x = {0.0, 1.0, 2.0};
    std::vector<double> y = {0.0, 1.0, 4.0};
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 5.0), 0.0);
}

// ─── Специальные случаи ──────────────────────────────────────────────────────

TEST(SplineInterpolate, TwoPointsExactLeft) {
    std::vector<double> x = {0.0, 1.0};
    std::vector<double> y = {5.0, 10.0};
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 0.0), 5.0);
}

TEST(SplineInterpolate, TwoPointsExactRight) {
    std::vector<double> x = {0.0, 1.0};
    std::vector<double> y = {5.0, 10.0};
    EXPECT_DOUBLE_EQ(splineInterpolate(x, y, 1.0), 10.0);
}

TEST(SplineInterpolate, NegativeCoordinates) {
    std::vector<double> x = {-2.0, -1.0, 0.0};
    std::vector<double> y = { 4.0,  1.0, 0.0};
    // Точка середины сегмента [-2,-1]: t=0.5 → y = 4 + 0.5*(1-4) = 2.5
    EXPECT_NEAR(splineInterpolate(x, y, -1.5), 2.5, 1e-10);
}

TEST(SplineInterpolate, FractionalStep) {
    std::vector<double> x = {0.0, 0.5, 1.0};
    std::vector<double> y = {0.0, 0.25, 1.0};
    // Сегмент [0, 0.5]: t = 0.25/0.5 = 0.5 → y = 0 + 0.5*0.25 = 0.125
    EXPECT_NEAR(splineInterpolate(x, y, 0.25), 0.125, 1e-10);
}
