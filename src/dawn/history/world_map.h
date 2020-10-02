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
#include <stdlib.h>
#include <random>

#include "../model.h"
#include "../shader.h"


enum biome_type{WATER, PLAIN, FOREST};
struct world_map_tile {

	unsigned int buffer_loc;
	biome_type biome;
	int debug_id;
	glm::vec2 trans;
};

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
	
	//sorts the map_tiles in to the buffer to get the different biome colors
	void create_buffers_biomes();
	void create_location_for_tiles();
	void create_map();

	void print_map(int data);

	glm::mat4 view;
	glm::mat4 projection;
	bool update_projection;
	bool update_cam;

	Shader* shader;

	unsigned int quadVAO, quadVBO;
	unsigned int instanceVBO;

	int width;
	int height;

	world_map_tile** tile_map;

	glm::vec2* translations;
	int translations_amount;
	int index;
	float offset;
	float map_tile_size;//total size
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

	int seed;
};

