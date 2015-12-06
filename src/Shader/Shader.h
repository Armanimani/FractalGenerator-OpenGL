#pragma once
#include "GL\glew.h"
#include "src\Settings\Settings.h"
#include "glm\glm.hpp"
#include <string>

class Shader{

public:

	Shader(Settings &settings) : settings(&settings){}

	void startProgram();
	void stopProgram();
	void installShader();

	void render();
	void load();
	void cleanUp();

protected:

	GLuint location_x_max;
	GLuint location_x_min;
	GLuint location_y_max;
	GLuint location_y_min;
	GLuint location_maxItteration;
	GLuint location_bound;
	GLuint location_C;


	GLuint vertexBufferID;
	GLuint indexBufferID;

	GLuint programID;
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
	std::string readShaderCode(const GLchar* fileName);
	GLint getUniformLocation(std::string &uniformName);
	void getAllUniformLocations();

	void loadToUniform(GLuint &location, bool &data);
	void loadToUniform(GLuint &location, GLfloat &data);
	void loadToUniform(GLuint &location, glm::vec3 &data);
	void loadToUniform(GLuint &location, glm::mat4 &data);
	void loadToUniform(GLuint &location, glm::vec2 &data);

	bool checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetter,
		PFNGLGETSHADERINFOLOGPROC getInfoLogFun, GLenum statusType);
	bool checkShaderStatus(GLuint ShaderID);
	bool checkProgramStatus(GLuint programID);


private:

	Settings *settings;
};