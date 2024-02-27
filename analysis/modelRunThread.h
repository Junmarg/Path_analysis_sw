#ifndef MODELRUNTHREAD_H
#define MODELRUNTHREAD_H

#include <QThread>
#include <QtDebug>
#include <QTimer>
#include <QString>

#include <time.h>
#include <unistd.h>
#include <QDateTime>
#include <iostream>
#include <typeinfo>
#include <thread>
#include <future>
#include <cstdlib>
#include <ctime>

#include "cppflow/cppflow.h"

using namespace std;

class modelRunThread : public QThread
{
    Q_OBJECT

public:
    explicit modelRunThread(QObject *parent = 0);
    ~modelRunThread();
    QVector<QString> sensor_data;
    void end();

private:
    void run();
    bool running = true;
    QVector<QString> model_output;

signals:
    void output(QVector<QString>);
};

#endif // MODELRUNTHREAD_H
