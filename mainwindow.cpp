#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // UI Load
    ui->setupUi(this);

    // Initialize
    openGLInit();
    customplotInit();
    dataCollectorInit();
    setEvent();
    plotInit();
    treeWidgetInit();

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*************************************************************************/
// INIT

void MainWindow::openGLInit()
{
    // OpenGL 초기화
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    format.setSamples(4);	// enable multisampling (antialiasing)
    format.setDepthBufferSize(8);

    m_sceneView = new SceneView;
    m_sceneView->setFormat(format);

    // *** create window container widget

    QWidget *container = QWidget::createWindowContainer(m_sceneView);
    container->setFocusPolicy(Qt::TabFocus);
    container->setMinimumSize(QSize(640,474));
    container->setFocus();
    // *** create the layout and insert widget container

    ui->vlay->setSpacing(0);
    ui->vlay->addWidget(container);

}

void MainWindow::customplotInit()
{
}

void MainWindow::dataCollectorInit()
{
}

void MainWindow::setEvent()
{
    // 데이터 수집 스레드 생성 및 시작
    rdc = new realTimeDataCollector();
    ddc = new dayDataCollector();

    // 데이터 수집 Thread Signal/Slots 등록
    connect(rdc, SIGNAL(sendRealTimeData(QVector<QString>)), this, SLOT(realTimeDataPlot(QVector<QString>)));
    connect(ddc, SIGNAL(sendDayDatax(QVector<double>)), this, SLOT(dayDataxPlot(QVector<double>)));
    connect(ddc, SIGNAL(sendDayDatay(QVector<double>)), this, SLOT(dayDatayPlot(QVector<double>)));

    // QCustomplot의 동적 출력을 위해 이벤트 등록
    realTimeDataTimer = new QTimer();
    connect(realTimeDataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));

    dayDataTimer = new QTimer();
    connect(dayDataTimer, SIGNAL(timeout()), this, SLOT(dayDataSlot()));

    // 데이터 분석 Thread 등록
    mrt_01 = new modelRunThread();
    connect(mrt_01, SIGNAL(output(QVector<QString>)), this, SLOT(modelOutput(QVector<QString>)));

    mrt_02 = new modelRunThread();
    connect(mrt_02, SIGNAL(output(QVector<QString>)), this, SLOT(modelOutput(QVector<QString>)));

    mrt_03 = new modelRunThread();
    connect(mrt_03, SIGNAL(output(QVector<QString>)), this, SLOT(modelOutput(QVector<QString>)));

    mrt_04 = new modelRunThread();
    connect(mrt_04, SIGNAL(output(QVector<QString>)), this, SLOT(modelOutput(QVector<QString>)));

    mrt_05 = new modelRunThread();
    connect(mrt_05, SIGNAL(output(QVector<QString>)), this, SLOT(modelOutput(QVector<QString>)));

    // 이벤트 슬롯 등록
    connect(ui->connect_btn, SIGNAL(clicked()), this, SLOT(dataCollectorRun()));        // 프로그램 시작 버튼 이벤트
    connect(ui->connect_test_btn, SIGNAL(clicked()), this, SLOT(dataCollectorTest()));  // 클라우드 연결 테스트 버튼 이벤트
    connect(ui->test_btn, SIGNAL(clicked()), this, SLOT(testBtnClicked()));             // 단일 기능 테스트를 위한 함수 시작 버튼 이벤트
    connect(ui->setting_btn, SIGNAL(clicked()), this, SLOT(settingBtnClicked()));       // 세팅 버튼 이벤트

    connect(ui->tree_expand_btn, SIGNAL(clicked()), this, SLOT(treeExpandBtnClicked()));    // treeWidget 확장 버튼 이벤트트
    connect(ui->tree_callabs_btn, SIGNAL(clicked()), this, SLOT(treeCallabBtnClicked()));   // treeWidget 축소 버튼 이벤트
    connect(ui->sensor_data_tree,  SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeCurrentItemChanged(QTreeWidgetItem*,int)));       // treeWidget 마우스 클릭 인식 이벤트

    // 프로그램에 시간을 Label로 출력하는 이벤트
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(setDataTime()));
    timer -> start(5);
}

