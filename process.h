#ifndef PROCESS_H
#define PROCESS_H

#include <QString>
#include <QColor>

struct Process {
    QString name;
    QColor color;

    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;

    Process() { }
    Process(QString n, QColor c, int a, int b)
        : name(n), color(c), arrivalTime(a), burstTime(b) { }

    bool operator<(Process p) {
        return arrivalTime < p.arrivalTime;
    }

    std::string toString() {
        return name.toStdString() + ": " + std::to_string(arrivalTime) + " " + std::to_string(burstTime);
    }
};

#endif // PROCESS_H
