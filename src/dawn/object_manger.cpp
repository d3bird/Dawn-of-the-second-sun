#include "object_manger.h"

object_manger::object_manger() {
	update_projection = false;
	update_cam = false;
	using_custom_shaders = false;
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
	common = NULL;
}

object_manger::~object_manger() {

}

void object_manger::draw() {

	for (int q = 0; q < items.size(); q++) {
		glm::mat4* matrix_temp = items[q]->modelMatrices;
		glBindBuffer(GL_ARRAY_BUFFER, items[q]->buffer);
		glBufferData(GL_ARRAY_BUFFER, items[q]->amount * sizeof(glm::mat4), &matrix_temp[0], GL_STATIC_DRAW);

		common->use();
		//common->setMat4("projection", projection);
		//common->setMat4("view", view);
		common->setInt("texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, items[q]->model->textures_loaded[0].id);
		for (unsigned int i = 0; i < items[q]->model->meshes.size(); i++)
		{
			glBindVertexArray(items[q]->model->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, items[q]->model->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, items[q]->amount);
			glBindVertexArray(0);
		}
	}
}

void object_manger::update(float deltaTime) {

}

void object_manger::init() {
	std::cout << "creating the object manager" << std::endl;

	create_log_objects();

	std::cout << "finished creating the object manager" << std::endl;
}

void object_manger::increase_buffer_size() {


}


void object_manger::create_log_objects() {

	unsigned int buffer;
	unsigned int buffer_size;
	unsigned int amount;
	glm::mat4* modelMatrices;
	Shader* custom_shader;
	Model* model;

	//creating the log item
	buffer = 0;
	buffer_size = 1;
	amount = 1;
	modelMatrices = new glm::mat4[buffer_size];
	custom_shader = NULL;
	model = new Model("resources/objects/log/log.obj");

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0, 7, 0));
	modelMatrices[0] = trans;


	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < model->meshes.size(); i++)
	{
		unsigned int VAO = model->meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	item* temp = new item;
	temp->buffer_size = buffer_size;
	temp->buffer = buffer;
	temp->amount = amount;
	temp->model = model;
	temp->modelMatrices = modelMatrices;
	temp->custom_shader = custom_shader;
	items.push_back(temp);

}