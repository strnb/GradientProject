#ifndef GRADIENT_H
#define GRADIENT_H

#include <vector>
#include <string>

// �������� ������� ������������ ������
double gradientDescent(
    const std::string& func,
    double start,
    double learning_rate,
    int iterations
);

// ������� ��� ���������� ��������� (�����������)
double computeGradient(
    const std::string& func,
    double x
);

std::vector<double> gradientPath(
    const std::string& func,
    double start,
    double learning_rate,
    int iterations
);

#endif