#include "realTimeDataCollector.h"

realTimeDataCollector::realTimeDataCollector(QObject *parent) : QThread(parent)
{

}

realTimeDataCollector::~realTimeDataCollector()
{
    sqlDisconnect();
}

void realTimeDataCollector::run()
{
    // thread->start() 시 동작하는 함수
    // Q_EMIT으로 데이터 송신

    int counter = 0;

    if(sqlConnect()==true)
    {
        running = true;
    }
    while(running)
    {
        counter += 1;
        qDebug() << "thread counter " << counter;

        Q_EMIT sendRealTimeData(rtCollector());

        usleep(1000000);
    }
}

void realTimeDataCollector::end()
{
    // thread 종료
    running = false;
    sqlDisconnect();
}

bool realTimeDataCollector::sqlConnect()
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
        qDebug() << "DB연결 성공";
        qDebug() << "Time : " << t_result;
        qDebug() << "###################################################";

        t_end = clock();
        t_result = (t_end - t_start) / CLOCKS_PER_SEC;

        return true;
    }
}

bool realTimeDataCollector::sqlDisconnect()
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

QVector<QString> realTimeDataCollector::rtCollector()
{
    /******************************************************************/
    // 데이터 삽입 * 추후 삭제 *

    // 1. DB 서버 연결 후 가상의 센서 데이터 Input
    // 2. Input 이후 DB 서버에서 가장 최근의 데이터 1개 호출
    // 3. 시간은 datax, 데이터는 datay에 저장하여 전송
    // 4. 일간 데이터 수집 스레드는 60초 간격으로 실행, 또는 treeWidget에서 child 클릭 시 실행

    rd_d.clear();
    col_name.clear();
    tmp.clear();

    for(int i=0; i<174; i++)
    {
        srand((double)time(NULL));

        if(i<10)
        {
            rd_d += "'" + QString::number(30.0) + "'";
            rd_d += ',';
        }
        else
        {
            rd_d += "'" + QString::number(15.0 + (rand() % 10) / 10.0) + "'";
            if (i!=173)
            {
                rd_d += ',';
            }
        }

        if(i!=173)
        {
            col_name += 'd'+QString::number(i)+',';
        }
        else
        {
            col_name += 'd'+QString::number(i);
        }

    }

    tmp = "INSERT INTO data(" + col_name + ") values (" + rd_d + ")";
    tmp_qba = tmp.toLatin1();
    Inser_Query = tmp_qba.data();

    mysql_query(ConnPtr, Inser_Query);

    /******************************************************************/
    // 데이터 호출 * 추후 http로 변경 *

    char *Query = "SELECT * FROM data ORDER BY t DESC LIMIT 1";

    Stat = mysql_query(ConnPtr, Query);
    if(Stat != 0)
    {
        qDebug() << mysql_error(&connection);
        mysql_close(ConnPtr);
    }

    Result = mysql_store_result(ConnPtr);

    datax.clear();

    while((Row = mysql_fetch_row(Result)) != NULL)
    {
        for(int i=2; i<176; i++)
        {
            datax << Row[i];
        }
    }
    mysql_free_result(Result);

    return datax;
}
