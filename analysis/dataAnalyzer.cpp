#include "dataAnalyzer.h"
#define V 100

dataAnalyzer::dataAnalyzer()
{
    mapInit();
    cppflowTest();
}

tuple<QString, QVector<QString>>dataAnalyzer::pathAnalyer(QString start, QVector<QString> data)
{
    // 센서 데이터를 받아와 최단 경로 분석하는 함수

    // data => sensor data
    // data로 path 가중치 변환 해야 함

    QString start_name = start;

    // start : "201", "202" ...
    // get2string을 통해 "room01", "room02" ... 변환
    // string2num을 통해 node에 저장된 인덱스로 자동 변환
    // 각 노드로의 최단 경로/값 계산
    serchShortestPath(string2num(gen2string(start_name)));

    // 경로를 담을 벡터 선언
    vector<int> forPrint;
    QVector<QString> path;
    QString smallest;
    // 선택한 노드로부터 최단 경로 역추적
    int temp = string2num("exit00");
    while(temp)
    {
        forPrint.push_back(temp);
        temp = route[temp];
        if(temp>100) break;
    }
    int size = forPrint.size();

    for(int i= size-1; i>=0; i--)
    {
        path << num2string(forPrint[i]);
    }


    // 출력 : 최소 비용, 경로 노드 수
    //qDebug() << "최소 비용" << d[string2num("exit00")] << "," << "노드 수 " << size;
    smallest = "최단 경로 : ";
    for(int i = size - 1; i >= 0; i--)
    {
        //smallest += "[" + QString::number(forPrint[i]) + "]" + " ";
        smallest += num2string(forPrint[i]);

        if(i!=0) smallest += " -> ";
    }
    //qDebug() << smallest;

    return {smallest, path};
}

void dataAnalyzer::serchShortestPath(int start)
{
    // 최단 경로 알고리즘 * Dijkstra

    fill(d, d+100, INF);
    fill(route, route+100, 0);

    priority_queue<pair<int,int>>pq;                                // 거리, 노드 인덱스
    pq.push({0,start});                                             // 시작 노드로 가기위한 최단 경로는 0으로 설정하여, 큐에 삽입.
    d[start]=0;

    while(!pq.empty())
    {
        int dist = -pq.top().first;                                 // 현재 노드까지의 비용
        int now = pq.top().second;                                  // 현재 노드
        pq.pop();

        if(d[now]<dist)                                             // 이미 최단경로를 체크한 노드인 경우 패스
            continue;

        for(float i=0; i<graph[now].size(); i++)
        {
            int cost = dist+graph[now][i].second;                   // 거쳐서 가는 노드의 비용을 계산
                                                                    // 현재노드까지 비용 + 다음 노드 비용
            if(cost<d[graph[now][i].first])                         // 비용이 더 작다면 최단경로 테이블 값을 갱신.
            {
                route[graph[now][i].first] = now;
                d[graph[now][i].first]=cost;
                pq.push(make_pair(-cost,graph[now][i].first));
            }
        }
    }
}

