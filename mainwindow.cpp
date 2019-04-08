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
    qc = new QueueChart(ui->queue, "Queue / Heap");
    gc = new GanttChart(ui->tableWidget);

    QHeaderView * hv = ui->process_list->horizontalHeader();
    hv->setSectionResizeMode(QHeaderView::Stretch);

    /*
    QVector<Process> v;
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 5; i++)
        v.push_back(Process("P" + QString::number(i + 1), color.getColor(i), 5 - i, std::rand() % 10 + 1));

    std::sort(v.begin(), v.end());

    for (auto p : v) {
        g->addItem(0, p.burstTime, p.name, p.color);
        q->push(p);
    }

    QVector<Process> v2;
    v2.push_back(Process("P1", color.getColor(0), 0, 3));
    v2.push_back(Process("P2", color.getColor(1), 1, 7));
    v2.push_back(Process("P3", color.getColor(2), 3, 2));
    v2.push_back(Process("P4", color.getColor(2), 5, 5));
    v2.push_back(Process("P5", color.getColor(2), 6, 3));


    Scheduler s(v2, gc, qc);
    auto result = s.FCFS();

    for (auto item : result) {
        std::cout << item.name.toUtf8().constData() << ": "
                  << item.arrivalTime << " " << item.burstTime
                  << " " << item.waitingTime << " " << item.turnaroundTime << std::endl;
    }

    std::cout << "\n\n";
    result = s.RR(3);

    for (auto item : result) {
        std::cout << item.name.toUtf8().constData() << ": "
                  << item.arrivalTime << " " << item.burstTime
                  << " " << item.waitingTime << " " << item.turnaroundTime << std::endl;
    }

    std::cout << "\n\n";
    result = s.SPN();

    for (auto item : result) {
        std::cout << item.name.toUtf8().constData() << ": "
                  << item.arrivalTime << " " << item.burstTime
                  << " " << item.waitingTime << " " << item.turnaroundTime << std::endl;
    }

    std::cout << "\n\n";
    result = s.SRTN();

    for (auto item : result) {
        std::cout << item.name.toUtf8().constData() << ": "
                  << item.arrivalTime << " " << item.burstTime
                  << " " << item.waitingTime << " " << item.turnaroundTime << std::endl;
    }

    std::cout << "\n\n";
    result = s.HRRN();

    for (auto item : result) {
        std::cout << item.name.toUtf8().constData() << ": "
                  << item.arrivalTime << " " << item.burstTime
                  << " " << item.waitingTime << " " << item.turnaroundTime << std::endl;
    }
    */

    initConnect();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect() {
    connect(ui->add, &QPushButton::clicked, this, &MainWindow::addTableRow);
    connect(ui->del, &QPushButton::clicked, this, &MainWindow::delTableRow);
    //connect(s, &Scheduler::ganttPushed, this, &MainWindow::addGantt);
    connect(ui->start, &QPushButton::clicked, this, &MainWindow::getProcessList);
}

void MainWindow::addTableRow() {
    ui->process_list->setRowCount(ui->process_list->rowCount() + 1);
}

void MainWindow::delTableRow() {
    auto tmp = ui->process_list->currentRow();
    ui->process_list->removeRow(tmp);
}

void MainWindow::getProcessList() {
    int row = ui->process_list->rowCount();
    processList.clear();
    color.clear();
    for (int i = 0; i < row; i++) {
        if (ui->process_list->item(i, 0) != nullptr && ui->process_list->item(i, 1) != nullptr && ui->process_list->item(i, 2) != nullptr) {
            processList.push_back(Process(ui->process_list->item(i, 0)->text(), color.getColor(i),
                    ui->process_list->item(i, 1)->text().toInt(), ui->process_list->item(i, 2)->text().toInt()));
        }
    }
    std::sort(processList.begin(), processList.end());
    for (int i = 0; i < processList.length(); i++)
        std::cout << processList[i].toString() << std::endl;

    int sel = ui->schedule_sel->currentIndex();
    int timeQuantum = ui->time_sel->text().toInt();
    Scheduler s(processList, gc, qc);
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
    }

    for (auto item : result) {
        std::cout << item.name.toUtf8().constData() << ": "
                  << item.arrivalTime << " " << item.burstTime
                  << " " << item.waitingTime << " " << item.turnaroundTime << std::endl;
    }

}
