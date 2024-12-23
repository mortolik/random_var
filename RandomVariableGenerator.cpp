#include "RandomVariableGenerator.hpp"
#include <QRandomGenerator>

double RandomVariableGenerator::generateUniform()
{
    return QRandomGenerator::global()->generateDouble();
}

std::vector<int> RandomVariableGenerator::generateSequence(double p, int numberOfExperiments)
{
    std::vector<int> results;

    for (int experiment = 0; experiment < numberOfExperiments; ++experiment)
    {
        int y = 0;
        while (true)
        {
            // Генерация случайного числа
            if (generateUniform() < p)
            {
                break; // Событие произошло, игра заканчивается
            }

            y += 1;
        }
        results.push_back(y);
    }

    return results;
}