void dataAnalyzer::mapInit()
{
    /**
    * 1층
    * stair01      ->  exit00  : 1
    * exit         ->  stair01 : 2
    *
    * 2층
    * room01       ->  exit01  : 2, floor01 : 1
    * room02       ->  exit02  : 2, floor02 : 1
    * terrace01    ->  exit08  : 2, floor03 : 1
    * room03       ->  exit03  : 2, floor04 : 1
    * room04       ->  exit04  : 2, floor05 : 1
    * room05       ->  exit05  : 2, floor06 : 1
    * room06       ->  exit06  : 2, floor07 : 1
    * room07       ->  exit07  : 2, floor07 : 1
    * floor01      ->  room01  : 2, floor02 : 1
    * floor02      ->  room02  : 2, floor01 : 2, floor03 : 1
    * floor03      ->  floor02 : 2, floor04 : 1, terrace01 : 2
    * floor04      ->  room03  : 2, floor03 : 2, floor05 : 2, stair02 : 1
    * floor05      ->  room04  : 2, floor04 : 2, floor06 : 2, stair02 : 1
    * floor06      ->  room05  : 2, floor05 : 1, floor07 : 2
    * floor07      ->  room06  : 2, room07  : 2, floor06 : 1
    * stair02      ->  floor04 : 2, floor05 : 2, stair01 : 1, stair03 : 2
    *
    * 3층
    * room08       ->  exit09  : 2, floor08 : 1
    * room09       ->  exit10  : 2, floor09 : 1
    * terrace02    ->  exit16  : 2, floor10 : 1
    * room10       ->  exit11  : 2, floor11 : 1
    * room11       ->  exit12  : 2, floor12 : 1
    * room12       ->  exit13  : 2, floor13 : 1
    * room13       ->  exit14  : 2, floor14 : 1
    * room14       ->  exit15  : 2, floor14 : 1
    * floor08      ->  room08  : 2, floor09 : 1
    * floor09      ->  room09  : 2, floor08 : 2, floor10 : 1
    * floor10      ->  floor09 : 2, floor11 : 1, terrace02 : 2
    * floor11      ->  room10  : 2, floor10 : 2, floor12 : 2, stair02 : 1
    * floor12      ->  room11  : 2, floor11 : 2, floor13 : 2, stair02 : 1
    * floor13      ->  room12  : 2, floor12 : 1, floor14 : 2
    * floor14      ->  room13  : 2, room14  : 2, floor13 : 1
    * stair03      ->  floor11 : 2, floor12 : 2, stair02 : 1, stair04 : 2
    *
    * 4층
    * room15       ->  exit17  : 2, floor15 : 1
    * room16       ->  exit18  : 2, floor16 : 1
    * terrace02    ->  exit24  : 2, floor17 : 1
    * room17       ->  exit19  : 2, floor18 : 1
    * room18       ->  exit20  : 2, floor19 : 1
    * room19       ->  exit21  : 2, floor20 : 1
    * room20       ->  exit22  : 2, floor21 : 1
    * room21       ->  exit23  : 2, floor21 : 1
    * floor15      ->  room15  : 2, floor16 : 1
    * floor16      ->  room16  : 2, floor15 : 2, floor17 : 1
    * floor17      ->  floor16 : 2, floor18 : 1, terrace03 : 2
    * floor18      ->  room17  : 2, floor17 : 2, floor19 : 2, stair04 : 1
    * floor19      ->  room18  : 2, floor18 : 2, floor20 : 2, stair04 : 1
    * floor20      ->  room19  : 2, floor19 : 1, floor21 : 2
    * floor21      ->  room20  : 2, room21  : 2, floor20 : 1
    * stair04      ->  floor18 : 2, floor19 : 2, stair02 : 1, stair05 : 2
    *
    * 5층
    * room22       ->  exit25  : 2, floor22 : 1
    * room23       ->  exit26  : 2, floor23 : 1
    * terrace03    ->  exit32  : 2, floor24 : 1
    * room24       ->  exit27  : 2, floor25 : 1
    * room25       ->  exit28  : 2, floor26 : 1
    * room26       ->  exit29  : 2, floor27 : 1
    * room27       ->  exit30  : 2, floor28 : 1
    * room28       ->  exit31  : 2, floor28 : 1
    * floor22      ->  room22  : 2, floor23 : 1
    * floor23      ->  room23  : 2, floor22 : 2, floor24 : 1
    * floor24      ->  floor23 : 2, floor25 : 1, terrace05 : 2
    * floor25      ->  room24  : 2, floor24 : 2, floor26 : 2, stair05 : 1
    * floor26      ->  room25  : 2, floor25 : 2, floor27 : 2, stair05 : 1
    * floor27      ->  room26  : 2, floor26 : 1, floor28 : 2
    * floor28      ->  room27  : 2, room28  : 2, floor27 : 1
    * stair05      ->  floor25 : 2, floor26 : 2, stair04 : 1, stair06 : 2
    **/

    // C++ 에서는 Python의 Dictionary 타입이 존재하지 않음
    // map과 QVector로 Dictionary 형식과 비슷하게 구현

    tmp.clear();
    path.clear();
    node.clear();

    // index 0번에 할당하지 않기 위한 dumy map insert
    // 0번에 할당되는 경우 경로 추적 while문 err 발생
    tmp = {{"a",99}};
    path.insert({"a",tmp});
    tmp.clear();

    #define floor_01 {
    tmp = {{"stair02", 2}, {"exit00", 1}};
    path.insert({"stair01", tmp});
    tmp.clear();

    tmp = {{"stair01", 1}};
    path.insert({"exit00", tmp});
    tmp.clear();
    #define floor_01 }

    #define floor_02 {
    tmp = {{"exit01", 2}, {"floor01", 1}};
    path.insert({"room201", tmp});
    tmp.clear();

    tmp = {{"exit02", 2}, {"floor02", 1}};
    path.insert({"room202", tmp});
    tmp.clear();

    tmp = {{"exit08", 2}, {"floor03", 1}};
    path.insert({"terrace01", tmp});
    tmp.clear();

    tmp = {{"exit03", 2}, {"floor04", 1}};
    path.insert({"room203", tmp});
    tmp.clear();

    tmp = {{"exit04", 2}, {"floor05", 1}};
    path.insert({"room204", tmp});
    tmp.clear();

    tmp = {{"exit05", 2}, {"floor06", 1}};
    path.insert({"room205", tmp});
    tmp.clear();

    tmp = {{"exit06", 2}, {"floor07", 1}};
    path.insert({"room206", tmp});
    tmp.clear();

    tmp = {{"exit07", 2}, {"floor07", 1}};
    path.insert({"room207", tmp});
    tmp.clear();

    tmp = {{"room201", 2}, {"floor02", 1}};
    path.insert({"floor01", tmp});
    tmp.clear();

    tmp = {{"room202", 2}, {"floor01", 2}, {"floor03", 1}};
    path.insert({"floor02", tmp});
    tmp.clear();

    tmp = {{"terrace01", 2}, {"floor02", 2}, {"floor04", 1}};
    path.insert({"floor03", tmp});
    tmp.clear();

    tmp = {{"room203", 2}, {"floor03", 2}, {"floor05", 2}, {"stair02", 1}};
    path.insert({"floor04", tmp});
    tmp.clear();

    tmp = {{"room204", 2}, {"floor04", 2}, {"floor06", 2}, {"stair02", 1}};
    path.insert({"floor05", tmp});
    tmp.clear();

    tmp = {{"room205", 2}, {"floor05", 1}, {"floor07", 2}};
    path.insert({"floor06", tmp});
    tmp.clear();

    tmp = {{"room206", 2}, {"room207", 2}, {"floor06", 1}};
    path.insert({"floor07", tmp});
    tmp.clear();

    tmp = {{"floor04", 2}, {"floor05", 2}, {"stair01", 1}, {"stair03", 1}};
    path.insert({"stair02", tmp});
    tmp.clear();

    # define floor_02 }

    # define floor_03 {
    tmp = {{"exit09", 2}, {"floor08", 1}};
    path.insert({"room08", tmp});
    tmp.clear();

    tmp = {{"exit10", 2}, {"floor09", 1}};
    path.insert({"room09", tmp});
    tmp.clear();

    tmp = {{"exit11", 2}, {"floor10", 1}};
    path.insert({"terrace02", tmp});
    tmp.clear();

    tmp = {{"exit16", 2}, {"floor11", 1}};
    path.insert({"room10", tmp});
    tmp.clear();

    tmp = {{"exit12", 2}, {"floor12", 1}};
    path.insert({"room11", tmp});
    tmp.clear();

    tmp = {{"exit13", 2}, {"floor13", 1}};
    path.insert({"room12", tmp});
    tmp.clear();

    tmp = {{"exit14", 2}, {"floor14", 1}};
    path.insert({"room13", tmp});
    tmp.clear();

    tmp = {{"exit15", 2}, {"floor14", 1}};
    path.insert({"room14", tmp});
    tmp.clear();

    tmp = {{"room08", 2}, {"floor09", 1}};
    path.insert({"floor08", tmp});
    tmp.clear();

    tmp = {{"room09", 2}, {"floor08", 2}, {"floor10", 1}};
    path.insert({"floor09", tmp});
    tmp.clear();

    tmp = {{"terrace02", 2}, {"floor09", 2}, {"floor11", 1}};
    path.insert({"floor10", tmp});
    tmp.clear();

    tmp = {{"room10", 2}, {"floor10", 2}, {"floor12", 2}, {"stair03", 1}};
    path.insert({"floor11", tmp});
    tmp.clear();

    tmp = {{"room11", 2}, {"floor11", 2}, {"floor13", 2}, {"stair03", 1}};
    path.insert({"floor12", tmp});
    tmp.clear();

    tmp = {{"room12", 2}, {"floor12", 1}, {"floor14", 2}};
    path.insert({"floor13", tmp});
    tmp.clear();

    tmp = {{"room13", 2}, {"room14", 2}, {"floor13", 1}};
    path.insert({"floor14", tmp});
    tmp.clear();

    tmp = {{"floor11", 2}, {"floor12", 2}, {"stair02", 1}, {"stair04", 1}};
    path.insert({"stair03", tmp});
    tmp.clear();

    # define floor_03 }

    # define floor_04 {
    tmp = {{"exit17", 2}, {"floor15", 1}};
    path.insert({"room15", tmp});
    tmp.clear();

    tmp = {{"exit18", 2}, {"floor16", 1}};
    path.insert({"room16", tmp});
    tmp.clear();

    tmp = {{"exit24", 2}, {"floor17", 1}};
    path.insert({"terrace03", tmp});
    tmp.clear();

    tmp = {{"exit19", 2}, {"floor18", 1}};
    path.insert({"room17", tmp});
    tmp.clear();

    tmp = {{"exit20", 2}, {"floor19", 1}};
    path.insert({"room18", tmp});
    tmp.clear();

    tmp = {{"exit21", 2}, {"floor20", 1}};
    path.insert({"room19", tmp});
    tmp.clear();

    tmp = {{"exit22", 2}, {"floor21", 1}};
    path.insert({"room20", tmp});
    tmp.clear();

    tmp = {{"exit23", 2}, {"floor21", 1}};
    path.insert({"room21", tmp});
    tmp.clear();

    tmp = {{"room15", 2}, {"floor16", 1}};
    path.insert({"floor15", tmp});
    tmp.clear();

    tmp = {{"room16", 2}, {"floor15", 2}, {"floor17", 1}};
    path.insert({"floor16", tmp});
    tmp.clear();

    tmp = {{"terrace03", 2}, {"floor16", 2}, {"floor18", 1}};
    path.insert({"floor17", tmp});
    tmp.clear();

    tmp = {{"room17", 2}, {"floor17", 2}, {"floor18", 2}, {"stair04", 1}};
    path.insert({"floor18", tmp});
    tmp.clear();

    tmp = {{"room18", 2}, {"floor18", 2}, {"floor20", 2}, {"stair04", 1}};
    path.insert({"floor19", tmp});
    tmp.clear();

    tmp = {{"room19", 2}, {"floor19", 1}, {"floor21", 2}};
    path.insert({"floor20", tmp});
    tmp.clear();

    tmp = {{"room20", 2}, {"floor20", 1}, {"room21", 2}};
    path.insert({"floor21", tmp});
    tmp.clear();

    tmp = {{"floor18", 2}, {"floor19", 2}, {"stair03", 1}, {"stair05", 1}};
    path.insert({"stair04", tmp});
    tmp.clear();

    # define floor_04 }

    # define floor_05 {
    tmp = {{"exit25", 2}, {"floor22", 1}};
    path.insert({"room22", tmp});
    tmp.clear();

    tmp = {{"exit26", 2}, {"floor23", 1}};
    path.insert({"room23", tmp});
    tmp.clear();

    tmp = {{"exit32", 2}, {"floor24", 1}};
    path.insert({"terrace04", tmp});
    tmp.clear();

    tmp = {{"exit27", 2}, {"floor25", 1}};
    path.insert({"room24", tmp});
    tmp.clear();

    tmp = {{"exit28", 2}, {"floor26", 1}};
    path.insert({"room25", tmp});
    tmp.clear();

    tmp = {{"exit29", 2}, {"floor27", 1}};
    path.insert({"room26", tmp});
    tmp.clear();

    tmp = {{"exit30", 2}, {"floor28", 1}};
    path.insert({"room27", tmp});
    tmp.clear();

    tmp = {{"exit31", 2}, {"floor28", 1}};
    path.insert({"room28", tmp});
    tmp.clear();

    tmp = {{"room22", 2}, {"floor23", 1}};
    path.insert({"floor22", tmp});
    tmp.clear();

    tmp = {{"room23", 2}, {"floor22", 2}, {"floor24", 1}};
    path.insert({"floor23", tmp});
    tmp.clear();

    tmp = {{"terrace04", 2}, {"floor23", 2}, {"floor25", 1}};
    path.insert({"floor24", tmp});
    tmp.clear();

    tmp = {{"room24", 2}, {"floor24", 2}, {"floor25", 2}, {"stair05", 1}};
    path.insert({"floor25", tmp});
    tmp.clear();

    tmp = {{"room25", 2}, {"floor25", 2}, {"floor27", 2}, {"stair05", 1}};
    path.insert({"floor26", tmp});
    tmp.clear();

    tmp = {{"room26", 2}, {"floor26", 1}, {"floor28", 2}};
    path.insert({"floor27", tmp});
    tmp.clear();

    tmp = {{"room27", 2}, {"floor27", 1}, {"room28", 2}};
    path.insert({"floor28", tmp});
    tmp.clear();

    tmp = {{"floor25", 2}, {"floor26", 2}, {"stair04", 1}, {"stair06", 1}};
    path.insert({"stair05", tmp});
    tmp.clear();
    # define floor_05}


    //문자형(QString) 노드를 정수형(int) 노드 이름으로 mapping
    int count = 0;
    for(auto &kv : path)
    {
        if(node.find(kv.first) != node.end()) qDebug();
        else
        {
            node.insert({kv.first, count});
            count ++;
        }

        for(auto &kv2 : kv.second)
        {
            if(node.find(kv2.first) != node.end()) qDebug();
            else
            {
                node.insert({kv2.first, count});
                count ++;
                break;
            }
        }
    }

    for(auto &kv : path)
    {
        for(auto &kv2 : kv.second)
        {
            graph[node[kv.first]].push_back({node[kv2.first], kv2.second});
        }
    }

#if 0 // map 데이터 확인, if 1 == On, if 0 == Off
    for(auto &kv : path)
    {
        qDebug() << kv.first;
        //qDebug() << kv.first << kv.second;
        for(auto &va : kv.second)
        {
            qDebug() << va.first << va.second;
        }
    }
#endif

#if 0 // node 데이터 확인, if 1 == On, if 0 == Off
    for(auto &kv2 : node)
        qDebug() << kv2.first << kv2.second;
#endif
}

