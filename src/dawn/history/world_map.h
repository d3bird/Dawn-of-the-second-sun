#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>  

#include "../model.h"
#include "../shader.h"


class world_map {
public:

	world_map();
	~world_map();
	void draw();
	void update(float deltaTime);

	void init();

	//settersand getters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }

private:
	glm::mat4 view;
	glm::mat4 projection;
	bool update_projection;
	bool update_cam;

	Shader* shader;

	unsigned int quadVAO, quadVBO;
	unsigned int instanceVBO;

	glm::vec2 translations[100];
	int index;
	float offset;

	//biome data
	struct uniform_data {
		std::string loc;
		unsigned int buffer_loc;
	};

	int total_biome_types;
	glm::vec3* area_colors;
	int* color_bounds;
	uniform_data* biomes;

	void set_uniforms();
};