void MainWindow::plotInit()
{
    /*******************************************************/
    //// 실시간 데이터 표출용 그래프 초기화
    //now_graph = ui->now_data_graph;
    //now_graph->plotLayout()->clear();
/*
    wideAxisRect = new QCPAxisRect(now_graph);
    wideAxisRect->setupFullAxesBox(true);
    wideAxisRect->axis(QCPAxis::atLeft)->setTickLabelColor(QColor("#FE2E2E"));
    //now_graph->plotLayout()->addElement(0, 0, wideAxisRect); // insert axis rect in first row

    //now_graph->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
    //now_graph->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    QSharedPointer<QCPAxisTickerDateTime> realTimeTicker(new QCPAxisTickerDateTime);
    realTimeTicker->setDateTimeSpec(Qt::UTC);
    realTimeTicker->setTickCount(2);
    realTimeTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssReadability);
    realTimeTicker->setDateTimeFormat("hh:mm:ss");
    now_graph->xAxis->setTicker(realTimeTicker);
    now_graph->axisRect()->setupFullAxesBox();
    now_graph->xAxis->setRange(-5,40);
    now_graph->yAxis->setRange(25, 40);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(wideAxisRect->axis(QCPAxis::atBottom), SIGNAL(rangeChanged(QCPRange)), wideAxisRect->axis(QCPAxis::atBottom), SLOT(setRange(QCPRange)));
    connect(wideAxisRect->axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)), wideAxisRect->axis(QCPAxis::atLeft), SLOT(setRange(QCPRange)));
*/
    /*******************************************************/
    // 하루 데이터 표출용 그래프 초기화
    //day_graph = ui->day_data_graph;
    //day_graph->plotLayout()->clear();
/*
    wideAxisRect = new QCPAxisRect(day_graph);
    wideAxisRect->setupFullAxesBox(true);
    wideAxisRect->axis(QCPAxis::atLeft)->setTickLabelColor(QColor("#FE2E2E"));
    //day_graph->plotLayout()->addElement(0, 0, wideAxisRect); // insert axis rect in first row

    day_graph->addGraph(wideAxisRect->axis(QCPAxis::atBottom), wideAxisRect->axis(QCPAxis::atLeft));
    day_graph->graph(0)->setPen(QPen(QColor(0, 0, 255)));

    QSharedPointer<QCPAxisTickerDateTime> dayTimeTicker(new QCPAxisTickerDateTime);
    dayTimeTicker->setDateTimeSpec(Qt::UTC);
    dayTimeTicker->setTickCount(8);
    dayTimeTicker->setTickStepStrategy(QCPAxisTicker::TickStepStrategy::tssReadability);
    dayTimeTicker->setDateTimeFormat("hh:mm:ss");
    day_graph->xAxis->setTicker(dayTimeTicker);
    day_graph->axisRect()->setupFullAxesBox();

    day_graph->xAxis->setRange(0, 86400);
    day_graph->yAxis->setRange(25, 40);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(wideAxisRect->axis(QCPAxis::atLeft), SIGNAL(rangeChanged(QCPRange)), wideAxisRect->axis(QCPAxis::atLeft), SLOT(setRange(QCPRange)));
*/
    /*******************************************************/
    // 사진 저장용 테스트
//    print_jpg_plot = ui->save_fig_graph;
//    print_jpg_plot->addGraph();
//    /*
//    print_jpg_plot->xAxis->setTickLabels(false);
//    print_jpg_plot->yAxis->setTickLabels(false);
//    print_jpg_plot->xAxis->setTicks(false);
//    print_jpg_plot->yAxis->setTicks(false);
//    */
//    QVector<double> x(30), y(30); // initialize with entries 0..100
//    for (int i=0; i<30; ++i)
//    {
//        x[i] = i;
//        y[i] = 30;
//    }
//    for(int i=0; i<30; i++)
//    {
//        print_jpg_plot->graph(0)->setData(x, y);

//    }
//    print_jpg_plot->xAxis->grid()->setVisible(false);
//    print_jpg_plot->yAxis->grid()->setVisible(false);
//    print_jpg_plot->xAxis->setTicks(false);
//    print_jpg_plot->yAxis->setTicks(false);
//    print_jpg_plot->xAxis->setRange(0,30);
//    print_jpg_plot->yAxis->setRange(0,60);

//    QPen penclear;
//    penclear.setStyle(Qt::SolidLine);
//    penclear.setWidth(0);
//    penclear.setColor(QColor(1,1,1,1));

//    print_jpg_plot->xAxis->setBasePen(penclear);
//    print_jpg_plot->yAxis->setBasePen(penclear);
//    print_jpg_plot->xAxis->setTickPen(penclear);
//    print_jpg_plot->yAxis->setTickPen(penclear);

//    print_jpg_plot->replot();
}

