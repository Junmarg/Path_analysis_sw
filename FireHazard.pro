QT     += core gui sql printsupport opengl

greaterThan(QT_MAJOR_VERSION, 6): QT += widgets

CONFIG += c++17 -fuse-ld=gold
          # build error : https://sourceware.org/bugzilla/show_bug.cgi?id=25135


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Setting.cpp \
    analysis/dataAnalyzer.cpp \
    analysis/dataLookup.cpp \
    analysis/dayDataCollector.cpp \
    analysis/modelRunThread.cpp \
    analysis/realTimeDataCollector.cpp \
    main.cpp \
    mainwindow.cpp \
    openGL/BoxMesh.cpp \
    openGL/BoxObject.cpp \
    openGL/GridObject.cpp \
    openGL/KeyboardMouseHandler.cpp \
    openGL/OpenGLWindow.cpp \
    openGL/PathObject.cpp \
    openGL/SceneView.cpp \
    openGL/ShaderProgram.cpp \
    openGL/Transform3D.cpp \
    plot/qcustomplot/qcustomplot.cpp

HEADERS += \
    Setting.h \
    analysis/dataAnalyzer.h \
    analysis/dataLookup.h \
    analysis/dayDataCollector.h \
    analysis/matplotlibcpp.h \
    analysis/modelRunThread.h \
    analysis/pythonWrapper.h \
    analysis/realTimeDataCollector.h \
    mainwindow.h \
    openGL/BoxMesh.h \
    openGL/BoxObject.h \
    openGL/Camera.h \
    openGL/GridObject.h \
    openGL/KeyboardMouseHandler.h \
    openGL/OpenGLWindow.h \
    openGL/PathObject.h \
    openGL/SceneView.h \
    openGL/ShaderProgram.h \
    openGL/Transform3D.h \
    openGL/Vertex.h \
    plot/qcustomplot/qcustomplot.h

FORMS += \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# path
win32:
    CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/release/ -lmysqlclient
else:
    win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/debug/ -lmysqlclient
else:
    unix: LIBS +=   -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ \
                    /usr/lib/x86_64-linux-gnu/libcpprest.so \
                    /usr/local/lib/libtensorflow.so.2 \
                    -lpython3.8 -lcrypto -lssl -DWITHOUT_NUMPY -lmysqlclient
    # libcpprest.so -> 터미널에서 dpkg -L libcpprest-dev 사용하여 위치 찾아야 함

INCLUDEPATH += /usr/include/cpprest /usr/include/python3.8 /usr/local/lib/python3.8/dist-packages/numpy/core/include
    # cpprest depath 추가

RESOURCES += \
    openGL/fh.qrc

DISTFILES += \
    model/saved_model/keras_metadata.pb \
    model/saved_model/saved_model.pb \
    model/saved_model/variables/variables.data-00000-of-00001 \
    model/saved_model/variables/variables.index \
    openGL/shaders/grid.frag \
    openGL/shaders/grid.vert \
    openGL/shaders/simple.frag \
    openGL/shaders/simple.vert \
    openGL/shaders/withWorldAndCamera.vert \
