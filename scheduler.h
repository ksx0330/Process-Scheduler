#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QVector>
#include <QQueue>
#include <QMap>
#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include "process.h"
#include "ganttchart.h"
#include "queuechart.h"

class Scheduler
{

private:
    QVector<Process> p;
    GanttChart * gc;
    QueueChart * qc;

public:
    Scheduler(QVector<Process> _p, GanttChart * _gc, QueueChart * _qc);

    QVector<Process> FCFS();
    QVector<Process> RR(int timeQuantum);
    QVector<Process> SPN();
    QVector<Process> SRTN();
    QVector<Process> HRRN();

signals:
    void ganttPushed(Process p);

};

struct SRTNCmp{
    bool operator()(std::pair<int, Process> t, std::pair<int, Process> u){
        return t.second.burstTime > u.second.burstTime;
    }
};


#endif // SCHEDULER_H
