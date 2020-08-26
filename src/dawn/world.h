#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "terrian.h"
#include "beast_manager.h"
#include "sky.h"

class world{
public:
	world();

	void draw();
	void draw_selection();

	void update(float deltaTime);
	void process_mouse_action(float m_x, float m_y);
	void init();

	//settersand getters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }

private:

	void init_lighting_test();
	void draw_lighitng_test();
	void update_lighting_test(float deltaTime);

	glm::mat4 view;
	glm::mat4 projection;
	bool update_projection;
	bool update_cam;

	terrian* Terrian;
	beast_manager* BM;
	sky* Sky;

	//lighting test
	Model* moon;
	Model* cube;
	Shader* lighting;//for one object
	Shader* def;//no lighting
	Shader* lighting_in;//lighting for instanced objects

	//light possitions 
	float x;
	float y;
	float z;
	float angle;
	float angle_incr = 0.3;

	//selection shader
	Shader* selection;
};

