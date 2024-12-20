#include "MainWindow.hpp"
#include "qtextedit.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Layout для ввода параметров
    auto *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("Введите вероятность (p):"));
    probabilityInput = new QLineEdit();
    inputLayout->addWidget(probabilityInput);

    inputLayout->addWidget(new QLabel("Введите значение окончания последовательности:"));
    targetValueInput = new QLineEdit();
    inputLayout->addWidget(targetValueInput);

    mainLayout->addLayout(inputLayout);

    // Кнопка для генерации последовательности
    auto *generateButton = new QPushButton("Генерировать последовательность");
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateSequence);
    mainLayout->addWidget(generateButton);

    // Layout для отображения результатов
    auto *resultLayout = new QHBoxLayout();

    // Текстовое поле для отображения последовательности
    sequenceDisplay = new QTextEdit();
    sequenceDisplay->setReadOnly(true); // Только для чтения
    sequenceDisplay->setPlaceholderText("Здесь будет отображена последовательность...");
    resultLayout->addWidget(sequenceDisplay);

    // Таблица для отображения частот
    resultTable = new ResultTable(this);
    resultLayout->addWidget(resultTable);

    mainLayout->addLayout(resultLayout);

    // Добавление виджета для характеристик и графиков
    // statisticsAndPlot = new StatisticsAndPlot(this);
    // mainLayout->addWidget(statisticsAndPlot);

    // Инициализация генератора
    generator = new RandomVariableGenerator();
}

void MainWindow::generateSequence()
{
    if (statisticsAndPlot != nullptr)
    {
        mainLayout->removeWidget(statisticsAndPlot);
        delete statisticsAndPlot;
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

    // Отображаем последовательность в QTextEdit
    QString sequenceText;
    for (int value : currentSequence)
    {
        sequenceText += QString::number(value) + " ";
    }
    sequenceDisplay->setText(sequenceText.trimmed()); // Выводим последовательность

    // Отображаем частоты в таблице
    resultTable->displayResults(currentSequence);

    // Обновляем виджет характеристик и графиков
    statisticsAndPlot->update(currentSequence);
    mainLayout->addWidget(statisticsAndPlot);
}
