#pragma once

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

class RandomVariableGenerator : public QObject
{
    Q_OBJECT

public:
    RandomVariableGenerator(QObject *parent = nullptr) : QObject(parent) {}

    std::vector<int> generateDiscrete(double p, int targetValue);

    std::vector<double> generateContinuous(double p, int targetValue);
};
