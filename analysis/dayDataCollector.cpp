
#include "dayDataCollector.h"

dayDataCollector::dayDataCollector(QObject *parent) : QThread(parent)
{

}

dayDataCollector::~dayDataCollector()
{
    sqlDisconnect();
}

void dayDataCollector::run()
{
    // thread->start() 시 동작하는 함수
    // Q_EMIT으로 데이터 송신

    if(sqlConnect()==true)
    {
        running = true;
    }
    while(running)
    {
        if(dtCollector()==true)
        {
            Q_EMIT sendDayDatax(datax);
            Q_EMIT sendDayDatay(datay);
        }
        usleep(1000000);
    }
}

void dayDataCollector::end()
{
    // thread 종료
    running = false;
    sqlDisconnect();
}

bool dayDataCollector::sqlConnect()
{
    // DB 서버(Mysql) 연결 함수

    t_start = clock();

    if(!mysql_init(&connection)) qDebug() << "Mysql 연결 초기화";
    ConnPtr = mysql_real_connect(&connection, db_host, db_user, db_pass, db_name, db_port, (char *)NULL, 0);
    if(ConnPtr == NULL)
    {
        qDebug() << mysql_error(&connection);
        mysql_close(ConnPtr);
    }
    else
    {
        qDebug() << db_host << '/' << db_user << '/' << db_pass << '/' << db_name << '/' << db_port;
        qDebug() << "DDC DB연결 성공";
        qDebug() << "Time : " << t_result;
        qDebug() << "###################################################";

        t_end = clock();
        t_result = (t_end - t_start) / CLOCKS_PER_SEC;

        return true;
    }
    return true;
}

bool dayDataCollector::sqlDisconnect()
{
    // DB 서버(Mysql) 연결 해제 함수

    t_start = clock();

    mysql_close(ConnPtr);

    t_end = clock();
    t_result = (t_end - t_start) / CLOCKS_PER_SEC;
    qDebug() << "DB연결 해제";
    qDebug() << "Time : " << t_result;
    qDebug() << "###################################################";
    return true;
}

bool dayDataCollector::dtCollector()
{
    // 1. 한 센서 데이터의 시간은 datax, 데이터는 datay에 저장하여 전송
    // 2. 일간 데이터 수집 스레드는 60초 간격으로 실행, 또는 treeWidget에서 child 클릭 시 실행

    if(dayDataCounter == 0)
    {
        // 하루 전 : "SELECT t, d1 FROM data WHERE t > CURRENT_DATE() - INTERVAL 1 DAY";

        char *Query = "SELECT t, d1 FROM data WHERE t > CURRENT_DATE()";

        Stat = mysql_query(ConnPtr, Query);
        if(Stat != 0)
        {
            qDebug() << mysql_error(&connection);
            mysql_close(ConnPtr);
        }

        Result = mysql_store_result(ConnPtr);

        while((Row = mysql_fetch_row(Result)) != NULL)
        {
            if (QString(Row[0]).mid(17, 2).toDouble()==0)
            {
                datax << QString(Row[0]).mid(11, 2).toDouble()*3600 + QString(Row[0]).mid(14, 2).toDouble()*60 + QString(Row[0]).mid(17, 2).toDouble();
                datay << QString(Row[1]).toDouble();
            }

        }

        mysql_free_result(Result);

        dayDataCounter ++;

    }
    else if(dayDataCounter > 0 && dayDataCounter < 60)
    {
        dayDataCounter ++;
    }
    else
    {
        dayDataCounter = 0;
    }

    return true;
}
