#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <QTableWidget>
#include <QKeyEvent>
#include "process.h"
#include "colorgenerator.h"

class ProcessTable : public QTableWidget
{
    Q_OBJECT
    ColorGenerator color;

private:
    void copy();
    void paste();
    void del();

public:
    ProcessTable(QWidget * parent);

    QVector<Process> getProcessList();

public slots:
    void addTableRow();
    void delTableRow();

    void keyPressEvent(QKeyEvent * event);
};

#endif // PROCESSTABLE_H
