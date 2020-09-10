#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <queue>
#include <string>

#include "model.h"
#include "shader.h"


/*
* this class maganges the information about the workshops, items, and misc furniture 
*/

//the information on each unique object
struct item_info {
	unsigned int item_id;
	unsigned int buffer_loc;
	unsigned int amount;
	float x, y, z;
	float x_scale;
	float y_scale;
	float z_scale;
	std::string* item_name;
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
	std::string* item_name;
};

struct update_pak {
	unsigned int item_id;
	unsigned int buffer_loc;
	float x,y,z;
	float x_scale;
	float y_scale;
	float z_scale;
};

//transfer object to tell the terrian what to block in bulk after the objects are created
struct block_loc {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	block_loc(unsigned int x1, unsigned int z1) {
		x = x1;
		z = z1;
	}
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

	//alter functions
	item_info* get_alter_info() { return alter; }
	void preform_sacrifice(item_info* sac);

	//getters and setters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }
	void set_standered_shader(Shader* i) { common = i; }
	//void set_terrian_obj(terrian* i) { Terrian = i; }

	bool need_cam_updates() { return using_custom_shaders; }
	std::vector<block_loc*>* get_blocked_spots() { return blocked_spots; }

private:
	
	void increase_buffer_size();

	void create_log_objects();
	void create_alter_objects();
	//common vars
	bool update_projection;
	bool update_cam;

	bool using_custom_shaders;


	glm::mat4 view;
	glm::mat4 projection;
	Shader* common;

	std::vector< item*> items;
	std::vector<block_loc*>* blocked_spots;
	//the main alter info
	item_info* alter;

};

