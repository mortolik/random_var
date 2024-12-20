#include "ResultTable.hpp"
#include <map>
#include <QString>
#include <QTableWidgetItem>

ResultTable::ResultTable(QWidget *parent) : QTableWidget(parent)
{
    setColumnCount(2);
    setHorizontalHeaderLabels({"Значение", "Частота"});
}

void ResultTable::displayResults(const std::vector<int> &sequence)
{
    setRowCount(0);

    std::map<int, int> frequencyMap;
    for (int value : sequence)
    {
        frequencyMap[value]++;
    }

    int row = 0;
    for (const auto &[value, frequency] : frequencyMap)
    {
        insertRow(row);
        setItem(row, 0, new QTableWidgetItem(QString::number(value)));
        setItem(row, 1, new QTableWidgetItem(QString::number(frequency)));
        ++row;
    }
}