void MainWindow::treeWidgetInit()
{
    /*******************************************************/
    // QTreeWidget initialize
    // place, 정온식, 차등식, 광전식
    treeWidget("201", 1,1,3);
    treeWidget("202", 0,1,1);
    treeWidget("203", 1,2,2);
    treeWidget("204", 1,2,2);
    treeWidget("205", 1,2,2);
    treeWidget("206", 1,2,2);
    treeWidget("207", 1,2,4);

    treeWidget("301", 1,2,3);
    treeWidget("302", 1,2,2);
    treeWidget("303", 1,2,2);
    treeWidget("304", 1,2,2);
    treeWidget("305", 1,2,2);
    treeWidget("306", 1,2,2);
    treeWidget("307", 1,2,4);

    treeWidget("401", 1,2,3);
    treeWidget("402", 1,2,2);
    treeWidget("403", 1,2,2);
    treeWidget("404", 1,2,2);
    treeWidget("405", 1,2,2);
    treeWidget("406", 1,2,2);
    treeWidget("407", 1,2,3);

    treeWidget("501", 1,2,3);
    treeWidget("502", 1,2,2);
    treeWidget("503", 1,2,2);
    treeWidget("504", 1,2,2);
    treeWidget("505", 1,2,2);
    treeWidget("506", 1,2,2);
    treeWidget("507", 1,2,3);

    treeWidget("floor_1", 0,5,4);
    treeWidget("floor_2", 0,0,4);
    treeWidget("floor_3", 0,0,4);
    treeWidget("floor_4", 0,0,4);
    treeWidget("floor_5", 0,0,4);
    treeWidget("roof", 0,0,1);
    treeWidget("under", 2,0,0);
}

void MainWindow::setDataTime()
{
    QTime time = QTime::currentTime();
    QString time_text = time.toString("hh:mm:ss.zzz");
    ui -> date_time_label -> setText(time_text);
}

/*************************************************************************/
// SLOT

void MainWindow::settingBtnClicked()
{
    // 메인 <-> 세팅 Form 변경 이벤트
    if(ui->stackedWidget->currentIndex()==0)
    {
        // Main -> Setting Form
        ui->stackedWidget->setCurrentIndex(1);
        ui->setting_btn->setText("메인");
    }
    else if(ui->stackedWidget->currentIndex()==1)
    {
        // Setting -> Main Form
        ui->stackedWidget->setCurrentIndex(0);
        ui->setting_btn->setText("세팅");
    }
}

void MainWindow::testBtnClicked()
{
    // 단일 기능 테스트용 함수
    // test 버튼 누를 시 동작
    m_sceneView->fire_trigger=false;
    // now_graph->graph(1)->setPen(QPen(QColor(1, 1, 1)));
    //print_jpg_plot->saveJpg("sas.jpg",600,400,1,100);
}

void MainWindow::treeExpandBtnClicked()
{
    // treeWidget child 펼치기
    ui->sensor_data_tree->resizeColumnToContents(0);
    ui->sensor_data_tree->expandAll();
    // ui->sensor_data_tree->collapseAll();

}

void MainWindow::treeCallabBtnClicked()
{
    // treeWidget child 접기
    ui->sensor_data_tree->resizeColumnToContents(0);
    ui->sensor_data_tree->collapseAll();
}

