#pragma once

#include "RandomVariableGenerator.hpp"
#include "ResultTable.hpp"
#include "StatisticsAndPlot.hpp"
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <random>
#include <vector>
#include <algorithm>
#include <map>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void generateSequence();

private:
    QLineEdit *probabilityInput {nullptr};
    QLineEdit *targetValueInput {nullptr};
    QTextEdit *sequenceDisplay {nullptr};
    ResultTable *resultTable {nullptr};
    RandomVariableGenerator *generator {nullptr};
    StatisticsAndPlot *statisticsAndPlot {nullptr};

    QVBoxLayout *mainLayout {nullptr};

    std::vector<int> currentSequence;

    void calculateStatisticsWrapper();
    void calculateStatistics(const std::vector<int> &sequence);
    void plotDistributionWrapper();
    void plotDistribution(const std::vector<int> &sequence);
};
