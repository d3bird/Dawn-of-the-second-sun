#pragma once

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "terrian.h"
#include "beast_manager.h"

class world{
public:
	world();

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

	terrian* Terrian;
	beast_manager* BM;
};

