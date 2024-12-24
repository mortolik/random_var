#ifndef STATISTICSANDPLOT_HPP
#define STATISTICSANDPLOT_HPP

#include <QWidget>
#include <QTextEdit>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <vector>
#include <map>

class StatisticsAndPlot : public QWidget
{
public:
    explicit StatisticsAndPlot(QWidget *parent = nullptr);
    void updateStatisticsAndPlot(const std::vector<int> &sequence, double p);

private:
    // Вычисление статистических характеристик
    void calculateStatistics(const std::vector<int> &sequence);

    // Построение ступенчатых графиков функций распределения
    void plotCDF(const std::vector<int> &sequence, double p);

    // Вычисление теоретической функции распределения Fη(x)
    double Feta(double p, int x);

    // Вычисление максимального отклонения D между функциями распределения
    double computeMaxDeviation();

    QTextEdit *statisticsDisplay;              // Отображение статистических характеристик
    QtCharts::QChart *chart;                   // Объект графика
    QtCharts::QChartView *chartView;          // Виджет для отображения графика
    QtCharts::QValueAxis *axisX;               // Ось X
    QtCharts::QValueAxis *axisY;               // Ось Y

    // Хранение значений CDF для вычисления D
    std::vector<std::pair<int, double>> empiricalCDF;
    std::vector<std::pair<int, double>> theoreticalCDF;
    double D; // Максимальное отклонение

    int start_x;
    int end_x;
};
#endif // STATISTICSANDPLOT_HPP
