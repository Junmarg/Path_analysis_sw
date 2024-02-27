/************************************************************************************

OpenGL with Qt - Tutorial
-------------------------
Autor      : Andreas Nicolai <andreas.nicolai@gmx.net>
Repository : https://github.com/ghorwin/OpenGLWithQt-Tutorial
License    : BSD License,
			 see https://github.com/ghorwin/OpenGLWithQt-Tutorial/blob/master/LICENSE

************************************************************************************/

#include "SceneView.h"

#include <QExposeEvent>
#include <QOpenGLShaderProgram>
#include <QDateTime>

#define SHADER(x) m_shaderPrograms[x].shaderProgram()

SceneView::SceneView() :
	m_inputEventReceived(false)
{
	// tell keyboard handler to monitor certain keys
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_W);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_A);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_S);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_D);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_Q);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_E);
	m_keyboardMouseHandler.addRecognizedKey(Qt::Key_Shift);

	// *** create scene (no OpenGL calls are being issued below, just the data structures are created.

	// Shaderprogram #0 : regular geometry (painting triangles via element index)
    ShaderProgram blocks(":/shaders/withWorldAndCamera.vert",":/shaders/simple.frag");
	blocks.m_uniformNames.append("worldToView");
	m_shaderPrograms.append( blocks );

	// Shaderprogram #1 : grid (painting grid lines)
	ShaderProgram grid(":/shaders/grid.vert",":/shaders/grid.frag");
	grid.m_uniformNames.append("worldToView"); // mat4
	grid.m_uniformNames.append("gridColor"); // vec3
	grid.m_uniformNames.append("backColor"); // vec3
	m_shaderPrograms.append( grid );
	// *** initialize camera placement and model placement in the world

    for(int i=0;i<100;i++)
    {
        ShaderProgram f_path_01(":/shaders/withWorldAndCamera.vert",":/shaders/simple.frag");
        blocks.m_uniformNames.append("worldToView");
        m_shaderPrograms.append( f_path_01 );
    }
    ShaderProgram f(":/shaders/withWorldAndCamera.vert",":/shaders/simple.frag");
    blocks.m_uniformNames.append("worldToView");
    m_shaderPrograms.append( f );

	// move camera a little back and up
    m_camera.translate(0,200,350);
	// look slightly down
    m_camera.rotate(-30, m_camera.right());
	// look slightly right
    m_camera.rotate(0, QVector3D(0.0f, 1.0f, 0.0f));
}

SceneView::~SceneView() {
	m_context->makeCurrent(this);

	for (ShaderProgram & p : m_shaderPrograms)
		p.destroy();

	m_boxObject.destroy();
	m_gridObject.destroy();

    /** new **/
    m_pathObject.destroy();
}

void SceneView::initializeGL() {
	// initialize shader programs
    for (ShaderProgram & p : m_shaderPrograms)
		p.create();

    // tell OpenGL to show only faces whose normal vector points towards us
    glEnable(GL_CULL_FACE);
    // enable depth testing, important for the grid and for the drawing order of several objects
    glEnable(GL_DEPTH_TEST);

	// initialize drawable objects
	m_boxObject.create(SHADER(0));
	m_gridObject.create(SHADER(1));
    m_pathObject.create(SHADER(2));
}

void SceneView::resizeGL(int width, int height) {
	// the projection matrix need to be updated only for window size changes
	m_projection.setToIdentity();
	// create projection matrix, i.e. camera lens
	m_projection.perspective(
				/* vertical angle */ 45.0f,
				/* aspect ratio */   width / float(height),
				/* near */           0.1f,
				/* far */            1000.0f
		);
	// Mind: to not use 0.0 for near plane, otherwise depth buffering and depth testing won't work!

	// update cached world2view matrix
	updateWorld2ViewMatrix();
}