QString dataAnalyzer::num2string(int node_num)
{
    // node의 index를 string으로 변경하는 함수
    for(auto &kv : node)
        if(node_num==kv.second) return kv.first;

    return "";
}

int dataAnalyzer::string2num(QString node_name)
{
    // node의 string을 index로 변경하는 함수
    for(auto &kv : node)
        if(node_name==kv.first) return kv.second;

    return 0;
}

QString dataAnalyzer::gen2string(QString gen)
{
    // treeWidget에서 얻은 세대 이름(String)을 경로 계산에 사용되는 형식으로 변경

    if(gen=="201") return "room201";
    if(gen=="202") return "room202";
    if(gen=="203") return "room203";
    if(gen=="204") return "room204";
    if(gen=="205") return "room205";
    if(gen=="206") return "room206";
    if(gen=="207") return "room207";
    if(gen=="301") return "room08";
    if(gen=="302") return "room09";
    if(gen=="303") return "room10";
    if(gen=="304") return "room11";
    if(gen=="305") return "room12";
    if(gen=="306") return "room13";
    if(gen=="307") return "room14";
    if(gen=="401") return "room15";
    if(gen=="402") return "room16";
    if(gen=="403") return "room17";
    if(gen=="404") return "room18";
    if(gen=="405") return "room19";
    if(gen=="406") return "room20";
    if(gen=="407") return "room21";
    if(gen=="501") return "room22";
    if(gen=="502") return "room23";
    if(gen=="503") return "room24";
    if(gen=="504") return "room25";
    if(gen=="505") return "room26";
    if(gen=="506") return "room27";
    if(gen=="507") return "room28";

    if(gen=="floor_1") return "stair02";
    if(gen=="floor_2") return "room201";
    if(gen=="floor_3") return "room08";
    if(gen=="floor_4") return "room15";
    if(gen=="floor_5") return "room22";
    if(gen=="roof") return "room22";
    if(gen=="under") return "stair02";
}

