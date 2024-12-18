#pragma once

#include "RandomVariableGenerator.hpp"
#include "ResultTable.hpp"
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void generateDiscrete();

    void generateContinuous();

private:
    QLineEdit *probabilityInput;
    QLineEdit *targetValueInput;
    ResultTable *resultTable;
    RandomVariableGenerator *generator;
};
