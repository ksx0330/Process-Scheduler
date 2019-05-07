#include "scheduler.h"

/*
                std::cout << " " << tmp.name.toUtf8().constData() << ": "
                          << tmp.arrivalTime << " " << tmp.burstTime
                          << " " << tmp.waitingTime << " " << tmp.turnaroundTime << std::endl;
*/


Scheduler::Scheduler(QVector<Process> _p, GanttChart * _gc)
    : p(_p), gc(_gc) { }

QVector<Process> Scheduler::FCFS() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    QQueue<std::pair<int, Process>> q;
    int turn = 0, index = 0;

    int lineNum = 0;
    gc->clearRow(0);
    for (; index < now.length() || !q.empty(); turn++) {

        if (!q.empty()) {
            if (q.front().second.burstTime == 0) {
                Process tmp = q.front().second;
                tmp.burstTime = now[q.front().first].burstTime;
                tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
                tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

                ret[q.front().first].waitingTime = tmp.waitingTime;
                ret[q.front().first].turnaroundTime = tmp.turnaroundTime;

                q.pop_front();
                gc->addItem(lineNum, tmp.burstTime, tmp.name, tmp.color);
            }
        }

        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push_back({ index, now[index] });
                index++;
            }
        }

        if (!q.empty())
            q.front().second.burstTime--;
        else if (index < now.length())
            gc->addItem(lineNum, 1, "X", Qt::black);


    }

    return ret;
}

QVector<Process> Scheduler::RR(int timeQuantum) {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    QQueue<std::pair<int, Process>> q;
    int turn = 0, index = 0;
    int times = 1;

    int lineNum = 1;
    gc->clearRow(1);
    for (; index < now.length() || !q.empty(); turn++) {
        if (!q.empty()) {
            if (q.front().second.burstTime == 0) {
                Process tmp = q.front().second;
                tmp.burstTime = now[q.front().first].burstTime;
                tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
                tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

                ret[q.front().first].waitingTime = tmp.waitingTime;
                ret[q.front().first].turnaroundTime = tmp.turnaroundTime;

                gc->addItem(lineNum, times - 1, tmp.name, tmp.color);

                q.pop_front();
                times = 1;
            }

            if (times % (timeQuantum + 1) == 0) {
                gc->addItem(lineNum, times - 1, q.front().second.name, q.front().second.color);

                q.push_back(q.front());
                q.pop_front();
                times = 1;
            }
        }

        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push_back({ index, now[index] });
                index++;
            }
        }

        if (!q.empty()) {
            times++;
            q.front().second.burstTime--;
        } else if (index < now.length())
            gc->addItem(lineNum, 1, "X", Qt::black);

    }

    return ret;
}

QVector<Process> Scheduler::SPN() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    std::priority_queue<std::pair<int, Process>, std::vector<std::pair<int, Process>>, SRTNCmp> q;
    int turn = 0, index = 0;

    std::pair<int, Process> nowP(-1, Process());

    int lineNum = 2;
    gc->clearRow(2);
    for (; index < now.length() || nowP.second.burstTime >= 0; turn++) {
        if (nowP.first != -1 && nowP.second.burstTime == 0) {
            Process tmp = nowP.second;
            tmp.burstTime = now[nowP.first].burstTime;
            tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
            tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

            ret[nowP.first].waitingTime = tmp.waitingTime;
            ret[nowP.first].turnaroundTime = tmp.turnaroundTime;

            gc->addItem(lineNum, tmp.burstTime, tmp.name, tmp.color);

            if (!q.empty()) {
                nowP = q.top();
                q.pop();
            }
        }

        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push({ index, now[index] });
                index++;

                if (index == 1) {
                    nowP = q.top();
                    q.pop();
                }
            }
        }

        if (nowP.first == -1) {
            gc->addItem(lineNum, 1, "X", Qt::black);
            continue;
        }

        if (nowP.second.burstTime <= 0 && !q.empty()) {
            nowP = q.top();
            q.pop();
        }

        nowP.second.burstTime--;
        if (nowP.second.burstTime < 0 && index < now.length())
            gc->addItem(lineNum, 1, "X", Qt::black);

    }

    return ret;
}

QVector<Process> Scheduler::SRTN() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    std::priority_queue<std::pair<int, Process>, std::vector<std::pair<int, Process>>, SRTNCmp> q;
    int turn = 0, index = 0;

    int lineNum = 3;
    gc->clearRow(3);
    for (; index < now.length() || !q.empty(); turn++) {
        if (!q.empty()) {
            if (q.top().second.burstTime == 0) {
                Process tmp = q.top().second;
                tmp.burstTime = now[q.top().first].burstTime;
                tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
                tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

                ret[q.top().first].waitingTime = tmp.waitingTime;
                ret[q.top().first].turnaroundTime = tmp.turnaroundTime;

                q.pop();
            }
        }

        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push({ index, now[index] });
                index++;
            }
        }

        if (!q.empty()) {
            std::pair<int, Process> tmp = q.top();
            q.pop();
            tmp.second.burstTime--;
            q.push(tmp);

            gc->addItem(lineNum, 1, tmp.second.name, tmp.second.color);
        } else if (index < now.length())
            gc->addItem(lineNum, 1, "X", Qt::black);

    }

    return ret;
}

