#ifndef GANTTCHART_H
#define GANTTCHART_H

#include <QTableWidget>
#include <QLabel>
#include <QVector>

class GanttChart
{
private:
    int row = 6, col = 10;
    QVector<int> backCol;

    QTableWidget * widget;

public:
    GanttChart(QTableWidget * v);
    void addItem(int row, int span, QString s, QColor c);
    void addRow();

    void resizeCol(int col);
    void clearRow(int row);
};

#endif // GANTTCHART_H
