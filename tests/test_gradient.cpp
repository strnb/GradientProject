#include <gtest/gtest.h>
#include <cmath>
#include "../shared/gradient.h"

// ─── computeGradient ─────────────────────────────────────────────────────────

TEST(ComputeGradient, X2AtZero) {
    EXPECT_DOUBLE_EQ(computeGradient("x^2", 0.0), 0.0);
}

TEST(ComputeGradient, X2AtPositive) {
    EXPECT_DOUBLE_EQ(computeGradient("x^2", 3.0), 6.0);
}

TEST(ComputeGradient, X2AtNegative) {
    EXPECT_DOUBLE_EQ(computeGradient("x^2", -5.0), -10.0);
}

TEST(ComputeGradient, SinAtZero) {
    EXPECT_DOUBLE_EQ(computeGradient("sin", 0.0), cos(0.0));
}

TEST(ComputeGradient, SinAtPiOver2) {
    EXPECT_NEAR(computeGradient("sin", M_PI / 2), cos(M_PI / 2), 1e-10);
}

TEST(ComputeGradient, UnknownFunctionReturnsZero) {
    EXPECT_DOUBLE_EQ(computeGradient("unknown_func", 42.0), 0.0);
}

// ─── gradientDescent ─────────────────────────────────────────────────────────

TEST(GradientDescent, X2ConvergesToZero) {
    // f(x) = x^2 имеет минимум в x=0
    double result = gradientDescent("x^2", 10.0, 0.1, 1000);
    EXPECT_NEAR(result, 0.0, 1e-6);
}

TEST(GradientDescent, X2FromNegativeStart) {
    double result = gradientDescent("x^2", -8.0, 0.1, 1000);
    EXPECT_NEAR(result, 0.0, 1e-6);
}

TEST(GradientDescent, ZeroIterationsReturnsStart) {
    double result = gradientDescent("x^2", 5.0, 0.1, 0);
    EXPECT_DOUBLE_EQ(result, 5.0);
}

TEST(GradientDescent, OneIterationCorrect) {
    // x_new = x - lr * grad = 4.0 - 0.1 * 8.0 = 3.2
    double result = gradientDescent("x^2", 4.0, 0.1, 1);
    EXPECT_NEAR(result, 3.2, 1e-10);
}

TEST(GradientDescent, UnknownFuncGradIsZeroStartUnchanged) {
    // grad=0 → x не меняется
    double result = gradientDescent("unknown", 7.0, 0.5, 100);
    EXPECT_DOUBLE_EQ(result, 7.0);
}

// ─── gradientPath ────────────────────────────────────────────────────────────

TEST(GradientPath, SizeIsIterationsPlusOne) {
    auto path = gradientPath("x^2", 1.0, 0.1, 5);
    EXPECT_EQ(path.size(), 6u);
}

TEST(GradientPath, FirstElementIsStart) {
    auto path = gradientPath("x^2", 3.5, 0.1, 10);
    EXPECT_DOUBLE_EQ(path[0], 3.5);
}

TEST(GradientPath, ZeroIterationsContainsOnlyStart) {
    auto path = gradientPath("x^2", 2.0, 0.1, 0);
    ASSERT_EQ(path.size(), 1u);
    EXPECT_DOUBLE_EQ(path[0], 2.0);
}

TEST(GradientPath, PathDecreases_X2_PositiveStart) {
    auto path = gradientPath("x^2", 5.0, 0.1, 20);
    // Каждый следующий |x| должен быть <= предыдущего
    for (size_t i = 1; i < path.size(); ++i) {
        EXPECT_LE(std::abs(path[i]), std::abs(path[i - 1]) + 1e-12);
    }
}

TEST(GradientPath, LastElementMatchesGradientDescent) {
    double lr = 0.05;
    int iters = 50;
    double start = 8.0;
    auto path = gradientPath("x^2", start, lr, iters);
    double direct = gradientDescent("x^2", start, lr, iters);
    EXPECT_NEAR(path.back(), direct, 1e-12);
}