void MainWindow::treeCurrentItemChanged(QTreeWidgetItem* a,int b)
{
    // treeWidget 마우스 클릭 시, 해당 아이템 정보 로드&저장
    treeCurrentData.clear();

    QModelIndexList indexes = ui->sensor_data_tree->selectionModel()->selectedIndexes();

    if(ui->sensor_data_tree->indexOfTopLevelItem(a)<0)
    {
        int count = 0;

        for(int i=0; i<ui->sensor_data_tree->topLevelItemCount();i++)
        {
            for(int j=0; j<ui->sensor_data_tree->topLevelItem(i)->childCount(); j++)
            {
                QTreeWidgetItem *item = ui->sensor_data_tree->topLevelItem(i)->child(j);

                if(a->parent()->text(0) == item->parent()->text(0))
                {
                    if(a->text(0) == item->text(0))
                    {
                        treeDataStruct c = {a->parent()->text(0), "child",  a->text(1), indexes[0].row(), count};
                        treeCurrentData.push_back(c);

                        break;
                    }
                }

                count++;
            }
        }
    }
    else
    {
        treeDataStruct c = {a->text(0), "parent",  a->text(1), indexes[0].row(), b};
        treeCurrentData.push_back(c);
    }

    qDebug() << treeCurrentData[0].name << treeCurrentData[0].parent << treeCurrentData[0].sensor << treeCurrentData[0].index_parent << treeCurrentData[0].index_child;

    plotDataChange();
}

void MainWindow::plotDataChange()
{
    // treeWidget에서 child 클릭 시 일간 데이터 출력 변경
    if(treeCurrentData[0].parent =="child")
    {
        // rdc->selected_data = treeCurrentData[0].index_child;
        ddc->selected_data = treeCurrentData[0].index_child;

        //now_graph->graph(0)->data()->clear();
    }
}

void MainWindow::dataCollectorRun()
{
    // 클라우드 연결 시작
    if(connected==false)
    {
        // 데이터 수집 스레드 설정 및 시작

        tmp = setting.getValue("server/host");
        tmp_host_qba = tmp.toLatin1();
        rdc->db_host = tmp_host_qba.data();
        ddc->db_host = tmp_host_qba.data();

        tmp = setting.getValue("server/user");
        tmp_user_qba = tmp.toLatin1();
        rdc->db_user = tmp_user_qba.data();
        ddc->db_user = tmp_user_qba.data();

        tmp = setting.getValue("server/pass");
        tmp_pass_qba = tmp.toLatin1();
        rdc->db_pass = tmp_pass_qba.data();
        ddc->db_pass = tmp_pass_qba.data();

        tmp = setting.getValue("server/name");
        tmp_name_qba = tmp.toLatin1();
        rdc->db_name = tmp_name_qba.data();
        ddc->db_name = tmp_name_qba.data();

        tmp = setting.getValue("server/port");
        tmp_port_qba = tmp.toLatin1();
        rdc->db_port = stoi(tmp_port_qba.data());
        ddc->db_port = stoi(tmp_port_qba.data());

        // Thread가 QVector인 경우 에러 출력 방지를 위해 사용
        qRegisterMetaType<QVector<QString>>("QVector");

        rdc->start();
        ddc->start();
        mrt_01->start();
        mrt_02->start();
        mrt_03->start();
        mrt_04->start();
        mrt_05->start();

        etime.start();
        realTimeDataTimer->start(0); // Interval 0 means to refresh as fast as possible
        dayDataTimer->start(0); // Interval 0 means to refresh as fast as possible

        connected = true;

        // 시스템 로그 변경 -> "Colud Connected"
        systemLogChange(1);

        // logTableWidget
        addLog("cloud connected");
    }
    else
    {
        // 데이터 수집 스레드 종료
        rdc->end();
        ddc->end();
        mrt_01->end();
        mrt_02->end();
        mrt_03->end();
        mrt_04->end();
        mrt_05->end();

        // 그래프 타이머 종료
        realTimeDataTimer->stop();
        dayDataTimer->stop();

        // 그래프 데이터 초기화
        //now_graph->graph(0)->data()->clear();
        //day_graph->graph(0)->data()->clear();

        connected = false;
        plotStart = true;

        // 시스템 로그 변경 -> "Colud Disconnected"
        systemLogChange(2);

        // logTableWidget
        addLog("cloud disconnected");
    }
}

