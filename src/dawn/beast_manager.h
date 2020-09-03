#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shader.h"
#include "model.h"
#include "terrian.h"
#include "object_manger.h"
#include "creature.h"


class beast_manager{
public:
	beast_manager();
	~beast_manager();
	
	void draw();
	void update(float deltaTime);

	void spawn_creature(zone* spawn_zone);

	void init();

	void set_map_size(unsigned int x, unsigned int z) { x_width = x; z_width = z; }
	
	void set_projection(glm::mat4 i) { projection = i; }
	void set_cam(glm::mat4 i) { view = i;  }
	void set_terrian(terrian* i) { map = i; }
	void set_object_manger(object_manger* i) { objects = i; }
	void set_def_shader(Shader* i) { beast_shader = i; }

	void assign_task(int creature_id, task* Job);

private:

	float diff_btwn_pnt(float start, float end);

	void inline updateBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &beast_matrices[0], GL_STATIC_DRAW);
	}
	bool determin_direction(float start, float end);

	glm::mat4 view;
	glm::mat4 projection;
	unsigned int buffer;

	unsigned int x_width;
	unsigned int z_width;

	std::vector<creature*> wandering;
	std::vector<creature*> moving;

	terrian* map;
	object_manger* objects;
	Model* beast;
	glm::mat4* beast_matrices;
	Shader* beast_shader;
	unsigned int amount;
	unsigned int max_amount;
};

