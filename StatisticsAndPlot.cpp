#include "StatisticsAndPlot.hpp"
#include <QtCharts/QLineSeries>
#include <QVBoxLayout>
#include <QMessageBox>
#include <algorithm>
#include <numeric>
#include <cmath>

using namespace QtCharts;

StatisticsAndPlot::StatisticsAndPlot(QWidget *parent) : QWidget(parent), D(0.0)
{
    // Создание основного вертикального макета
    auto *layout = new QVBoxLayout(this);

    // Виджет для отображения статистических характеристик
    statisticsDisplay = new QTextEdit();
    statisticsDisplay->setReadOnly(true);
    statisticsDisplay->setPlaceholderText("Здесь будут отображены характеристики...");
    statisticsDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    statisticsDisplay->setMaximumHeight(150);
    layout->addWidget(statisticsDisplay, 0, Qt::AlignTop);

    // Настройка графика
    chart = new QChart();
    chart->setTitle("Функции распределения");
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView, 1);

    // Настройка оси X
    axisX = new QValueAxis();
    axisX->setTitleText("Значение");
    chart->addAxis(axisX, Qt::AlignBottom);

    // Настройка оси Y
    axisY = new QValueAxis();
    axisY->setTitleText("F(x)");
    axisY->setRange(0, 1);
    chart->addAxis(axisY, Qt::AlignLeft);
}

void StatisticsAndPlot::updateStatisticsAndPlot(const std::vector<int> &sequence, double p)
{
    if (sequence.empty())
    {
        QMessageBox::warning(this, "Ошибка", "Пустая последовательность.");
        return;
    }

    // Очистка предыдущих данных
    chart->removeAllSeries();
    empiricalCDF.clear();
    theoreticalCDF.clear();
    D = 0.0;

    // Вычисление статистических характеристик
    calculateStatistics(sequence);

    // Построение функций распределения
    plotCDF(sequence, p);

    // Вычисление максимального отклонения D
    D = computeMaxDeviation();

    // Отображение значения D в статистическом окне
    statisticsDisplay->append(QString("\nМаксимальное отклонение D: %1").arg(D, 0, 'f', 4));
}

void StatisticsAndPlot::calculateStatistics(const std::vector<int> &sequence)
{
    int n = sequence.size();
    double sum = std::accumulate(sequence.begin(), sequence.end(), 0.0);
    double mean = sum / n;

    // Вычисление выборочной дисперсии
    double variance = 0.0;
    for (int val : sequence)
        variance += (val - mean) * (val - mean);
    variance /= n;

    // Сортировка для вычисления медианы
    std::vector<int> sortedSeq = sequence;
    std::sort(sortedSeq.begin(), sortedSeq.end());

    double median;
    if (n % 2 == 0)
    {
        median = (sortedSeq[n / 2 - 1] + sortedSeq[n / 2]) / 2.0;
    }
    else
    {
        median = sortedSeq[n / 2];
    }

    // Вычисление размаха выборки
    int minVal = sortedSeq.front();
    int maxVal = sortedSeq.back();
    double range = static_cast<double>(maxVal - minVal);

    // Формирование текста со статистическими характеристиками
    QString statsText = QString("Выборочные характеристики:\n")
                        + QString("Среднее (x̄): %1\n").arg(mean, 0, 'f', 4)
                        + QString("Дисперсия (S²): %1\n").arg(variance, 0, 'f', 4)
                        + QString("Медиана: %1\n").arg(median, 0, 'f', 4)
                        + QString("Размах (Rb): %1\n").arg(range, 0, 'f', 4);
    // Вывод текста в QTextEdit
    statisticsDisplay->setText(statsText);

    // Обновление диапазона осей
    axisX->setRange(minVal - 1, maxVal + 1);
    axisY->setRange(0, 1);
}

double StatisticsAndPlot::Feta(double p, int x)
{
    if (x < 0)
        return 0.0;
    return 1.0 - pow(1.0 - p, x + 1);
}