void MainWindow::dataCollectorTest()
{
    // 클라우드 연결 테스트
   // bool a = rdc->sqlConnect();
}

void MainWindow::systemLogChange(int Case)
{
    // 현재 상태에 따른 위젯 하단의 시스템 로그 변경
    if(Case==0)
    {
        ui->system_log_label->setText("None");
        ui->system_log_label->setStyleSheet("color:black");
    }
    if(Case==1)
    {
        ui->system_log_label->setText("Cloud Connected");
        ui->system_log_label->setStyleSheet("color:green");
    }
    if(Case==2)
    {
        ui->system_log_label->setText("Cloud Disconnected");
        ui->system_log_label->setStyleSheet("color:red");
    }
    if(Case==3)
    {
        ui->system_log_label->setText("Cloud Connect Error");
        ui->system_log_label->setStyleSheet("color:red");
    }
}

void MainWindow::realtimeDataSlot()
{
    // QCustomplot, 실시간 데이터 출력을 위한 그래프의 Slots 함수
    QTime current_time = QTime::currentTime();
    double key = current_time.msecsSinceStartOfDay()/1000.0;
    static double lastPointKey = 0;

    if(key-lastPointKey > 0.002)
    {
        if(nowData.empty() or treeCurrentData.empty())
        {

        }
        else
        {
            //now_graph->graph(0)->addData(key, nowData[treeCurrentData[0].index_child].toDouble());
            //now_graph->yAxis->setRange(nowData[treeCurrentData[0].index_child].toDouble()-5, nowData[treeCurrentData[0].index_child].toDouble()+10);
        }
        lastPointKey = key;
    }

   // now_graph->xAxis->setRange(key + 0.25, 8, Qt::AlignRight);
    //now_graph->replot(QCustomPlot::rpQueuedReplot);
}

void MainWindow::realTimeDataPlot(QVector<QString> data)
{
    // 실시간 센서 데이터 수집
    nowData = data;

    // TreeWidget 표출
    dataPlot(nowData);

    // 데이터 분석 ( 화재 여부, 최단 경로 )
    // 각 thread에 데이터 넣어서 실행
    if(mrt_01->sensor_data.empty()==false)
    {
        if(mrt_02->sensor_data.empty()==false)
        {
            if(mrt_03->sensor_data.empty()==false)
            {
                if(mrt_04->sensor_data.empty()==false)
                {
                    if(mrt_05->sensor_data.empty()==false)   qDebug() << "analyer Thread is FULL!!";
                    else    mrt_05->sensor_data = data;
                }
                else    mrt_04->sensor_data = data;
            }
            else    mrt_03->sensor_data = data;
        }
        else    mrt_02->sensor_data = data;
    }
    else    mrt_01->sensor_data = data;

    // OpenGL에서 피난경로 표출, TreeWidget에서 선택한 데이터 == 출발지
    if(treeCurrentData.empty()==true){

        auto [xa, ya] = da.pathAnalyer("201", nowData);
        m_sceneView->path2render(ya);
        addpath(xa);}
    else{
        auto [xa, ya] = da.pathAnalyer(treeCurrentData[0].name, nowData);
        m_sceneView->path2render(ya);
        addpath(xa);}

}

void MainWindow::dayDataSlot()
{
    // QCustomplot, 일간 데이터 출력을 위한 그래프의 Slots 함수
   // day_graph->graph(0)->setData(dayDatax, dayDatay);
    //day_graph->replot(QCustomPlot::rpQueuedReplot);
}

void MainWindow::dayDataxPlot(QVector<double> datax)
{
    // 일간 센서 데이터 수집
  //  dayDatax = datax;
    //qDebug() << dayDatax;
}

void MainWindow::dayDatayPlot(QVector<double> datay)
{
   // dayDatay = datay;
    //qDebug() << dayDatay;
}