void SceneView::paintGL() {
	// process input, i.e. check if any keys have been pressed
	if (m_inputEventReceived)
		processInput();

	const qreal retinaScale = devicePixelRatio(); // needed for Macs with retina display
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
//	qDebug() << "SceneView::paintGL(): Rendering to:" << width() << "x" << height();

	// set the background color = clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the background color = clear color
	QVector3D backColor(0.1f, 0.15f, 0.3f);
    glClearColor(0.1f, 0.15f, 0.3f, 0.5f);

	QVector3D gridColor(0.5f, 0.5f, 0.7f);

    // *** render boxes
    SHADER(0)->bind();
    SHADER(0)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[0], m_worldToView);
    m_boxObject.render(); // render the boxes
    SHADER(0)->release();
	// *** render grid afterwards ***
	SHADER(1)->bind();
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[0], m_worldToView);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[1], gridColor);
	SHADER(1)->setUniformValue(m_shaderPrograms[1].m_uniformIDs[2], backColor);
    m_gridObject.render(); // render the grid
	SHADER(1)->release();

    if(fire_trigger == true)    // 화재 발생 시 피난 경로 표출
    {
        SHADER(2)->bind();
        SHADER(2)->setUniformValue(m_shaderPrograms[0].m_uniformIDs[0], m_worldToView);
        m_pathObject.render(render); // render the boxes
        SHADER(2)->release();
        renderLater();
    }

#if 0
	// do some animation stuff
	m_transform.rotate(1.0f, QVector3D(0.0f, 0.1f, 0.0f));
	updateWorld2ViewMatrix();
	renderLater();
#endif

	checkInput();
}

void SceneView::keyPressEvent(QKeyEvent *event) {
	m_keyboardMouseHandler.keyPressEvent(event);
	checkInput();
}

void SceneView::keyReleaseEvent(QKeyEvent *event) {
	m_keyboardMouseHandler.keyReleaseEvent(event);
	checkInput();
}

void SceneView::mousePressEvent(QMouseEvent *event) {
	m_keyboardMouseHandler.mousePressEvent(event);
	checkInput();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event) {
	m_keyboardMouseHandler.mouseReleaseEvent(event);
	checkInput();
}

void SceneView::mouseMoveEvent(QMouseEvent *event) {
	checkInput();
}

void SceneView::wheelEvent(QWheelEvent *event) {
	m_keyboardMouseHandler.wheelEvent(event);
	checkInput();
}

void SceneView::checkInput() {
	// this function is called whenever _any_ key/mouse event was issued

	// we test, if the current state of the key handler requires a scene update
	// (camera movement) and if so, we just set a flag to do that upon next repaint
	// and we schedule a repaint

	// trigger key held?
	if (m_keyboardMouseHandler.buttonDown(Qt::RightButton)) {
		// any of the interesting keys held?
		if (m_keyboardMouseHandler.keyDown(Qt::Key_W) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_A) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_S) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_D) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_Q) ||
			m_keyboardMouseHandler.keyDown(Qt::Key_E))
		{
			m_inputEventReceived = true;
//			qDebug() << "SceneView::checkInput() inputEventReceived";
			renderLater();
			return;
		}

		// has the mouse been moved?
		if (m_keyboardMouseHandler.mouseDownPos() != QCursor::pos()) {
			m_inputEventReceived = true;
//			qDebug() << "SceneView::checkInput() inputEventReceived: " << QCursor::pos() << m_keyboardMouseHandler.mouseDownPos();
			renderLater();
			return;
		}
	}
	// scroll-wheel turned?
	if (m_keyboardMouseHandler.wheelDelta() != 0) {
		m_inputEventReceived = true;
		renderLater();
		return;
	}
}

