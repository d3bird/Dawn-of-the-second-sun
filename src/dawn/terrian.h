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

using namespace std;
typedef pair<int, int> Pair;
typedef pair<double, pair<int, int>> pPair;
struct cell {
	int parent_i, parent_j;
	// f = g + h 
	double f, g, h;
};

struct map_tile {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int buffer_loc;
	int type;
	bool blocked;
	int g_cost;//move cost to move to this square
};

class terrian{
public:

	terrian();

	void draw();
	void update(float delta_time);

	void space_init();
	void cubes_init();

	std::vector<glm::vec3*> find_path(int x1, int z1, int x2,int z2, float height);

	//settersand getters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }

	int get_draw_mode() { return draw_mode; }

	map_tile** get_map() { return terrian_map; }
	unsigned int get_x_width() { return x_width; }
	unsigned int get_y_width() { return y_width; }
	unsigned int get_z_width() { return z_width; }

private:

	void draw_space();
	void draw_cubes();

	void update_cubes(float delta_time);

	void print_map();
	void print_map_blocked();

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
	unsigned int cube_buffer_size;//amount of spaces in the buffer
	unsigned int x_width;//rows
	unsigned int z_width;//collums
	unsigned int y_width;
	float cube_offset;
	map_tile** terrian_map;

	//space terrian vars
	Model *rock;
	Model* planet;
	Shader* planetShader;
	Shader* asteroidShader;
	unsigned int amount = 100000;
	glm::mat4* modelMatrices;

	//path finding functions/vars


	bool isValid(int row, int col);
	bool isUnBlocked(int row, int col);
	bool isDestination(int row, int col, Pair dest);
	double calculateHValue(int row, int col, Pair dest);
	void tracePath(cell** cellDetails, Pair dest);
	cell** aStarSearch(Pair src, Pair dest);

};

