#pragma once
#include "glm\glm.hpp"

class Settings{

public:

	float x_min = -2.0f;
	float x_max = 2.0f;
	float y_min = -2.0f;
	float y_max = 2.0f;

	float bound = 4.0f;
	float maxItteration = 300.0f;

	glm::vec2 C = glm::vec2(-0.4f, 0.6f);

	float move_xIncreament = 0.01f;
	float move_yIncreament = 0.01f;
	float zoom_increament = 0.01f;

	int max_FPS = 60;


};