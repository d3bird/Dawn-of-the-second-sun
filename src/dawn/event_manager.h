#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

#include "shader.h"
#include "time.h"
#include "model.h"

class event_manager {
public:

	event_manager();
	~event_manager();

	void draw();
	void update();

	void cause_event(int ev, int x, int y, int z);

	void init();

	bool* get_draw_bool() { return draw_event; }
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; }
	void set_time(timing* i) { Time = i; }

private:

	glm::mat4 view;
	glm::mat4 projection;
	bool update_projection;
	bool update_cam;

	timing* Time;
	float* deltatime;
	bool* draw_event;


	//event data
	//singularity
	Shader* singularity;
	Model* moon;
	int sig_id;
	bool sig;
	float x;
	float y;
	float z;
	int x_map_loc;
	int y_map_loc;
	int z_map_loc;
};