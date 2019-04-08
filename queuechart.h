#ifndef QUEUECHART_H
#define QUEUECHART_H

#include "process.h"
#include <QtCharts>
using namespace QtCharts;

class QueueChart
{
private:
    QChart * chart;
    QHorizontalStackedBarSeries * series;
    QQueue<QBarSet *> queue;

public:
    QueueChart(QChartView * v, QString s = "Test");

    QBarSet * addBarSet(QString s, int d, QColor c);
    void addCategory(QString s);
    void removeBarSet(QBarSet * s);

    void push(Process p);
    void pop();
    Process front();
};

#endif // QUEUECHART_H
