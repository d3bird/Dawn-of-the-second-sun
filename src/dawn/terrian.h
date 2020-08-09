#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "model.h"
#include "shader.h"

class terrian{
public:

	terrian();

	void draw();

	void space_init();
	void cubes_init();

	//settersand getters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }

	int get_draw_mode() { return draw_mode; }

private:

	void draw_space();
	void draw_cubes();

	bool update_projection;
	bool update_cam;
	int draw_mode;

	//common vars
	unsigned int buffer;
	glm::mat4 view;
	glm::mat4 projection;

	//cube terrian vars
	Model* cube;
	glm::mat4* cube_matrices;
	Shader* cube_shader;
	unsigned int cube_amount;
	unsigned int x_width;
	unsigned int z_width;
	float cube_offset;

	//space terrian vars
	Model *rock;
	Model* planet;
	Shader* planetShader;
	Shader* asteroidShader;
	unsigned int amount = 100000;
	glm::mat4* modelMatrices;


};

