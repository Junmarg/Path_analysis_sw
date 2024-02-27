
#include "modelRunThread.h"

modelRunThread::modelRunThread(QObject *parent) : QThread(parent)
{

}

modelRunThread::~modelRunThread()
{

}

void modelRunThread::run()
{
    cppflow::model load_model("model/gaussianModel");

    while(running)
    {
        if(sensor_data.empty()==true){}
        else
        {
            // Cppflow 라이브러리를 사용하여 모델 load 및 예측까지 하는 함수 구현
            model_output.clear();

            // 분석 시작 시간 Push
            QTime time = QTime::currentTime();
            model_output.push_back(time.toString("hh:mm:ss.zzz"));

            QElapsedTimer timer;
            timer.start();

            for(int i=0; i<174; i++)
            {
                // CNN - pix2pix gaussian Model
                auto input = cppflow::decode_png(cppflow::read_file(std::string("image/1/fire_00011.jpg")));
                input = cppflow::expand_dims(input, 0);
                auto resized = cppflow::resize_bicubic(input, cppflow::tensor({256,256}),true);
                resized = cppflow::cast(resized, TF_UINT8, TF_FLOAT);

                // terminal : saved_model_cli show --dir model_gaussian/ --all
                auto output = load_model({{"serving_default_conv2d_9_input:0", resized}}, {"StatefulPartitionedCall:0"});

                // 분석 결과 출력
                //std::cout << "Content of output:" << output[0] << std::endl;
                //std::cout << "Content of output:" << cppflow::softmax(output[0]) << std::endl;
                //std::cout << "Content of output:" << cppflow::arg_max(output[0], 1) << std::endl;
            }
            // 분석 종료 시간 Push
            QTime time2 = QTime::currentTime();
            model_output.push_back(time2.toString("hh:mm:ss.zzz"));

            // 분석 결과값 Push
            model_output.push_back(QString::number(timer.elapsed()));

            Q_EMIT output(model_output);
            sensor_data.clear();

        }
        usleep(100000);
    }
}

void modelRunThread::end()
{
    running=false;

}
