#include "src\window\window.h"
#include <iostream>

Window::Window(){

	shader = new Shader(settings); 
}

void Window::initializeGL(){

	glewInit();

	shader->installShader();
	shader->load();

	connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
	timer.start(1000 / settings.max_FPS);

}

void Window::paintGL(){

	glViewport(0, 0, width(), height());
	shader->render();

}

void Window::cleanUp(){

	shader->cleanUp();

}

void Window::update(){

	checkKeyState();

}

void Window::checkKeyState(){


	if (GetAsyncKeyState('W')){
		float scale = settings.y_max - settings.y_min;
		settings.y_max += settings.move_yIncreament * scale;
		settings.y_min += settings.move_yIncreament * scale;
	}

	if (GetAsyncKeyState('S')){
		float scale = settings.y_max - settings.y_min;
		settings.y_max -= settings.move_yIncreament * scale;
		settings.y_min -= settings.move_yIncreament * scale;
	}

	if (GetAsyncKeyState('D')){
		float scale = settings.x_max - settings.x_min;
		settings.x_max += settings.move_xIncreament * scale;
		settings.x_min += settings.move_xIncreament * scale;
	}

	if (GetAsyncKeyState('A')){
		float scale = settings.x_max - settings.x_min;
		settings.x_max -= settings.move_xIncreament * scale;
		settings.x_min -= settings.move_xIncreament * scale;
	}
	if (GetAsyncKeyState(VK_UP)){
		float scale = settings.zoom_increament * (settings.x_max - settings.x_min);
		settings.x_min += scale;
		settings.x_max -= scale;
		settings.y_min += scale;
		settings.y_max -= scale;
	}

	if (GetAsyncKeyState(VK_DOWN)){
		float scale = settings.zoom_increament * (settings.x_max - settings.x_min);
		settings.x_min -= scale;
		settings.x_max += scale;
		settings.y_min -= scale;
		settings.y_max += scale;
	}
	repaint();

}