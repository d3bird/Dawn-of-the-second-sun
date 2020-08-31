#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "model.h"
#include "shader.h"

/*
* this class maganges the information about the workshops, items, and misc furniture 
*/


struct item{
	Model* model;
	unsigned int buffer;
	unsigned int buffer_size;
	unsigned int amount;
	glm::mat4* modelMatrices;
	Shader* custom_shader;
};

class object_manger{
public:

	object_manger();
	~object_manger();

	void draw();
	void update(float deltaTime);
	void init();

	//getters and setters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }
	void set_standered_shader(Shader* i) { common = i; }

	bool need_cam_updates() { return using_custom_shaders; }

private:
	
	void increase_buffer_size();

	void create_log_objects();

	//common vars
	bool update_projection;
	bool update_cam;

	bool using_custom_shaders;

	glm::mat4 view;
	glm::mat4 projection;
	Shader* common;

	std::vector< item*> items;

};

