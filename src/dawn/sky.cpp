#include "sky.h"

sky::sky(){
	update_projection = false;
	update_cam = false;
	angle = 0;
}

sky::~sky(){
	delete space;
	delete moon;
}


void sky::draw() {
	space->use();
	space->setMat4("projection", projection);
	space->setMat4("view", view);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, z));
	//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	space->setMat4("model", model);
	moon->Draw(space);
}

void sky::update(float deltaTime) {

	x = center_x + radius * cos(angle);
	y = center_y + radius * sin(angle);

	angle += (angle_incr * deltaTime);
	if (angle >= 360) {
		angle = 0;
	}
	
}

void sky::init() {
	center_x = x_width;
	center_z = z_width;
	x = center_x;
	z = center_z;
	y = center_y;
	radius = x_width + (x_width * 0.5);
	space = new Shader("shaders/planet.vs", "shaders/planet.fs"); 
	moon = new Model("resources/objects/planet/planet.obj");

}