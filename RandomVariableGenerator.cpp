#include "RandomVariableGenerator.hpp"

double RandomVariableGenerator::generateUniform()
{
    static unsigned int seed = 123456789; // Фиксированный seed для воспроизводимости
    seed = (1103515245 * seed + 12345) % (1U << 31); // Линейный конгруэнтный метод
    return static_cast<double>(seed) / (1U << 31);
}

// Функция для генерации последовательности
std::vector<int> RandomVariableGenerator::generateSequence(double p, int targetValue)
{
    std::vector<int> sequence;
    int count = 0;

    while (true)
    {
        count++;
        double u = generateUniform(); // Генерация случайного числа из (0, 1)

        // Если число меньше p, считаем, что "успех"
        if (u < p)
        {
            sequence.push_back(targetValue); // Добавляем целевое значение
            break; // Завершаем цикл
        }
        else
        {
            sequence.push_back(count); // Добавляем текущее значение
        }
    }

    return sequence; // Возвращаем сгенерированную последовательность
}
