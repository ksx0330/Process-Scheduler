#include "ganttchart.h"

GanttChart::GanttChart(QTableWidget * v)
{
    v->setRowCount(row);
    v->setColumnCount(col);
    v->resizeColumnsToContents();
    widget = v;

    for (int i = 0; i < v->columnCount(); i++)
        v->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i)));
    for (int i = 0; i < row; i++)
        backCol.push_back(0);
}

void GanttChart::addItem(int row, int span, QString s, QColor c) {
    if (span == 0) return;
    QTableWidgetItem * i = new QTableWidgetItem();
    i->setText(s);
    i->setTextAlignment(Qt::AlignCenter);
    i->setTextColor(Qt::white);
    i->setBackgroundColor(c);

    QFont font("Helvetica", 11, QFont::Bold);
    i->setFont(font);

    if (backCol[row] + span > col) {
        resizeCol(backCol[row] + span);
    }

    if (span != 1)
        widget->setSpan(row, backCol[row], 1, span);
    widget->setItem(row, backCol[row], i);

    backCol[row] += span;
}

void GanttChart::addRow() {
    backCol.push_back(0);
    widget->setRowCount(row + 1);
}

void GanttChart::resizeCol(int resizeCol) {
    int before = col;
    col = resizeCol;
    widget->setColumnCount(resizeCol);
    widget->resizeColumnsToContents();
    for (int i = before; i < col; i++)
        widget->setHorizontalHeaderItem(i, new QTableWidgetItem(QString::number(i)));

}

void GanttChart::clearRow(int row) {
    for (int i = 0; i < widget->columnCount(); i++) {
        if (widget->item(row, i) != nullptr) {
            widget->item(row, i)->setText("");
            widget->item(row, i)->setBackgroundColor(Qt::white);

            if (widget->columnSpan(row, i) != 1)
                widget->setSpan(row, i, 1, 1);
        }
    }
    backCol[row] = 0;
}
