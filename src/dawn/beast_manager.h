#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shader.h"
#include "model.h"
#include "terrian.h"
#include "creature.h"


class beast_manager{
public:
	beast_manager();
	~beast_manager();
	
	void draw();
	void update(float deltaTime);

	void init();

	void set_projection(glm::mat4 i) { projection = i; }
	void set_cam(glm::mat4 i) { view = i;  }

private:

	void inline updateBuffer_space() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &beast_matrices[0], GL_STATIC_DRAW);
	}

	glm::mat4 view;
	glm::mat4 projection;
	unsigned int buffer;

	map_tile** map;
	Model* beast;
	glm::mat4* beast_matrices;
	Shader* beast_shader;
	unsigned int amount;

};

