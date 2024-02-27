#ifndef DAYDATACOLLECTOR_H
#define DAYDATACOLLECTOR_H

#include <QThread>
#include <QtDebug>
#include <QTimer>
#include <QString>

#include <time.h>
#include <unistd.h>
#include <QDateTime>
#include <mysql/mysql.h>
#include <iostream>
#include <typeinfo>
#include <thread>
#include <future>
#include <cstdlib>
#include <ctime>

using namespace std;

class dayDataCollector : public QThread
{
    Q_OBJECT

public:
    explicit dayDataCollector(QObject *parent = 0);
    ~dayDataCollector();

    void end();
    void sqlInit();
    bool sqlConnect();
    bool sqlDisconnect();
    bool dtCollector();

    const char *db_host;
    const char *db_user;
    const char *db_pass;
    const char *db_name;
    unsigned int db_port = 23306;
    float t_start = 0;
    float t_end = 0;
    float t_result = 0;
    int dayDataCounter = 0;

    MYSQL* conn, connection;
    MYSQL* ConnPtr = NULL;
    MYSQL_RES* Result;
    MYSQL_ROW Row;
    int Stat;

    int selected_data;

    QVector<double> datax;
    QVector<double> datay;

    QString rd_d;
    QString tmp;
    QByteArray tmp_qba;

private:
    void run();
    bool running = false;

signals:
    void sendDayDatax(QVector<double>);
    void sendDayDatay(QVector<double>);
};

#endif // DAYDATACOLLECTOR_H