void SceneView::processInput() {
	// function must only be called if an input event has been received
	Q_ASSERT(m_inputEventReceived);
	m_inputEventReceived = false;
//	qDebug() << "SceneView::processInput()";

	// check for trigger key
	if (m_keyboardMouseHandler.buttonDown(Qt::RightButton)) {

		// Handle translations
		QVector3D translation;
		if (m_keyboardMouseHandler.keyDown(Qt::Key_W)) 		translation += m_camera.forward();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_S)) 		translation -= m_camera.forward();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_A)) 		translation -= m_camera.right();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_D)) 		translation += m_camera.right();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_Q)) 		translation -= m_camera.up();
		if (m_keyboardMouseHandler.keyDown(Qt::Key_E)) 		translation += m_camera.up();

		float transSpeed = 0.8f;
		if (m_keyboardMouseHandler.keyDown(Qt::Key_Shift))
			transSpeed = 0.1f;
		m_camera.translate(transSpeed * translation);

		// Handle rotations
		// get and reset mouse delta (pass current mouse cursor position)
		QPoint mouseDelta = m_keyboardMouseHandler.resetMouseDelta(QCursor::pos()); // resets the internal position
		static const float rotatationSpeed  = 0.4f;
		const QVector3D LocalUp(0.0f, 1.0f, 0.0f); // same as in Camera::up()
		m_camera.rotate(-rotatationSpeed * mouseDelta.x(), LocalUp);
		m_camera.rotate(-rotatationSpeed * mouseDelta.y(), m_camera.right());

		// finally, reset "WasPressed" key states
		m_keyboardMouseHandler.clearWasPressedKeyStates();
	}
	int wheelDelta = m_keyboardMouseHandler.resetWheelDelta();
	if (wheelDelta != 0) {
		float transSpeed = 8.f;
		if (m_keyboardMouseHandler.keyDown(Qt::Key_Shift))
            transSpeed = 3.8f;
		m_camera.translate(wheelDelta * transSpeed * m_camera.forward());
	}

	updateWorld2ViewMatrix();
	// not need to request update here, since we are called from paint anyway
}

void SceneView::updateWorld2ViewMatrix() {
	// transformation steps:
	//   model space -> transform -> world space
	//   world space -> camera/eye -> camera view
	//   camera view -> projection -> normalized device coordinates (NDC)
	m_worldToView = m_projection * m_camera.toMatrix() * m_transform.toMatrix();
}

