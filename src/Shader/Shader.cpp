#include "src\Shader\Shader.h"
#include <fstream>
#include <iostream>
#include "glm\glm.hpp"
#include <vector>
using std::vector;
using glm::mat4;

void Shader::installShader(){

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* adapter[1];
	std::string temp = readShaderCode("src/Shader/VertexShader.glsl").c_str();
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("src/Shader/FragmentShader.glsl").c_str();
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) | !checkShaderStatus(fragmentShaderID)){
		return;
	}

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);
	glValidateProgram(programID);

	if (!checkProgramStatus(programID)){
		return;
	}

	getAllUniformLocations();

}

void Shader::startProgram(){

	glUseProgram(programID);
}

void Shader::stopProgram(){

	glUseProgram(0);
}


void Shader::load(){

	vector<float> verts{
		-1.0, -1.0, 0.0,
		+1.0, -1.0, 0.0,
		+1.0, +1.0, 0.0,
		-1.0, +1.0, 0.0
	};

	vector<GLuint> indicies{
		0, 1, 2,
		2, 3, 0
	};

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);


	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(GLuint), &indicies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

}

void Shader::render(){

	startProgram();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	loadToUniform(location_x_min, settings->x_min);
	loadToUniform(location_x_max, settings->x_max);
	loadToUniform(location_y_min, settings->y_min);
	loadToUniform(location_y_max, settings->y_max);
	loadToUniform(location_bound, settings->bound);
	loadToUniform(location_maxItteration, settings->maxItteration);
	loadToUniform(location_C, settings->C);

	startProgram();

	glEnableVertexAttribArray(0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	stopProgram();

	stopProgram();



}

void Shader::cleanUp(){

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glDeleteProgram(programID);
}


std::string Shader::readShaderCode(const GLchar* fileName){

	std::ifstream input(fileName);
	if (!input.good()){
		std::cout << "File failed to load" << std::endl;
		exit(1);
	}
	return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}


GLint Shader::getUniformLocation(std::string &uniformName){

	const GLchar* adapter[1];
	std::string temp = uniformName.c_str();
	adapter[0] = temp.c_str();
	return glGetUniformLocation(programID, adapter[0]);
}

void Shader::getAllUniformLocations(){

	std::string string = " ";

	string = "x_max";
	location_x_max = getUniformLocation(string);

	string = "x_min";
	location_x_min = getUniformLocation(string);

	string = "y_max";
	location_y_max = getUniformLocation(string);

	string = "y_min";
	location_y_min = getUniformLocation(string);

	string = "maxItteration";
	location_maxItteration = getUniformLocation(string);

	string = "bound";
	location_bound = getUniformLocation(string);

	string = "C";
	location_C = getUniformLocation(string);
}


void Shader::loadToUniform(GLuint &location, bool &data){

	if (data == true){
		glUniform1f(location, data);
	}
	else {
		glUniform1f(location, data);
	}

}
void Shader::loadToUniform(GLuint &location, GLfloat &data){

	glUniform1f(location, data);
}

void Shader::loadToUniform(GLuint &location, glm::vec3 &data){

	glUniform3fv(location, 1, &data[0]);
}

void Shader::loadToUniform(GLuint &location, glm::mat4 &data){

	glUniformMatrix4fv(location, 1, GL_FALSE, &data[0][0]);
}

void Shader::loadToUniform(GLuint &location, glm::vec2 &data){
	
	glUniform2fv(location, 1, &data[0]);
}

bool Shader::checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetter,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFun, GLenum statusType){

	GLint status;
	objectPropertyGetter(objectID, statusType, &status);
	if (status != GL_TRUE){
		GLint infoLogLength;
		objectPropertyGetter(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];
		GLsizei bufferSize;
		getInfoLogFun(objectID, infoLogLength, &bufferSize, buffer);

		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool Shader::checkShaderStatus(GLuint ShaderID){

	return checkStatus(ShaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool Shader::checkProgramStatus(GLuint programID){

	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);

}