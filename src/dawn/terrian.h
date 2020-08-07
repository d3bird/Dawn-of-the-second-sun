#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "model.h"
#include "shader.h"

class terrian{
public:
	terrian();

	void draw();

	void init();

private:

	//space terrian models
	Model *rock;
	Model* planet;

};

