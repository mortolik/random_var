#include "RandomVariableGenerator.hpp"

double RandomVariableGenerator::generateUniform()
{
    static unsigned int seed = 123456789; // Фиксированный seed для воспроизводимости
    seed = (1103515245 * seed + 12345) % (1U << 31); // Линейный конгруэнтный метод
    return static_cast<double>(seed) / (1U << 31);
}

std::vector<int> RandomVariableGenerator::generateSequence(double p, int targetValue)
{
    std::vector<int> sequence;
    int count = 0;

    while (true)
    {
        count++;
        double u = generateUniform();

        if (u < p)
        {
            sequence.push_back(targetValue);
            break;
        }
        else
        {
            sequence.push_back(count);
        }
    }

    return sequence;
}
