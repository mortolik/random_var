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

class ResultTable : public QTableWidget
{
    Q_OBJECT
public:
    ResultTable(QWidget *parent = nullptr);

    void displayResults(const std::vector<int> &sequence);

    void displayResults(const std::vector<double> &sequence);
};
