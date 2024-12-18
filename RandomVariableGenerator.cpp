#include <QRandomGenerator>
#include "RandomVariableGenerator.hpp"

#include "qvector.h"




std::vector<int> RandomVariableGenerator::generateDiscrete(double p, int targetValue)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(p);

    std::vector<int> sequence;
    int value;
    do
    {
        value = dist(gen) ? targetValue : std::uniform_int_distribution<>(1, targetValue - 1)(gen);
        sequence.push_back(value);
    } while (value != targetValue);

    std::sort(sequence.begin(), sequence.end());
    return sequence;
}

std::vector<double> RandomVariableGenerator::generateContinuous(double p, int targetValue)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    std::vector<double> sequence;
    double value;
    do
    {
        value = dist(gen) * targetValue;
        sequence.push_back(value);
    } while (value < targetValue - 0.1);

    std::sort(sequence.begin(), sequence.end());
    return sequence;
}
