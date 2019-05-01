#include <QApplication>
#include <QClipboard>
#include "processtable.h"

#include <iostream>

ProcessTable::ProcessTable(QWidget * parent)
    : QTableWidget(parent) {

}

void ProcessTable::addTableRow() {
    setRowCount(rowCount() + 1);
}

void ProcessTable::delTableRow() {
    auto tmp = currentRow();
    removeRow(tmp);
}

QVector<Process> ProcessTable::getProcessList() {
    int row = rowCount();
    QVector<Process> processList;
    processList.clear();
    color.clear();
    for (int i = 0; i < row; i++) {
        if (item(i, 0) != nullptr && item(i, 1) != nullptr && item(i, 2) != nullptr) {
            processList.push_back(Process(item(i, 0)->text(), color.getColor(i),
                    item(i, 1)->text().toInt(), item(i, 2)->text().toInt()));
        }
    }
    std::sort(processList.begin(), processList.end());

    return processList;
}

void ProcessTable::copy() {
    int row = rowCount();
    QString s = "";

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < 3; j++) {
            if (item(i, j) == nullptr || !item(i, j)->isSelected())
                s += "/";
            else if (item(i, j)->isSelected())
                s += item(i, j)->text() + "/";
        }
        s += "|";
    }

    qApp->clipboard()->setText(s);
}

void ProcessTable::paste() {
    clearContents();

    int index = 0;
    QString s = qApp->clipboard()->text();
    for (auto a : s.split("|")) {
        int i = 0;
        for (auto b : a.split("/")) {
            if (b != "")
                setItem(index, i, new QTableWidgetItem(b));
            i++;
        }
        index++;
    }
}

void ProcessTable::del() {
    for (auto i : selectedItems())
        i->setText("");
}

void ProcessTable::keyPressEvent(QKeyEvent * event) {
    if (event->matches(QKeySequence::Copy))
        copy();
    else if (event->matches(QKeySequence::Paste))
        paste();
    else if (event->matches(QKeySequence::Delete))
        del();
    else
        QTableWidget::keyPressEvent(event);
}

