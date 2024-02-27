#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include <QTimer>
#include <QDateTime>

#include "openGL/SceneView.h"
#include "analysis/dataAnalyzer.h"
#include "plot/qcustomplot/qcustomplot.h"
#include "analysis/realTimeDataCollector.h"
#include "analysis/dayDataCollector.h"
#include "analysis/dataLookup.h"
#include "analysis/modelRunThread.h"
#include "Setting.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openGLInit();
    void customplotInit();
    void dataCollectorInit();
    void setEvent();
    void plotInit();
    void treeWidgetInit();
    void treeWidget(QString place, int a, int b, int c);
    void systemLogChange(int Case);
    // logTableWidget
    void addLog(QString context);

private:
    Ui::MainWindow *ui;

    // Thread
    QTimer *timer;
    QElapsedTimer etime;

    // QTimer
    QTimer *realTimeDataTimer;
    QTimer *dayDataTimer;

    // OpenGL
    SceneView * m_sceneView;
    /*
    // Plot
    QCustomPlot *now_graph;
    QCustomPlot *day_graph;
    QCustomPlot *print_jpg_plot;
    QCPAxisRect *wideAxisRect;
    QCPBarsGroup *_group_1;
    QCPBars *bars;
    QVector<QString> _data;
    QVector<QString> _timestamp;
    QStringList _dataList;
    QVector<double> dayDatax;
    QVector<double> dayDatay;
    */

    // Treewidget
    typedef struct
    {
        QString name = "201";
        QString parent;
        QString sensor;
        int index_parent;
        int index_child;
    }treeDataStruct;

    QVector<treeDataStruct> treeCurrentData;
    bool plotStart = true;
    QVector<QString> nowData;
    QVector<QString> preData;

    // Data Processing
    realTimeDataCollector   *rdc;
    dayDataCollector        *ddc;
    dataAnalyzer   da;
    dataLookup     dl;
    bool connected = false;
    QString tmp;
    QByteArray tmp_host_qba;
    QByteArray tmp_user_qba;
    QByteArray tmp_name_qba;
    QByteArray tmp_pass_qba;
    QByteArray tmp_port_qba;

    // analysis Thread
    modelRunThread *mrt_01;
    modelRunThread *mrt_02;
    modelRunThread *mrt_03;
    modelRunThread *mrt_04;
    modelRunThread *mrt_05;

    // Setting
    Setting setting;

private Q_SLOTS:
    void settingBtnClicked();
    void testBtnClicked();
    void setDataTime();
    void dataCollectorRun();
    void dataCollectorTest();

    //graphWidget
    void dataPlot(QVector<QString>);
    void realtimeDataSlot();
    void realTimeDataPlot(QVector<QString> data);
    void dayDataSlot();
    void dayDataxPlot(QVector<double> datax);
    void dayDatayPlot(QVector<double> datay);

    //treeWidget
    void treeExpandBtnClicked();
    void treeCallabBtnClicked();
    void treeCurrentItemChanged(QTreeWidgetItem*,int);
    void plotDataChange();

    //analyer
    void modelOutput(QVector<QString>);
    void addpath(QString path);

};
#endif // MAINWINDOW_H
