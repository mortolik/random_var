#include "RandomVariableGenerator.hpp"
#include <QRandomGenerator>

double RandomVariableGenerator::generateUniform()
{
    return QRandomGenerator::global()->generateDouble();
}


std::vector<int> RandomVariableGenerator::generateSequence(double p, int numberOfExperiments)
{
    std::vector<int> stepsPerExperiment;

    for (int experiment = 0; experiment < numberOfExperiments; ++experiment)
    {
        int count = 0;

        while (true)
        {
            count++;
            double u = generateUniform();

            if (u < p)
            {
                stepsPerExperiment.push_back(count);
                break;
            }
        }
    }

    return stepsPerExperiment;
}