void dataAnalyzer::tensorflowTest()
{
    // Cppflow가 아닌 Tensorflow 모듈을 직접 연동하여 구현한 함수
    // 실제로 사용하지 않으나 추후 사용 가능성을 고려해 테스트 한 코드를 보관
    /*
    qDebug() << "Hello from TensorFlow C library version " << TF_Version();

    auto *run_options = TF_NewBufferFromString("", 0);
    auto *session_options = TF_NewSessionOptions();
    auto *graph = TF_NewGraph();
    auto *status = TF_NewStatus();
    std::array<char const *, 1> tags{ "serve" };

    auto* session = TF_LoadSessionFromSavedModel(session_options, run_options,
                                                 "model2/saved_model", tags.data(), tags.size(),
                                                 graph, nullptr, status);
    if (TF_GetCode(status) != TF_OK) {
        qDebug() << TF_Message(status) << '\n';
    }
    auto *input_op = TF_GraphOperationByName(graph, "serving_default_input");
    if (input_op == nullptr) {
        qDebug() << "Failed to find graph operation\n";
    }

    auto *output_op = TF_GraphOperationByName(graph, "StatefulPartitionedCall");
    if (output_op == nullptr) {
        qDebug() << "Failed to find graph operation\n";
    }

    std::array<TF_Output, 1> input_ops = { TF_Output{ input_op, 0 } };
    std::array<TF_Output, 1> output_ops = { TF_Output{ output_op, 0 } };

    std::array<int, 2> x{ 1, 0 };
    std::vector<std::array<int, 2>> inputs{ x };

    std::array<int64_t, 2> const dims{ static_cast<int64_t>(inputs.size()), static_cast<int64_t>(x.size()) };
    void *data = (void *) inputs.data();
    std::size_t const ndata = inputs.size() * x.size() * TF_DataTypeSize(TF_INT32);

    auto const deallocator = [](void *, std::size_t, void *) {}; // unused deallocator because of RAII

    auto *input_tensor = TF_NewTensor(TF_INT32, dims.data(), dims.size(), data, ndata, deallocator, nullptr);
    std::array<TF_Tensor *, 1> input_values{ input_tensor };

    std::array<TF_Tensor *, 1> output_values{};

    TF_SessionRun(session,
                  run_options,
                  input_ops.data(), input_values.data(), input_ops.size(),
                  output_ops.data(), output_values.data(), output_ops.size(),
                  nullptr, 0,
                  nullptr,
                  status);
    if (TF_GetCode(status) != TF_OK) {
        qDebug() << TF_Message(status) << '\n';
    }

    auto *output_tensor = static_cast<std::array<float, 1> *>(TF_TensorData(output_values[0]));
    std::vector<std::array<float, 1>> outputs{ output_tensor, output_tensor + inputs.size() };

    qDebug() << "output: " << outputs[0][0] << '\n'; // output: 0.907109

    TF_DeleteTensor(input_values[0]);
    TF_DeleteTensor(output_values[0]);
    TF_DeleteBuffer(run_options);
    TF_DeleteSessionOptions(session_options);
    TF_DeleteSession(session, status);
    TF_DeleteGraph(graph);
    TF_DeleteStatus(status);
    */
}

