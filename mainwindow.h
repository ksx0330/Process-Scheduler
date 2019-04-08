#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "process.h"
#include "queuechart.h"
#include "ganttchart.h"
#include "colorgenerator.h"
#include "scheduler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QVector<Process> processList;
    QueueChart * qc;
    GanttChart * gc;
    ColorGenerator color;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void initConnect();

public slots:
    void addTableRow();
    void delTableRow();

    void getProcessList();
};

#endif // MAINWINDOW_H