void SceneView::path2render(QVector<QString> path)
{
    // 최단 경로 분석 결과를 메모리로 mapping 해주는 함수
    // ex) room01 -> floor01 에 해당하는 화살표는 0번 메모리에 저장되어 있음

    render.clear();
    //qDebug() << path;
    for(int i=0; i<path.size()-1; i++)
    {
        if(path[i]=="room201" && path[i+1]=="floor01") render << 0;
        else if (path[i] == "room202" && path[i + 1] == "floor02") render << 1;
        else if (path[i] == "room203" && path[i + 1] == "floor04") render << 2;
        else if (path[i] == "room204" && path[i + 1] == "floor05") render << 3;
        else if (path[i] == "room205" && path[i + 1] == "floor06") render << 4;
        else if (path[i] == "room206" && path[i + 1] == "floor07") render << 5;
        else if (path[i] == "room207" && path[i + 1] == "floor07") render << 6;
        else if (path[i] == "floor01" && path[i + 1] == "floor02") render << 7;
        else if (path[i] == "floor02" && path[i + 1] == "floor03") render << 8;
        else if (path[i] == "floor03" && path[i + 1] == "floor04") render << 9;
        else if (path[i] == "floor04" && path[i + 1] == "floor05") render << 11;
        else if (path[i] == "floor05" && path[i + 1] == "floor06") render << 12;
        else if (path[i] == "floor06" && path[i + 1] == "floor07") render << 13;
        else if (path[i] == "floor02" && path[i + 1] == "floor01") render << 14;
        else if (path[i] == "floor03" && path[i + 1] == "floor02") render << 15;
        else if (path[i] == "floor04" && path[i + 1] == "floor03") render << 16;
        else if (path[i] == "floor05" && path[i + 1] == "floor04") render << 18;
        else if (path[i] == "floor06" && path[i + 1] == "floor05") render << 19;
        else if (path[i] == "floor07" && path[i + 1] == "floor06") render << 20;
        else if (path[i] == "floor01" && path[i + 1] == "room201") render << 21;
        else if (path[i] == "floor02" && path[i + 1] == "room202") render << 22;
        else if (path[i] == "floor04" && path[i + 1] == "room203") render << 23;
        else if (path[i] == "floor05" && path[i + 1] == "room204") render << 24;
        else if (path[i] == "floor06" && path[i + 1] == "room205") render << 25;
        else if (path[i] == "floor07" && path[i + 1] == "room206") render << 26;
        else if (path[i] == "floor07" && path[i + 1] == "room207") render << 27;
        else if (path[i] == "floor03" && path[i + 1] == "terrace01") render << 28;
        else if (path[i] == "terrace01" && path[i + 1] == "floor03") render << 29;
        else if (path[i] == "terrace01" && path[i + 1] == "exit08") render << 30;
        else if (path[i] == "floor04" && path[i + 1] == "stair02") render << 31;
        else if (path[i] == "floor05" && path[i + 1] == "stair02") render << 33;
        else if (path[i] == "stair02" && path[i + 1] == "floor03") render << 35;
        else if (path[i] == "stair02" && path[i + 1] == "floor04") render << 37;
        else if (path[i] == "stair02" && path[i + 1] == "stair01") render << 39;
        else if (path[i] == "stair02" && path[i + 1] == "stair03") render << 41;
        else if (path[i] == "room201" && path[i + 1] == "exit01") render << 43;
        else if (path[i] == "room202" && path[i + 1] == "exit02") render << 45;
        else if (path[i] == "room203" && path[i + 1] == "exit03") render << 47;
        else if (path[i] == "room204" && path[i + 1] == "exit04") render << 49;
        else if (path[i] == "room205" && path[i + 1] == "exit05") render << 51;
        else if (path[i] == "room206" && path[i + 1] == "exit06") render << 53;
        else if (path[i] == "room207" && path[i + 1] == "exit07") render << 55;

        else if (path[i] == "room08" && path[i + 1] == "floor08") render << 57;
        else if (path[i] == "room09" && path[i + 1] == "floor09") render << 58;
        else if (path[i] == "room10" && path[i + 1] == "floor11") render << 59;
        else if (path[i] == "room11" && path[i + 1] == "floor12") render << 60;
        else if (path[i] == "room12" && path[i + 1] == "floor13") render << 61;
        else if (path[i] == "room13" && path[i + 1] == "floor14") render << 62;
        else if (path[i] == "room14" && path[i + 1] == "floor14") render << 63;
        else if (path[i] == "floor08" && path[i + 1] == "floor09") render << 64;
        else if (path[i] == "floor09" && path[i + 1] == "floor10") render << 65;
        else if (path[i] == "floor10" && path[i + 1] == "floor11") render << 66;
        else if (path[i] == "floor11" && path[i + 1] == "floor12") render << 68;
        else if (path[i] == "floor12" && path[i + 1] == "floor13") render << 69;
        else if (path[i] == "floor13" && path[i + 1] == "floor14") render << 70;
        else if (path[i] == "floor09" && path[i + 1] == "floor08") render << 71;
        else if (path[i] == "floor10" && path[i + 1] == "floor09") render << 72;
        else if (path[i] == "floor11" && path[i + 1] == "floor10") render << 73;
        else if (path[i] == "floor12" && path[i + 1] == "floor11") render << 75;
        else if (path[i] == "floor13" && path[i + 1] == "floor12") render << 76;
        else if (path[i] == "floor14" && path[i + 1] == "floor13") render << 77;
        else if (path[i] == "floor08" && path[i + 1] == "room08") render << 78;
        else if (path[i] == "floor09" && path[i + 1] == "room09") render << 79;
        else if (path[i] == "floor11" && path[i + 1] == "room10") render << 80;
        else if (path[i] == "floor12" && path[i + 1] == "room11") render << 81;
        else if (path[i] == "floor13" && path[i + 1] == "room12") render << 82;
        else if (path[i] == "floor14" && path[i + 1] == "room13") render << 83;
        else if (path[i] == "floor14" && path[i + 1] == "room14") render << 84;
        else if (path[i] == "floor10" && path[i + 1] == "terrace02") render << 85;
        else if (path[i] == "terrace02" && path[i + 1] == "floor10") render << 86;
        else if (path[i] == "terrace02" && path[i + 1] == "exit16") render << 87;
        else if (path[i] == "floor11" && path[i + 1] == "stair03") render << 88;
        else if (path[i] == "floor12" && path[i + 1] == "stair03") render << 90;
        else if (path[i] == "stair03" && path[i + 1] == "floor10") render << 92;
        else if (path[i] == "stair03" && path[i + 1] == "floor11") render << 94;
        else if (path[i] == "stair03" && path[i + 1] == "stair02") render << 96;
        else if (path[i] == "stair03" && path[i + 1] == "stair04") render << 98;
        else if (path[i] == "room08" && path[i + 1] == "exit09") render << 100;
        else if (path[i] == "room09" && path[i + 1] == "exit10") render << 102;
        else if (path[i] == "room10" && path[i + 1] == "exit11") render << 104;
        else if (path[i] == "room11" && path[i + 1] == "exit12") render << 106;
        else if (path[i] == "room12" && path[i + 1] == "exit13") render << 108;
        else if (path[i] == "room13" && path[i + 1] == "exit14") render << 110;
        else if (path[i] == "room14" && path[i + 1] == "exit15") render << 112;

        else if (path[i] == "room15" && path[i + 1] == "floor15") render << 114;
        else if (path[i] == "room16" && path[i + 1] == "floor16") render << 115;
        else if (path[i] == "room17" && path[i + 1] == "floor17") render << 116;
        else if (path[i] == "room18" && path[i + 1] == "floor19") render << 117;
        else if (path[i] == "room19" && path[i + 1] == "floor20") render << 118;
        else if (path[i] == "room20" && path[i + 1] == "floor21") render << 119;
        else if (path[i] == "room21" && path[i + 1] == "floor21") render << 120;
        else if (path[i] == "floor15" && path[i + 1] == "floor16") render << 121;
        else if (path[i] == "floor16" && path[i + 1] == "floor17") render << 122;
        else if (path[i] == "floor17" && path[i + 1] == "floor18") render << 124; //
        else if (path[i] == "floor18" && path[i + 1] == "floor19") render << 125;
        else if (path[i] == "floor19" && path[i + 1] == "floor20") render << 126;
        else if (path[i] == "floor20" && path[i + 1] == "floor21") render << 127;
        else if (path[i] == "floor16" && path[i + 1] == "floor15") render << 128;
        else if (path[i] == "floor17" && path[i + 1] == "floor16") render << 129;
        else if (path[i] == "floor18" && path[i + 1] == "floor17") render << 130;
        else if (path[i] == "floor19" && path[i + 1] == "floor18") render << 132;//
        else if (path[i] == "floor20" && path[i + 1] == "floor19") render << 133;
        else if (path[i] == "floor21" && path[i + 1] == "floor20") render << 134;
        else if (path[i] == "floor15" && path[i + 1] == "room15") render << 135;
        else if (path[i] == "floor16" && path[i + 1] == "room16") render << 136;
        else if (path[i] == "floor18" && path[i + 1] == "room17") render << 137;
        else if (path[i] == "floor19" && path[i + 1] == "room18") render << 138;
        else if (path[i] == "floor20" && path[i + 1] == "room19") render << 139;
        else if (path[i] == "floor21" && path[i + 1] == "room20") render << 140;
        else if (path[i] == "floor21" && path[i + 1] == "room21") render << 141;
        else if (path[i] == "floor17" && path[i + 1] == "terrace03") render << 142;
        else if (path[i] == "terrace03" && path[i + 1] == "floor17") render << 143;
        else if (path[i] == "terrace03" && path[i + 1] == "exit24") render << 144;
        else if (path[i] == "floor18" && path[i + 1] == "stair04") render << 145;//
        else if (path[i] == "floor19" && path[i + 1] == "stair04") render << 147;
        else if (path[i] == "stair04" && path[i + 1] == "floor17") render << 149;
        else if (path[i] == "stair04" && path[i + 1] == "floor18") render << 151;
        else if (path[i] == "stair04" && path[i + 1] == "stair03") render << 153;
        else if (path[i] == "stair04" && path[i + 1] == "stair05") render << 155;
        else if (path[i] == "room15" && path[i + 1] == "exit17") render << 157;
        else if (path[i] == "room16" && path[i + 1] == "exit18") render << 159;
        else if (path[i] == "room17" && path[i + 1] == "exit19") render << 161;
        else if (path[i] == "room18" && path[i + 1] == "exit20") render << 163;
        else if (path[i] == "room19" && path[i + 1] == "exit21") render << 165;
        else if (path[i] == "room20" && path[i + 1] == "exit22") render << 167;
        else if (path[i] == "room21" && path[i + 1] == "exit23") render << 169;

        else if (path[i] == "room22" && path[i + 1] == "floor22") render << 171;
        else if (path[i] == "room23" && path[i + 1] == "floor23") render << 172;
        else if (path[i] == "room24" && path[i + 1] == "floor25") render << 173;
        else if (path[i] == "room25" && path[i + 1] == "floor26") render << 174;
        else if (path[i] == "room26" && path[i + 1] == "floor27") render << 175;
        else if (path[i] == "room27" && path[i + 1] == "floor28") render << 176;
        else if (path[i] == "room28" && path[i + 1] == "floor28") render << 177;
        else if (path[i] == "floor22" && path[i + 1] == "floor23") render << 178;
        else if (path[i] == "floor23" && path[i + 1] == "floor24") render << 179;
        else if (path[i] == "floor24" && path[i + 1] == "floor25") render << 180;
        else if (path[i] == "floor25" && path[i + 1] == "floor26") render << 182; //
        else if (path[i] == "floor26" && path[i + 1] == "floor27") render << 183;
        else if (path[i] == "floor27" && path[i + 1] == "floor28") render << 184;
        else if (path[i] == "floor23" && path[i + 1] == "floor22") render << 185;
        else if (path[i] == "floor24" && path[i + 1] == "floor23") render << 186;
        else if (path[i] == "floor25" && path[i + 1] == "floor24") render << 187;
        else if (path[i] == "floor26" && path[i + 1] == "floor25") render << 189; //
        else if (path[i] == "floor27" && path[i + 1] == "floor26") render << 190;
        else if (path[i] == "floor28" && path[i + 1] == "floor27") render << 191;
        else if (path[i] == "floor22" && path[i + 1] == "room22") render << 192;
        else if (path[i] == "floor23" && path[i + 1] == "room23") render << 193;
        else if (path[i] == "floor25" && path[i + 1] == "room24") render << 194;
        else if (path[i] == "floor26" && path[i + 1] == "room25") render << 195;
        else if (path[i] == "floor27" && path[i + 1] == "room26") render << 196;
        else if (path[i] == "floor28" && path[i + 1] == "room27") render << 197;
        else if (path[i] == "floor28" && path[i + 1] == "room28") render << 198;
        else if (path[i] == "floor24" && path[i + 1] == "terrace04") render << 199;
        else if (path[i] == "terrace04" && path[i + 1] == "floor24") render << 200;
        else if (path[i] == "terrace04" && path[i + 1] == "exit32") render << 201;
        else if (path[i] == "floor25" && path[i + 1] == "stair05") render << 202;
        else if (path[i] == "floor26" && path[i + 1] == "stair05") render << 204; //
        else if (path[i] == "stair05" && path[i + 1] == "floor24") render << 206;
        else if (path[i] == "stair05" && path[i + 1] == "floor25") render << 208;
        else if (path[i] == "stair05" && path[i + 1] == "stair04") render << 210;
        else if (path[i] == "stair05" && path[i + 1] == "stair06") render << 212;
        else if (path[i] == "room22" && path[i + 1] == "exit25") render << 214;
        else if (path[i] == "room23" && path[i + 1] == "exit26") render << 216;
        else if (path[i] == "room24" && path[i + 1] == "exit27") render << 218;
        else if (path[i] == "room25" && path[i + 1] == "exit28") render << 220;
        else if (path[i] == "room26" && path[i + 1] == "exit29") render << 222;
        else if (path[i] == "room27" && path[i + 1] == "exit30") render << 224;
        else if (path[i] == "room28" && path[i + 1] == "exit31") render << 226;

        else if(path[i]=="stair01" && path[i+1]=="exit00") render << 228;

    }
    //qDebug() << "path to render" << render;
}