void dataAnalyzer::cppflowTest()
{
    // Cppflow 라이브러리를 사용하여 모델 load 및 예측까지 하는 함수 구현
    //QElapsedTimer timer;
    //timer.start();

    cppflow::model model("model/gaussianModel");

    for(int i=0; i<163; i++)
    {
        // CNN - pix2pix gaussian Model
        auto input = cppflow::decode_png(cppflow::read_file(std::string("image/0/nomal_00001.jpg")));
        input = cppflow::expand_dims(input, 0);
        auto resized = cppflow::resize_bicubic(input, cppflow::tensor({256,256}),true);
        resized = cppflow::cast(resized, TF_UINT8, TF_FLOAT);

        // terminal : saved_model_cli show --dir model_gaussian/ --all
        auto output = model({{"serving_default_conv2d_9_input:0", resized}}, {"StatefulPartitionedCall:0"});

        // 분석 결과 출력
        //std::cout << "Content of output:" << output[0] << std::endl;
        //std::cout << "Content of output:" << cppflow::softmax(output[0]) << std::endl;
        //std::cout << "Content of output:" << cppflow::arg_max(output[0], 1) << std::endl;
    }
    qDebug() << "163회 분석시간" ;//<< timer.elapsed();
}

void dataAnalyzer::matplot_numpy_lib_Test()
{/*
    std::vector<double> y = {1, 3, 2, 4};
    plt::plot(y);
    plt::savefig("temp/minimal.pdf");*/
}

