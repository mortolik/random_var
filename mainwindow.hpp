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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void generateSequence();

private:
    QVBoxLayout *mainLayout;
    QLineEdit *probabilityInput; // Для p
    QLineEdit *targetValueInput;
    QTextEdit *sequenceDisplay;
    ResultTable *resultTable;
    RandomVariableGenerator *generator;
    StatisticsAndPlot *statisticsAndPlot;
    std::vector<int> currentSequence;
};
