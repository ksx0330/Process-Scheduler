#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <algorithm>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //qc = new QueueChart(ui->queue, "Queue / Heap");
    gc = new GanttChart(ui->tableWidget);

    QHeaderView * hv = ui->process_list->horizontalHeader();
    hv->setSectionResizeMode(QHeaderView::Stretch);

    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect() {
    connect(ui->add, &QPushButton::clicked, ui->process_list, &ProcessTable::addTableRow);
    connect(ui->del, &QPushButton::clicked, ui->process_list, &ProcessTable::delTableRow);
    connect(ui->start, &QPushButton::clicked, this, &MainWindow::printProcess);
    connect(ui->fullStart, &QPushButton::clicked, this, &MainWindow::printFullProcess);
}

QVector<Process> MainWindow::getScheduledProcessList(int sel, int timeQuantum) {
    Scheduler s(processList, gc);
    QVector<Process> result;

    switch (sel) {
    case 0:
        result = s.FCFS();
        break;

    case 1:
        result = s.RR(timeQuantum);
        break;

    case 2:
        result = s.SPN();
        break;

    case 3:
        result = s.SRTN();
        break;

    case 4:
        result = s.HRRN();
        break;

    case 5:
        result = s.MyScheduler(timeQuantum);
        break;
    }

    return result;
}

void MainWindow::printResult(QVector<Process> result, int sel) {
    QTableWidget * tmp = nullptr;
    QLabel * label = nullptr;

    switch(sel) {
    case 0:
        tmp = ui->FCFS_table;
        label = ui->FCFS_ART;
        break;

    case 1:
        tmp = ui->RR_table;
        label = ui->RR_ART;
        break;

    case 2:
        tmp = ui->SPN_table;
        label = ui->SPN_ART;
        break;

    case 3:
        tmp = ui->SRTN_table;
        label = ui->SRTN_ART;
        break;

    case 4:
        tmp = ui->HRRN_table;
        label = ui->HRRN_ART;
        break;

    case 5:
        tmp = ui->NEW_table;
        label = ui->NEW_ART;
        break;

    }
    tmp->setRowCount(0);

    double sum = 0;
    for (auto item : result) {
        tmp->setRowCount(tmp->rowCount() + 1);
        tmp->setItem(tmp->rowCount() - 1, 0, new QTableWidgetItem(item.name));
        tmp->setItem(tmp->rowCount() - 1, 1, new QTableWidgetItem(QString::number(item.arrivalTime)));
        tmp->setItem(tmp->rowCount() - 1, 2, new QTableWidgetItem(QString::number(item.burstTime)));
        tmp->setItem(tmp->rowCount() - 1, 3, new QTableWidgetItem(QString::number(item.waitingTime)));
        tmp->setItem(tmp->rowCount() - 1, 4, new QTableWidgetItem(QString::number(item.turnaroundTime)));
        tmp->setItem(tmp->rowCount() - 1, 5, new QTableWidgetItem(QString::number(static_cast<double>(item.turnaroundTime) / item.burstTime)));

        sum += item.turnaroundTime;
    }

    label->setText(QString::number(sum / result.length()));
}

void MainWindow::printProcess() {
    int sel = ui->schedule_sel->currentIndex();
    int timeQuantum = ui->time_sel->text().toInt();
    if (timeQuantum == 0) timeQuantum = 1;

    processList = ui->process_list->getProcessList();
    QVector<Process> result = getScheduledProcessList(sel, timeQuantum);
    printResult(result, sel);
    ui->resultTable->setCurrentIndex(sel);
}

void MainWindow::printFullProcess() {
    int timeQuantum = ui->time_sel->text().toInt();
    if (timeQuantum == 0) timeQuantum = 1;
    processList = ui->process_list->getProcessList();

    for (int i = 0; i < 6; i++) {
        QVector<Process> result = getScheduledProcessList(i, timeQuantum);
        printResult(result, i);
    }

}
