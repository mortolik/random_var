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

    statisticsDisplay = new QTextEdit();
    statisticsDisplay->setReadOnly(true);
    statisticsDisplay->setPlaceholderText("Здесь будут отображены характеристики...");

    statisticsDisplay->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    statisticsDisplay->setMaximumHeight(80);
    layout->addWidget(statisticsDisplay, 0, Qt::AlignTop);

    chart = new QtCharts::QChart();
    chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView, 1);

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

    double mean = sum / n;

    double variance = 0;
    for (int val : sequence)
        variance += (val - mean) * (val - mean);
    variance /= n;

    std::vector<int> sortedSequence = sequence;
    std::sort(sortedSequence.begin(), sortedSequence.end());
    double median = n % 2 == 0 ?
                        (sortedSequence[n / 2 - 1] + sortedSequence[n / 2]) / 2.0
                               : sortedSequence[n / 2];

    double range = maxVal - minVal;

    statisticsDisplay->setText(
        QString("Среднее: %1\nДисперсия: %2\nМедиана: %3\nРазмах: %4")
            .arg(mean)
            .arg(variance)
            .arg(median)
            .arg(range));

    axisX->setRange(minVal, maxVal);
    axisY->setRange(0, 1);
}

void StatisticsAndPlot::plotDistribution(const std::vector<int> &sequence)
{
    chart->removeAllSeries();

    auto *theoreticalSeries = new QtCharts::QLineSeries();
    auto *empiricalSeries = new QtCharts::QLineSeries();

    int minVal = *std::min_element(sequence.begin(), sequence.end());
    int maxVal = *std::max_element(sequence.begin(), sequence.end());
    for (int i = minVal; i <= maxVal; ++i)
    {
        double x = static_cast<double>(i - minVal) / (maxVal - minVal);
        theoreticalSeries->append(i, x);
    }
    theoreticalSeries->setName("Теоретическая функция");

    std::map<int, int> freqMap;
    for (int val : sequence)
        freqMap[val]++;

    int cumulative = 0;
    for (const auto &[value, freq] : freqMap)
    {
        cumulative += freq;
        empiricalSeries->append(value, static_cast<double>(cumulative) / sequence.size());
    }
    empiricalSeries->setName("Выборочная функция");

    chart->addSeries(theoreticalSeries);
    chart->addSeries(empiricalSeries);

    theoreticalSeries->attachAxis(axisX);
    theoreticalSeries->attachAxis(axisY);
    empiricalSeries->attachAxis(axisX);
    empiricalSeries->attachAxis(axisY);
}