QVector<Process> Scheduler::HRRN() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    QVector<std::pair<int, Process>> v;
    std::pair<int, Process> nowP(-1, Process());
    int turn = 0, index = 0;
    int complete = 0;

    int lineNum = 4;
    gc->clearRow(lineNum);
    for (; index < now.length() || nowP.second.burstTime >= 0; turn++) {
        if (nowP.first != -1 && nowP.second.burstTime == 0) {
            complete++;

            Process tmp = nowP.second;
            tmp.burstTime = now[nowP.first].burstTime;
            tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
            tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

            ret[nowP.first].waitingTime = tmp.waitingTime;
            ret[nowP.first].turnaroundTime = tmp.turnaroundTime;

            gc->addItem(lineNum, tmp.burstTime, tmp.name, tmp.color);

            if (!v.empty()) {
                v[nowP.first].second.burstTime = -1;

                if (v.length() > complete) {
                    nowP = v[1];
                    for (int i = 2; i < v.length(); i++) {
                        if (v[i].second.burstTime == -1) continue;
                        if (static_cast<double>(turn - v[i].second.arrivalTime + v[i].second.burstTime) / v[i].second.burstTime
                                > static_cast<double>(turn - nowP.second.arrivalTime + nowP.second.burstTime) / nowP.second.burstTime)
                            nowP = v[i];
                    }
                }
            }
        }

        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                v.push_back({ index, now[index] });
                index++;

                if (index == 1) {
                    nowP = v[0];
                }
            }
        }

        if (nowP.second.burstTime <= 0 && v.length() > complete) {
            v[nowP.first].second.burstTime = -1;
            nowP = v[1];
            for (int i = 2; i < v.length(); i++) {
                if (v[i].second.burstTime == -1) continue;
                if (static_cast<double>(turn - v[i].second.arrivalTime + v[i].second.burstTime) / v[i].second.burstTime
                        > static_cast<double>(turn - nowP.second.arrivalTime + nowP.second.burstTime) / nowP.second.burstTime)
                    nowP = v[i];
            }
        }

        if (nowP.first == -1) {
            gc->addItem(lineNum, 1, "X", Qt::black);
            continue;
        }

        nowP.second.burstTime--;
        if (nowP.second.burstTime < 0 && index < now.length())
            gc->addItem(lineNum, 1, "X", Qt::black);
    }

    return ret;
}

QVector<Process> Scheduler::MyScheduler(int timeQuantum) {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    QQueue<std::pair<int, Process>> q;
    int turn = 0, index = 0;
    int times = 1;

    bool SPNMode = false;

    int lineNum = 5;
    gc->clearRow(lineNum);
    for (; index < now.length() || !q.empty(); turn++) {
        if (!q.empty()) {
            if (q.front().second.burstTime == 0) {
                Process tmp = q.front().second;
                tmp.burstTime = now[q.front().first].burstTime;
                tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
                tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

                ret[q.front().first].waitingTime = tmp.waitingTime;
                ret[q.front().first].turnaroundTime = tmp.turnaroundTime;

                gc->addItem(lineNum, times - 1, tmp.name, tmp.color);

                q.pop_front();
                times = 1;
            }

            if (!SPNMode && times % (timeQuantum + 1) == 0) {
                gc->addItem(lineNum, times - 1, q.front().second.name, q.front().second.color);

                q.push_back(q.front());
                q.pop_front();
                times = 1;
            }

        }

        if (!q.empty()) {
            if (times == 1 && oneTimeRatio(q, timeQuantum) >= 0.3) {
                SPNMode = true;

                int min = 0;
                std::pair<int, Process> minItem = q[0];
                for (int i = 1; i < q.length(); i++)
                    if (q[i].second.burstTime < minItem.second.burstTime) {
                        minItem = q[i];
                        min = i;
                    }

                q.removeAt(min);
                q.push_front(minItem);
            } else
                SPNMode = false;
        }

        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push_back({ index, now[index] });
                index++;
            }
        }

        if (!q.empty()) {
            times++;
            q.front().second.burstTime--;
        } else if (index < now.length())
            gc->addItem(lineNum, 1, "X", Qt::black);
    }

    return ret;
}

double Scheduler::oneTimeRatio(QQueue<std::pair<int, Process>> _p, int timeQuantum) {
    int size = _p.length();
    double sum = 0;
    for (auto i : _p) {
        if (i.second.burstTime <= timeQuantum)
            sum += 1;
    }c
    return sum / size;
}
