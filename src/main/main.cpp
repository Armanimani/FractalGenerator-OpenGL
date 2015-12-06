#include <GL\glew.h>
#include <QtWidgets\qapplication.h>
#include "src\window\window.h"
int main(int argc, char* argv[]){

	QApplication application(argc, argv);

	Window window;

	window.show();


	return application.exec();
}