void StatisticsAndPlot::plotCDF(const std::vector<int> &sequence, double p)
{
    // Вычисление частот
    std::map<int, int> freqMap;
    for (int val : sequence)
        freqMap[val]++;

    // Извлечение отсортированных уникальных yj
    std::vector<int> uniqueYj;
    for (const auto &[key, value] : freqMap)
        uniqueYj.push_back(key);
    std::sort(uniqueYj.begin(), uniqueYj.end());

    // Определение максимального yj для диапазона x
    int max_yj = uniqueYj.empty() ? 0 : uniqueYj.back();
    int min_yj = uniqueYj.empty() ? 0 : uniqueYj.front();

    // Создание серии для эмпирической функции распределения
    QLineSeries *empiricalSeries = new QLineSeries();
    empiricalSeries->setName("Выборочная F_b(x)");

    // Создание серии для теоретической функции распределения
    QLineSeries *theoreticalSeries = new QLineSeries();
    theoreticalSeries->setName("Теоретическая F_eta(x)");

    // Определение диапазона x
    int start_x = min_yj - 1;
    int end_x = max_yj + 1;

    // Векторы для хранения CDF
    std::vector<std::pair<int, double>> empiricalCdfVector;
    std::vector<std::pair<int, double>> theoreticalCdfVector;

    // Построение эмпирической CDF
    for (int x = start_x; x <= end_x; ++x)
    {
        double F_b = 0.0;
        for (int y : uniqueYj)
        {
            if (y <= x)
                F_b += static_cast<double>(freqMap[y]) / sequence.size();
            else
                break;
        }

        empiricalCdfVector.emplace_back(x, F_b);
    }

    // Построение теоретической CDF
    for (int x = start_x; x <= end_x; ++x)
    {
        double F_eta = Feta(p, x);
        // Ограничение F_eta не превышать 1
        F_eta = std::min(F_eta, 1.0);
        theoreticalCdfVector.emplace_back(x, F_eta);
    }

    // Построение ступенчатой эмпирической CDF
    for (size_t i = 0; i < empiricalCdfVector.size(); ++i)
    {
        int x = empiricalCdfVector[i].first;
        double F_b = empiricalCdfVector[i].second;

        if (i == 0)
        {
            // Начальная точка
            empiricalSeries->append(x, F_b);
        }
        else
        {
            // Горизонтальная линия
            empiricalSeries->append(x, empiricalCdfVector[i - 1].second);
            // Вертикальный скачок
            empiricalSeries->append(x, F_b);
        }

        // Добавление точек в CDF для вычисления D
        empiricalCDF.emplace_back(x, F_b);
    }

    // Построение ступенчатой теоретической CDF
    for (size_t i = 0; i < theoreticalCdfVector.size(); ++i)
    {
        int x = theoreticalCdfVector[i].first;
        double F_eta = theoreticalCdfVector[i].second;

        if (i == 0)
        {
            // Начальная точка
            theoreticalSeries->append(x, F_eta);
        }
        else
        {
            // Горизонтальная линия
            theoreticalSeries->append(x, theoreticalCdfVector[i - 1].second);
            // Вертикальный скачок
            theoreticalSeries->append(x, F_eta);
        }

        // Добавление точек в CDF для вычисления D
        theoreticalCDF.emplace_back(x, F_eta);
    }

    // Добавление серий на график
    chart->addSeries(empiricalSeries);
    chart->addSeries(theoreticalSeries);

    // Привязка серий к осям
    empiricalSeries->attachAxis(axisX);
    empiricalSeries->attachAxis(axisY);
    theoreticalSeries->attachAxis(axisX);
    theoreticalSeries->attachAxis(axisY);

    // Настройка стилей линий
    QPen empiricalPen(Qt::blue);
    empiricalPen.setWidth(2);
    empiricalSeries->setPen(empiricalPen);

    QPen theoreticalPen(Qt::red);
    theoreticalPen.setWidth(2);
    theoreticalPen.setStyle(Qt::DashLine);
    theoreticalSeries->setPen(theoreticalPen);

    // Отображение легенды
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}

double StatisticsAndPlot::computeMaxDeviation()
{
    double maxDev = 0.0;
    size_t empSize = empiricalCDF.size();
    size_t theoSize = theoreticalCDF.size();

    // Предполагается, что empSize и theoSize совпадают
    size_t size = std::min(empSize, theoSize);
    for (size_t i = 0; i < size; ++i)
    {
        double dev = std::abs(empiricalCDF[i].second - theoreticalCDF[i].second);
        if (dev > maxDev)
            maxDev = dev;
    }

    return maxDev;
}
