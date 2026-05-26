#ifndef GRADIENT_H
#define GRADIENT_H

#include <string>

// Основная функция градиентного спуска
double gradientDescent(
    const std::string& func,
    double start,
    double learning_rate,
    int iterations
);

// Функция для вычисления градиента (производной)
double computeGradient(
    const std::string& func,
    double x
);

#endif