#pragma once

#include <vector>
#include <stdexcept>

// Вычисляет Mean Squared Error между реальными значениями y и предсказанными yPred.
// Бросает std::invalid_argument, если размеры векторов не совпадают.
// Возвращает 0.0 для пустых векторов.
double computeMSE(
    const std::vector<double>& y,
    const std::vector<double>& yPred);
