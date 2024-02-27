#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include <stdio.h>
#include <iostream>
#include <limits.h>
#include <vector>
#include <queue>
#include <QDebug>
#include <time.h>
#include "tensorflow/c/c_api.h"
#include "cppflow/cppflow.h"
//#include "plot/qcustomplot/qcustomplot.h"
#include "pythonWrapper.h"
#include <boost/python/numpy.hpp>
#include <boost/python.hpp>
#include <NumCpp.hpp>

namespace p = boost::python;
namespace np = boost::python::numpy;

using namespace std;

#define INF 1e9 // 무한을 의미하는 값으로 10억을 설정

class dataAnalyzer
{
public:
    dataAnalyzer();

    tuple<QString, QVector<QString>> pathAnalyer(QString start, QVector<QString> data);
    void mapInit();
    void serchShortestPath(int start);

    void weightChange(map<QString, map<QString,float>>, QString start, QString end, float weight);
    QString num2string(int node_num);
    int string2num(QString node_name);
    QString gen2string(QString gen);
    auto recPlot(nc::NdArray<float> s, float eps, float steps);
    void num2pix();
    void tensorflowTest();
    void cppflowTest();
    void matplot_numpy_lib_Test();

    int run_python();
private:
    map<QString, map<QString,float>> path;
    map<QString,float> tmp;
    map<QString,int> node;
    vector<pair<int, int>> graph[100];
    int d[100]; // 최단 거리 테이블
    int route[100]; // 최단 경로 리스트

    // number to pix
    //QCustomPlot *print_jpg_plot;


};

#endif // DATAANALYZER_H
