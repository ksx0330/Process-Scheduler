#include "scheduler.h"

/*
                std::cout << " " << tmp.name.toUtf8().constData() << ": "
                          << tmp.arrivalTime << " " << tmp.burstTime
                          << " " << tmp.waitingTime << " " << tmp.turnaroundTime << std::endl;
*/


Scheduler::Scheduler(QVector<Process> _p, GanttChart * _gc, QueueChart * _qc)
    : p(_p), gc(_gc), qc(_qc) { }

QVector<Process> Scheduler::FCFS() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    QQueue<std::pair<int, Process>> q;
    int turn = 0, index = 0;

    gc->clearRow(0);
    for (; index < now.length() || !q.empty(); turn++) {
        //QThread::sleep(1);
        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push_back({ index, now[index] });
                index++;
            }
        }

        if (!q.empty()) {
            if (q.front().second.burstTime == 0) {
                Process tmp = q.front().second;
                tmp.burstTime = now[q.front().first].burstTime;
                tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
                tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

                ret[q.front().first].waitingTime = tmp.waitingTime;
                ret[q.front().first].turnaroundTime = tmp.turnaroundTime;

                q.pop_front();
                //qc->pop();
                gc->addItem(0, tmp.burstTime, tmp.name, tmp.color);
            }
            if (!q.empty())
                q.front().second.burstTime--;
            else if (index < now.length())
                gc->addItem(0, 1, "X", Qt::black);
        } else
            gc->addItem(0, 1, "X", Qt::black);
    }

    return ret;
}

QVector<Process> Scheduler::RR(int timeQuantum) {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    QQueue<std::pair<int, Process>> q;
    int turn = 0, index = 0;
    int times = 1;

    gc->clearRow(1);
    for (; index < now.length() || !q.empty(); turn++) {
        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push_back({ index, now[index] });
                index++;
            }
        }


        if (!q.empty()) {
            if (q.front().second.burstTime == 0) {
                Process tmp = q.front().second;
                tmp.burstTime = now[q.front().first].burstTime;
                tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
                tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

                ret[q.front().first].waitingTime = tmp.waitingTime;
                ret[q.front().first].turnaroundTime = tmp.turnaroundTime;

                gc->addItem(1, times - 1, tmp.name, tmp.color);

                q.pop_front();
                times = 1;
            }

            if (!q.empty()) {
                if (times % (timeQuantum + 1) == 0) {
                    gc->addItem(1, times - 1, q.front().second.name, q.front().second.color);

                    q.push_back(q.front());
                    q.pop_front();
                    times = 1;
                }

                times++;
                q.front().second.burstTime--;
            } else if (index < now.length())
                gc->addItem(1, 1, "X", Qt::black);
        } else
            gc->addItem(1, 1, "X", Qt::black);

    }

    return ret;
}

QVector<Process> Scheduler::SPN() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    std::priority_queue<std::pair<int, Process>, std::vector<std::pair<int, Process>>, SRTNCmp> q;
    int turn = 0, index = 0;

    std::pair<int, Process> nowP(-1, Process());

    gc->clearRow(2);
    for (; index < now.length() || nowP.second.burstTime >= 0; turn++) {
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
            gc->addItem(4, 1, "X", Qt::black);
            continue;
        }

        if (nowP.second.burstTime == 0) {
            Process tmp = nowP.second;
            tmp.burstTime = now[nowP.first].burstTime;
            tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
            tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

            ret[nowP.first].waitingTime = tmp.waitingTime;
            ret[nowP.first].turnaroundTime = tmp.turnaroundTime;

            gc->addItem(2, tmp.burstTime, tmp.name, tmp.color);

            if (!q.empty()) {
                nowP = q.top();
                q.pop();
            }
        }

        if (nowP.second.burstTime <= 0 && !q.empty()) {
            nowP = q.top();
            q.pop();
        }

        nowP.second.burstTime--;
        if (nowP.second.burstTime < 0 && index < now.length())
            gc->addItem(2, 1, "X", Qt::black);

    }

    return ret;
}

QVector<Process> Scheduler::SRTN() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    std::priority_queue<std::pair<int, Process>, std::vector<std::pair<int, Process>>, SRTNCmp> q;
    int turn = 0, index = 0;

    gc->clearRow(3);
    for (; index < now.length() || !q.empty(); turn++) {
        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                q.push({ index, now[index] });
                index++;
            }
        }


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

            if (!q.empty()) {
                std::pair<int, Process> tmp = q.top();
                q.pop();
                tmp.second.burstTime--;
                q.push(tmp);

                gc->addItem(3, 1, tmp.second.name, tmp.second.color);
            } else if (index < now.length())
                gc->addItem(3, 1, "X", Qt::black);
        } else
            gc->addItem(0, 1, "X", Qt::black);
    }

    return ret;
}

QVector<Process> Scheduler::HRRN() {
    QVector<Process> ret(p);
    QVector<Process> now(p);
    QVector<std::pair<int, Process>> v;
    std::pair<int, Process> nowP(-1, Process());
    int turn = 0, index = 0;

    gc->clearRow(4);
    for (; index < now.length() || nowP.second.burstTime >= 0; turn++) {
        for (int idx = index; idx < now.length(); idx++) {
            if (now[index].arrivalTime == turn) {
                v.push_back({ index, now[index] });
                index++;

                if (index == 1) {
                    nowP = v[0];
                }
            }
        }

        if (nowP.first == -1) {
            gc->addItem(4, 1, "X", Qt::black);
            continue;
        }

        if (nowP.second.burstTime == 0) {
            Process tmp = nowP.second;
            tmp.burstTime = now[nowP.first].burstTime;
            tmp.waitingTime = turn - tmp.burstTime - tmp.arrivalTime;
            tmp.turnaroundTime = tmp.waitingTime + tmp.burstTime;

            ret[nowP.first].waitingTime = tmp.waitingTime;
            ret[nowP.first].turnaroundTime = tmp.turnaroundTime;

            gc->addItem(4, tmp.burstTime, tmp.name, tmp.color);

            if (!v.empty()) {
                v[nowP.first].second.burstTime = -1;
                nowP = v[1];
                for (int i = 1; i < v.length(); i++) {
                    if (v[i].second.burstTime == -1) continue;
                    if (static_cast<double>(turn - v[i].second.arrivalTime + v[i].second.burstTime) / v[i].second.burstTime
                            > static_cast<double>(turn - nowP.second.arrivalTime + nowP.second.burstTime) / nowP.second.burstTime)
                        nowP = v[i];
                }
            }
        }

        if (nowP.second.burstTime <= 0 && !v.empty()) {
            v[nowP.first].second.burstTime = -1;
            nowP = v[1];
            for (int i = 1; i < v.length(); i++) {
                if (v[i].second.burstTime == -1) continue;
                if (static_cast<double>(turn - v[i].second.arrivalTime + v[i].second.burstTime) / v[i].second.burstTime
                        > static_cast<double>(turn - nowP.second.arrivalTime + nowP.second.burstTime) / nowP.second.burstTime)
                    nowP = v[i];
            }
        }

        nowP.second.burstTime--;
        if (nowP.second.burstTime < 0 && index < now.length())
            gc->addItem(4, 1, "X", Qt::black);
    }

    return ret;
}
