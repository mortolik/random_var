#pragma once
#include "qvalueaxis.h"
#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
QT_FORWARD_DECLARE_CLASS(QTextEdit);

using namespace QtCharts;

class StatisticsAndPlot : public QWidget
{
public:
    StatisticsAndPlot(QWidget *parent);
    void update(const std::vector<int> &sequence);
private:
    void calculateStatistics(const std::vector<int> &sequence);
    void plotDistribution(const std::vector<int> &sequence);
    QTextEdit* statisticsDisplay;
    QChart* chart {nullptr};
    QChartView* chartView {nullptr};
    QtCharts::QValueAxis *axisX;
    QtCharts::QValueAxis *axisY;
};

