#include "MainWindow.hpp"
#include "qtextedit.h"
#include <QScrollArea>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Введите вероятность (p):"));
    probabilityInput = new QLineEdit();
    inputLayout->addWidget(probabilityInput);

    inputLayout->addWidget(new QLabel("Введите значение окончания последовательности:"));
    targetValueInput = new QLineEdit();
    inputLayout->addWidget(targetValueInput);

    auto *generateButton = new QPushButton("Генерировать последовательность");
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateSequence);
    inputLayout->addWidget(generateButton);

    mainLayout->addLayout(inputLayout);

    auto *resultLayout = new QHBoxLayout();

    sequenceDisplay = new QTextEdit();
    sequenceDisplay->setReadOnly(true);
    sequenceDisplay->setPlaceholderText("Здесь будет отображена последовательность...");
    sequenceDisplay->setFixedHeight(150);
    resultLayout->addWidget(sequenceDisplay, 0, Qt::AlignTop);

    resultTable = new ResultTable(this);

    QScrollArea *tableScrollArea = new QScrollArea(this);
    tableScrollArea->setWidget(resultTable);
    tableScrollArea->setWidgetResizable(true);
    tableScrollArea->setFixedHeight(150);
    resultLayout->addWidget(tableScrollArea, 0, Qt::AlignTop);

    mainLayout->addLayout(resultLayout);

    generator = new RandomVariableGenerator();
    statisticsAndPlot = nullptr;
}

void MainWindow::generateSequence()
{
    if (statisticsAndPlot != nullptr)
    {
        mainLayout->removeWidget(statisticsAndPlot);
        delete statisticsAndPlot;
        statisticsAndPlot = nullptr;
    }
    statisticsAndPlot = new StatisticsAndPlot(this);

    double p = probabilityInput->text().toDouble();
    int targetValue = targetValueInput->text().toInt();

    if (p <= 0.0 || p > 1.0 || targetValue <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения параметров.");
        return;
    }

    currentSequence = generator->generateSequence(p, targetValue);

    QString sequenceText;
    for (int value : currentSequence)
    {
        sequenceText += QString::number(value) + " ";
    }
    sequenceDisplay->setText(sequenceText.trimmed());

    resultTable->displayResults(currentSequence);

    statisticsAndPlot->update(currentSequence);
    mainLayout->addWidget(statisticsAndPlot);

}
