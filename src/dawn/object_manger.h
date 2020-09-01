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
#include "terrian.h"

/*
* this class maganges the information about the workshops, items, and misc furniture 
*/


//the information on each unique object
struct item_info {
	unsigned int item_id;
	unsigned int buffer_loc;
	float x, y, z;
	float x_scale;
	float y_scale;
	float z_scale;
};

//the data needed to render the objects in the world
struct item {
	Model* model;
	unsigned int buffer;
	unsigned int buffer_size;
	unsigned int amount;
	glm::mat4* modelMatrices;
	Shader* custom_shader;
	vector<item_info*> item_data;
};

struct update_pak {
	unsigned int item_id;
	unsigned int buffer_loc;
	float x,y,z;
	float x_scale;
	float y_scale;
	float z_scale;
};

class object_manger{
public:

	object_manger();
	~object_manger();

	void draw();
	void update(float deltaTime);
	void init();

	void update_item_matrix(update_pak* data);
	item_info* get_item_info();


	//getters and setters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }
	void set_standered_shader(Shader* i) { common = i; }
	void set_terrian_obj(terrian* i) { Terrian = i; }

	bool need_cam_updates() { return using_custom_shaders; }

private:
	
	void increase_buffer_size();

	void create_log_objects();
	void create_alter_objects();
	//common vars
	bool update_projection;
	bool update_cam;

	bool using_custom_shaders;

	terrian* Terrian;

	glm::mat4 view;
	glm::mat4 projection;
	Shader* common;

	std::vector< item*> items;

};

