#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "process.h"
#include "queuechart.h"
#include "ganttchart.h"
#include "colorgenerator.h"
#include "scheduler.h"
#include "processtable.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QVector<Process> processList;
    GanttChart * gc;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void initConnect();
    QVector<Process> getScheduledProcessList(int sel, int timeQuantum);
    void printResult(QVector<Process> result, int sel);

public slots:
    void printProcess();
    void printFullProcess();


};

#endif // MAINWINDOW_H
