#ifndef REALTIMEDATACOLLECTOR_H
#define REALTIMEDATACOLLECTOR_H

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

class realTimeDataCollector : public QThread
{
    Q_OBJECT

public:
    explicit realTimeDataCollector(QObject *parent = 0);
    ~realTimeDataCollector();

    void end();
    void sqlInit();
    bool sqlConnect();
    bool sqlDisconnect();
    QVector<QString> rtCollector();
    QVector<QString> dtCollector();
    void printData();

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

    QVector<QString> datax;

    QString rd_d;
    QString col_name;
    QString tmp;
    QByteArray tmp_qba;
    const char *Inser_Query;

private:
    void run();
    bool running = false;

signals:
    void sendRealTimeData(QVector<QString>);
};

#endif // REALTIMEDATACOLLECTOR_H




