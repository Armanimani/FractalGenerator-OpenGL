#pragma once
#include <GL\glew.h>
#include <QtOpenGL\qglwidget>
#include "src\Settings\Settings.h"
#include "src\Shader\Shader.h"
#include "QtCore\qtimer.h"

class Window : public QGLWidget{

	Q_OBJECT

public:

	Window();
	void cleanUp();

protected:

	Settings settings;
	QTimer timer;
	Shader *shader;

	void initializeGL();
	void paintGL();
	void checkKeyState();

private slots:
	void update();
};