void MainWindow::modelOutput(QVector<QString> output)
{
/*
    ui->analyLogTableWidget->insertRow(ui->analyLogTableWidget->rowCount()); // Row 추가
    int index = ui->analyLogTableWidget->rowCount() - 1;

    QTime time = QTime::currentTime();
    ui->analyLogTableWidget->setItem(index, 0, new QTableWidgetItem(time.toString("hh:mm:ss.zzz")));


    ui->analyLogTableWidget->setItem(index, 1, new QTableWidgetItem(output[0]));
    ui->analyLogTableWidget->setItem(index, 2, new QTableWidgetItem(output[1]));
    ui->analyLogTableWidget->setItem(index, 3, new QTableWidgetItem(output[2]));
    ui->analyLogTableWidget->scrollToBottom();
*/
}
/*************************************************************************/
// FUNC

void MainWindow::dataPlot(QVector<QString> data)
{
    // TreeWidget에 센서 데이터 플로팅하는 함수
    if(plotStart==true)
    {
        preData = nowData;
        plotStart = false;
    }

    int count = 0;

    // 센서 값의 상승 or 하락에 따른 TreeWidget에서의 배경색 변경
    for(int i=0; i<ui->sensor_data_tree->topLevelItemCount();i++)
    {
        for(int j=0; j<ui->sensor_data_tree->topLevelItem(i)->childCount(); j++)
        {
            QTreeWidgetItem *item = ui->sensor_data_tree->topLevelItem(i)->child(j);
            item->setText(2, nowData[count]);
            QString now = nowData[count];
            QString pre = preData[count];
            float n = now.toFloat();
            float p = pre.toFloat();

            if(n==p)      item->setBackground(2, Qt::white);
            else if(n>p)  item->setBackground(2, Qt::red);
            else if(n<p)  item->setBackground(2, Qt::green);

            count ++;
        }
    }

    preData = nowData;
}

void MainWindow::treeWidget(QString place, int a, int b, int c)
{
    // TreeWidget에 parent, child를 등록하기 위해 모듈화한 함수로 호출하여 세대번호 및 센서 개수 하드코딩
    QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(ui->sensor_data_tree);
    topLevelItem->setText(0, place);
    for(int i=0; i<a; i++)
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem();
        treeItem->setText(0,QString("%1").arg(i));
        treeItem->setText(1,"정온식");
        topLevelItem->addChild(treeItem);
        treeItem->setTextAlignment(2, Qt::AlignCenter);
    }
    for(int i=a; i<a+b; i++)
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem();
        treeItem->setText(0,QString("%1").arg(i));
        treeItem->setText(1,"차등식");
        topLevelItem->addChild(treeItem);
        treeItem->setTextAlignment(2, Qt::AlignCenter);
    }
    if(b==0 or a==0)
    {
        for(int i=a+b; i<b+c; i++)
        {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem();
            treeItem->setText(0,QString("%1").arg(i));
            treeItem->setText(1,"광전식");
            topLevelItem->addChild(treeItem);
            treeItem->setTextAlignment(2, Qt::AlignCenter);
        }
    }
    else
    {
        for(int i=a+b; i<=b+c; i++)
        {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem();
            treeItem->setText(0,QString("%1").arg(i));
            treeItem->setText(1,"광전식");
            topLevelItem->addChild(treeItem);
            treeItem->setTextAlignment(2, Qt::AlignCenter);
        }
    }

}

void MainWindow::addLog(QString context)
{
    // TableWidget에 Log추가

    ui->logTableWidget->insertRow(ui->logTableWidget->rowCount()); // Row 추가
    int index = ui->logTableWidget->rowCount() - 1;

    QTime time = QTime::currentTime();
    ui->logTableWidget->setItem(index, 0, new QTableWidgetItem(time.toString("hh:mm:ss.zzz")));
    ui->logTableWidget->setItem(index, 1, new QTableWidgetItem(context));
    ui->logTableWidget->scrollToBottom();
}

void MainWindow::addpath(QString path)
{
    // TableWidget에 피난경로추가

    ui->analyLogTableWidget->insertRow(ui->analyLogTableWidget->rowCount()); // Row 추가
    int index = ui->analyLogTableWidget->rowCount() - 1;

    QTime time = QTime::currentTime();
    ui->analyLogTableWidget->setItem(index, 0, new QTableWidgetItem(time.toString("hh:mm:ss.zzz")));
    ui->analyLogTableWidget->setItem(index, 1, new QTableWidgetItem(path));
    ui->analyLogTableWidget->scrollToBottom();
}
