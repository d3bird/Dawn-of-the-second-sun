#include "event_manager.h"

event_manager::event_manager() {
	sig = false;
}

event_manager::~event_manager() {

}

void event_manager::draw() {
	if (sig) {
		singularity->use();
		singularity->setMat4("projection", projection);
		singularity->setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x, y, z));
		//model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		singularity->setMat4("model", model);
		moon->Draw(singularity);

	}
}

void event_manager::update() {
	if (sig) {


	}

}

void event_manager::cause_event(int ev, int x, int y, int z) {
	std::cout << "casuing event number "<< ev << std::endl;
	switch (ev) {
	case 0://singularity
		sig = true;
		break;
	case 1://earthquake

		break;
	default:
		std::cout << "there is no event for this number" << std::endl;
		break;
	}

}

void event_manager::init() {
	singularity = new Shader("singularity.vs", "singularity.fs");
	moon = new Model("resources/objects/planet/planet.obj");

}