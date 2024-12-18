#include "mainwindow.hpp"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);

    // Input fields for parameters
    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Введите вероятность (p):"));
    probabilityInput = new QLineEdit();
    inputLayout->addWidget(probabilityInput);

    inputLayout->addWidget(new QLabel("Введите значение окончания последовательности:"));
    targetValueInput = new QLineEdit();
    inputLayout->addWidget(targetValueInput);

    mainLayout->addLayout(inputLayout);

    // Buttons for generating random variables
    auto *buttonLayout = new QHBoxLayout();
    auto *generateDiscreteButton = new QPushButton("Генерировать (Дискретная С.В.)");
    buttonLayout->addWidget(generateDiscreteButton);
    connect(generateDiscreteButton, &QPushButton::clicked, this, &MainWindow::generateDiscrete);

    auto *generateContinuousButton = new QPushButton("Генерировать (Непрерывная С.В.)");
    buttonLayout->addWidget(generateContinuousButton);
    connect(generateContinuousButton, &QPushButton::clicked, this, &MainWindow::generateContinuous);

    mainLayout->addLayout(buttonLayout);

    // Table for displaying results
    resultTable = new ResultTable(this);
    mainLayout->addWidget(resultTable);

    generator = new RandomVariableGenerator(this);
}

void MainWindow::generateDiscrete()
{
    double p = probabilityInput->text().toDouble();
    int targetValue = targetValueInput->text().toInt();

    if (p <= 0.0 || p > 1.0 || targetValue <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения параметров.");
        return;
    }

    auto sequence = generator->generateDiscrete(p, targetValue);
    resultTable->displayResults(sequence);
}

void MainWindow::generateContinuous()
{
    double p = probabilityInput->text().toDouble();
    int targetValue = targetValueInput->text().toInt();

    if (p <= 0.0 || p > 1.0 || targetValue <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения параметров.");
        return;
    }

    auto sequence = generator->generateContinuous(p, targetValue);
    resultTable->displayResults(sequence);
}
