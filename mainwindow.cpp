#include "MainWindow.hpp"
#include "StatisticsAndPlot.hpp"
#include "RandomVariableGenerator.hpp"
#include "ResultTable.hpp"
#include <QGroupBox>
#include <QTextEdit>
#include <QScrollArea>
#include <QtCharts/QChartView>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setAlignment(Qt::AlignTop);

    // Создание группы ввода основных параметров
    QHBoxLayout *groupBoxesLayout = new QHBoxLayout();
    groupBoxesLayout->setSpacing(10);
    groupBoxesLayout->setAlignment(Qt::AlignLeft);

    QGroupBox* inputGroupBox = new QGroupBox("Основные параметры", this);
    QVBoxLayout *groupBoxLayout1 = new QVBoxLayout(inputGroupBox);
    groupBoxLayout1->setSpacing(10);

    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(10);

    // Поле ввода вероятности p
    inputLayout->addWidget(new QLabel("Введите вероятность (p):"));
    probabilityInput = new QLineEdit();
    inputLayout->addWidget(probabilityInput);

    // Поле ввода количества экспериментов
    inputLayout->addWidget(new QLabel("Количество экспериментов:"));
    targetValueInput = new QLineEdit();
    inputLayout->addWidget(targetValueInput);

    // Кнопка генерации последовательности
    auto *generateButton = new QPushButton("Генерировать последовательность");
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateSequence);
    inputLayout->addWidget(generateButton);

    groupBoxLayout1->addLayout(inputLayout);
    groupBoxesLayout->addWidget(inputGroupBox, 0, Qt::AlignLeft);

    mainLayout->addLayout(groupBoxesLayout);

    // Размещение области результатов
    auto *resultLayout = new QHBoxLayout();

    // Отображение последовательности
    sequenceDisplay = new QTextEdit();
    sequenceDisplay->setReadOnly(true);
    sequenceDisplay->setPlaceholderText("Здесь будет отображена последовательность...");
    sequenceDisplay->setFixedHeight(150);
    resultLayout->addWidget(sequenceDisplay, 0, Qt::AlignTop);

    // Таблица результатов
    resultTable = new ResultTable(this);

    QScrollArea *tableScrollArea = new QScrollArea(this);
    tableScrollArea->setWidget(resultTable);
    tableScrollArea->setWidgetResizable(true);
    tableScrollArea->setFixedHeight(150);
    resultLayout->addWidget(tableScrollArea, 0, Qt::AlignTop);

    mainLayout->addLayout(resultLayout);

    // Инициализация генератора случайных величин
    generator = new RandomVariableGenerator();
    statisticsAndPlot = nullptr;
}

MainWindow::~MainWindow()
{
    delete generator;
    if (statisticsAndPlot)
        delete statisticsAndPlot;
}

void MainWindow::generateSequence()
{
    // Удаление предыдущего графика, если он существует
    if (statisticsAndPlot != nullptr)
    {
        mainLayout->removeWidget(statisticsAndPlot);
        delete statisticsAndPlot;
        statisticsAndPlot = nullptr;
    }
    statisticsAndPlot = new StatisticsAndPlot(this);

    // Получение и проверка введённых значений
    bool ok_p, ok_n;
    double p = probabilityInput->text().toDouble(&ok_p);
    int targetValue = targetValueInput->text().toInt(&ok_n);

    if (!ok_p || !ok_n || p <= 0.0 || p > 1.0 || targetValue <= 0)
    {
        QMessageBox::warning(this, "Ошибка", "Введите корректные значения параметров.");
        delete statisticsAndPlot;
        statisticsAndPlot = nullptr;
        return;
    }

    // Генерация последовательности
    currentSequence = generator->generateSequence(p, targetValue);

    // Отображение последовательности в QTextEdit
    QString sequenceText;
    for (int value : currentSequence)
    {
        sequenceText += QString::number(value) + " ";
    }
    sequenceDisplay->setText(sequenceText.trimmed());

    // Отображение результатов в таблице
    resultTable->displayResults(currentSequence);

    // Обновление статистики и построение графиков
    statisticsAndPlot->updateStatisticsAndPlot(currentSequence, p);
    mainLayout->addWidget(statisticsAndPlot);
}
