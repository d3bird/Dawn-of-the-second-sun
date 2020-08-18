#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <set>

#include "model.h"
#include "shader.h"

class sky{
public:
	sky();
	~sky();

	void draw();
	void update(float deltaTime);
	void init();

	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }

private:

	glm::mat4 view;
	glm::mat4 projection;
	bool update_projection;
	bool update_cam;

};

