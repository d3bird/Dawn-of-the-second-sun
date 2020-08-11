#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "model.h"
#include "shader.h"

struct map_tile{
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int buffer_loc;
	int type;
};

class terrian{
public:

	terrian();

	void draw();
	void update(float delta_time);

	void space_init();
	void cubes_init();

	//settersand getters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }

	int get_draw_mode() { return draw_mode; }

	map_tile** get_map() { return map; }
	unsigned int get_x_width() { return x_width; }
	unsigned int get_z_width() { return z_width; }

private:

	void draw_space();
	void draw_cubes();

	void update_cubes(float delta_time);

	void print_map();

	void inline updateBuffer_ter() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, cube_amount * sizeof(glm::mat4), &cube_matrices[0], GL_STATIC_DRAW);
	}

	void inline updateBuffer_space() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	}

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
	map_tile** map;

	//space terrian vars
	Model *rock;
	Model* planet;
	Shader* planetShader;
	Shader* asteroidShader;
	unsigned int amount = 100000;
	glm::mat4* modelMatrices;


};

