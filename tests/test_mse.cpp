#include <gtest/gtest.h>
#include <cmath>
#include "../shared/mseutils.h"

// ─── Базовые случаи ───────────────────────────────────────────────────────────

TEST(MSE, PerfectPredictionReturnsZero) {
    // Если предсказание совпадает с реальными значениями — MSE = 0
    std::vector<double> y     = {1.0, 2.0, 3.0};
    std::vector<double> yPred = {1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), 0.0);
}

TEST(MSE, ConstantErrorOfOne) {
    // Ошибка всегда 1: (1²+1²+1²)/3 = 1
    std::vector<double> y     = {2.0, 3.0, 4.0};
    std::vector<double> yPred = {1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), 1.0);
}

TEST(MSE, MixedErrors) {
    // (0-1)²+(2-2)²+(3-5)² = 1+0+4 = 5 → 5/3
    std::vector<double> y     = {0.0, 2.0, 3.0};
    std::vector<double> yPred = {1.0, 2.0, 5.0};
    EXPECT_NEAR(computeMSE(y, yPred), 5.0 / 3.0, 1e-10);
}

TEST(MSE, SingleElement) {
    // (3-5)²/1 = 4
    std::vector<double> y     = {3.0};
    std::vector<double> yPred = {5.0};
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), 4.0);
}

TEST(MSE, EmptyVectorsReturnZero) {
    // Пустые векторы — MSE = 0, не падает
    std::vector<double> y;
    std::vector<double> yPred;
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), 0.0);
}

// ─── Граничные случаи ────────────────────────────────────────────────────────

TEST(MSE, NegativeValues) {
    // (-1-1)²+((-3)-(-3))² = 4+0 → 4/2 = 2
    std::vector<double> y     = {-1.0, -3.0};
    std::vector<double> yPred = { 1.0, -3.0};
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), 2.0);
}

TEST(MSE, LargeValuesExactMatch) {
    // Предсказание совпадает — MSE = 0 вне зависимости от масштаба
    std::vector<double> y     = {1e6, 2e6, 3e6};
    std::vector<double> yPred = {1e6, 2e6, 3e6};
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), 0.0);
}

TEST(MSE, SizeMismatchThrows) {
    // Разные размеры — invalid_argument
    std::vector<double> y     = {1.0, 2.0};
    std::vector<double> yPred = {1.0};
    EXPECT_THROW(computeMSE(y, yPred), std::invalid_argument);
}

// ─── Точность вычисления ─────────────────────────────────────────────────────

TEST(MSE, KnownValueFourPoints) {
    // (2-1)²+(4-3)²+(6-4)²+(8-6)² = 1+1+4+4 = 10 → 10/4 = 2.5
    std::vector<double> y     = {2.0, 4.0, 6.0, 8.0};
    std::vector<double> yPred = {1.0, 3.0, 4.0, 6.0};
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), 2.5);
}

TEST(MSE, SymmetricErrors) {
    // MSE(y, yPred) == MSE(yPred, y) — квадрат симметричен
    std::vector<double> y     = {1.0, 3.0, 5.0};
    std::vector<double> yPred = {2.0, 2.0, 7.0};
    EXPECT_DOUBLE_EQ(computeMSE(y, yPred), computeMSE(yPred, y));
}

TEST(MSE, FractionalValues) {
    // (0.5-0.0)²+(1.5-1.0)² = 0.25+0.25 = 0.5 → 0.5/2 = 0.25
    std::vector<double> y     = {0.0, 1.0};
    std::vector<double> yPred = {0.5, 1.5};
    EXPECT_NEAR(computeMSE(y, yPred), 0.25, 1e-10);
}
