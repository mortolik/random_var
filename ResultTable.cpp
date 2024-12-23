#include "ResultTable.hpp"
#include <map>
#include <QString>
#include <QTableWidgetItem>
#include <set>

ResultTable::ResultTable(QWidget *parent) : QTableWidget(parent)
{
    setColumnCount(2);
    setHorizontalHeaderLabels({"Количество шагов", "Частота"});
}

void ResultTable::displayResults(const std::vector<int> &stepsPerExperiment)
{
    setRowCount(0);
    std::set<int> uniqueSteps(stepsPerExperiment.begin(), stepsPerExperiment.end());

    std::map<int, int> frequencyMap;
    for (int steps : stepsPerExperiment)
    {
        frequencyMap[steps]++;
    }

    int totalExperiments = stepsPerExperiment.size();

    int currentRow = 0;
    for (int steps : uniqueSteps)
    {
        int frequency = frequencyMap[steps];
        double relativeFrequency = static_cast<double>(frequency) / totalExperiments;

        insertRow(currentRow);

        setItem(currentRow, 0, new QTableWidgetItem(QString::number(steps)));
        setItem(currentRow, 1, new QTableWidgetItem(QString::number(relativeFrequency, 'f', 4)));

        ++currentRow;
    }
}