auto dataAnalyzer::recPlot(nc::NdArray<float> s, float eps=0, float steps=0)
{
    // RP 알고리즘 c++ porting
    // numcpp 라이브러리 사용

    if(eps==0) eps=0.01;
    if(steps==0) steps=10;

    int n = s.size();
    s = nc::repeat(s, n, 1);
    auto z = nc::floor(nc::abs(s-s.transpose())/eps);
    z[z>steps] = steps;

    return z;
}

void dataAnalyzer::num2pix()
{
    // RP 알고리즘을 사용/테스트 하는 함수
    auto a = nc::random::randFloat<float>(nc::Shape(1,100), 0, 1);
//    std::cout << a.shape();
//    std::cout << a.size();

    auto cc = recPlot(a, 0, 0);
    // std::cout << cc;
    // std::cout << "SHAPE :" << cc.shape();
}

int dataAnalyzer::run_python()
{
    // Python 코드 실행을 위한 initialze 함수
    // python 파일 로드 또는 simplestring에 직접 입력하여 실행 가능

    //Py_SetProgramName(program);  /* optional but recommended */

    Py_Initialize();

    //PyRun_SimpleString("import numpy as np");
    //PyRun_SimpleString("print(np.random.random(100))");

    //FILE* fp = nullptr;

    //auto ret = fopen(pathname, "r");
    //if(ret != 0) return -1;

    //auto iret = PyRun_SimpleFile(fp, pathname);
    //if(iret < 0) return iret;
    Py_Finalize();

    //fclose(fp);

    return 0;
}
