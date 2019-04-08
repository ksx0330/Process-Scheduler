#include "queuechart.h"
#include <iostream>

QueueChart::QueueChart(QChartView * v, QString s)
{
    series = new QHorizontalStackedBarSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(s);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "1";
    QBarCategoryAxis * axis = new QBarCategoryAxis();
    axis->append(categories);

    chart->setAxisY(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    v->setChart(chart);
    v->setRenderHint(QPainter::Antialiasing);
}

QBarSet * QueueChart::addBarSet(QString s, int i, QColor c) {
    QBarSet * set = new QBarSet(s);
    *set << i;
    set->setColor(c);

    chart->removeSeries(series);
    series->append(set);
    chart->addSeries(series);

    return set;
}

void QueueChart::removeBarSet(QBarSet * s) {
    chart->removeSeries(series);
    series->remove(s);
    chart->addSeries(series);
}

Process QueueChart::front() {
    auto tmp = queue.front();
    return Process(tmp->label(), tmp->color(), -1, tmp->at(0));
}

void QueueChart::push(Process p) {
    queue.push_back(addBarSet(p.name, p.burstTime, p.color));
}

void QueueChart::pop() {
    removeBarSet(queue.head());
    queue.pop_front();
}

void QueueChart::addCategory(QString s) {
    dynamic_cast<QBarCategoryAxis *>(chart->axisY())->categories() << s;
}
