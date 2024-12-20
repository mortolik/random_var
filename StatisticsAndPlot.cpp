#include "StatisticsAndPlot.hpp"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <algorithm>
#include <numeric>

StatisticsAndPlot::StatisticsAndPlot(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);

    // Добавляем текстовый виджет для характеристик
    statisticsDisplay = new QTextEdit();
    statisticsDisplay->setReadOnly(true);
    statisticsDisplay->setPlaceholderText("Здесь будут отображены характеристики...");
    layout->addWidget(statisticsDisplay);

    // Добавляем виджет для графика
    chart = new QtCharts::QChart();
    chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout->addWidget(chartView);

    // Устанавливаем начальные оси для графика
    axisX = new QtCharts::QValueAxis();
    axisX->setTitleText("Значение");
    chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QtCharts::QValueAxis();
    axisY->setTitleText("Частота");
    chart->addAxis(axisY, Qt::AlignLeft);
}

void StatisticsAndPlot::update(const std::vector<int> &sequence)
{
    if (sequence.empty())
    {
        QMessageBox::warning(this, "Ошибка", "Пустая последовательность.");
        return;
    }

    // Удаляем старый график
    chart->removeAllSeries();

    calculateStatistics(sequence);
    plotDistribution(sequence);
}

void StatisticsAndPlot::calculateStatistics(const std::vector<int> &sequence)
{
    double sum = std::accumulate(sequence.begin(), sequence.end(), 0.0);
    int n = sequence.size();
    int maxVal = *std::max_element(sequence.begin(), sequence.end());
    int minVal = *std::min_element(sequence.begin(), sequence.end());

    // Выборочное среднее
    double mean = sum / n;

    // Дисперсия
    double variance = 0;
    for (int val : sequence)
        variance += (val - mean) * (val - mean);
    variance /= n;

    // Медиана
    std::vector<int> sortedSequence = sequence;
    std::sort(sortedSequence.begin(), sortedSequence.end());
    double median = n % 2 == 0 ?
                        (sortedSequence[n / 2 - 1] + sortedSequence[n / 2]) / 2.0
                               : sortedSequence[n / 2];

    // Размах выборки
    double range = maxVal - minVal;

    // Отображение результатов
    statisticsDisplay->setText(
        QString("Среднее: %1\nДисперсия: %2\nМедиана: %3\nРазмах: %4")
            .arg(mean)
            .arg(variance)
            .arg(median)
            .arg(range));

    // Обновление диапазонов осей графика
    axisX->setRange(minVal, maxVal);
    axisY->setRange(0, 1); // Для вероятностей максимум равен 1
}

void StatisticsAndPlot::plotDistribution(const std::vector<int> &sequence)
{
    // Удаляем предыдущие данные из графика
    chart->removeAllSeries();

    auto *theoreticalSeries = new QtCharts::QLineSeries();
    auto *empiricalSeries = new QtCharts::QLineSeries();

    // Теоретическая функция (пример для равномерного распределения)
    int minVal = *std::min_element(sequence.begin(), sequence.end());
    int maxVal = *std::max_element(sequence.begin(), sequence.end());
    for (int i = minVal; i <= maxVal; ++i)
    {
        double x = static_cast<double>(i - minVal) / (maxVal - minVal);
        theoreticalSeries->append(i, x);
    }

    // Выборочная функция
    std::map<int, int> freqMap;
    for (int val : sequence)
        freqMap[val]++;

    int cumulative = 0;
    for (const auto &[value, freq] : freqMap)
    {
        cumulative += freq;
        empiricalSeries->append(value, static_cast<double>(cumulative) / sequence.size());
    }

    chart->addSeries(theoreticalSeries);
    chart->addSeries(empiricalSeries);

    theoreticalSeries->attachAxis(axisX);
    theoreticalSeries->attachAxis(axisY);
    empiricalSeries->attachAxis(axisX);
    empiricalSeries->attachAxis(axisY);

    chart->setTitle("Функция распределения");
}
