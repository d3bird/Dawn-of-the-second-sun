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
	//create the differnt objects
	create_log_objects();
	create_alter_objects();
	std::cout << "finished creating the object manager" << std::endl;
}

void object_manger::increase_buffer_size() {


}

void object_manger::update_item_matrix(update_pak* data) {

	if (data != NULL && data->item_id < items.size()) {
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(data->x_scale, data->y_scale, data->z_scale));
		model = glm::translate(model, glm::vec3(data->x, data->y, data->z));
		items[data->item_id]->modelMatrices[data->buffer_loc] = model;
		delete data;//clean mem
	}
	else {
		std::cout << "could not update item, item_id out of range" << std::endl;
	}

}

item_info* object_manger::get_item_info() {
	item_info* output;// = new item_info;
	output = items[0]->item_data[0];
	return output;
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

	float x = 0;
	float y = 7;
	float z = 0;
	float x_scale = 1;
	float y_scale = 1;
	float z_scale = 1;
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x, y, z));
	modelMatrices[0] = trans;

	item_info* temp_data = new item_info;
	temp_data->x = x;
	temp_data->y = y;
	temp_data->z = z;
	temp_data->x_scale = x_scale;
	temp_data->y_scale = y_scale;
	temp_data->z_scale = z_scale;
	temp_data->item_id = 0;
	temp_data->buffer_loc = 0;

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

	temp->item_data.push_back(temp_data);//add the data for the object

	items.push_back(temp);

}


void object_manger::create_alter_objects() {

	unsigned int buffer;
	unsigned int buffer_size;
	unsigned int amount;
	glm::mat4* modelMatrices;
	Shader* custom_shader;
	Model* model;

	//creating the alter object
	buffer = 0;
	buffer_size = 1;
	amount = 1;
	modelMatrices = new glm::mat4[buffer_size];
	custom_shader = NULL;
	model = new Model("resources/objects/alter/alter.obj");

	//get the location in respect to the bottom right correner of hte map
	int int_x_loc = Terrian->get_x_width() - 2;
	int int_y_loc;
	int int_z_loc = Terrian->get_z_width() - 2;;
	glm::vec3 location = Terrian->get_coridents(int_x_loc, int_z_loc);
	//block the locations
	Terrian->block_spot(int_x_loc, int_z_loc);
	Terrian->block_spot(int_x_loc - 1, int_z_loc);
	Terrian->block_spot(int_x_loc + 1, int_z_loc);
	Terrian->block_spot(int_x_loc, int_z_loc - 1);
	Terrian->block_spot(int_x_loc, int_z_loc + 1);
	Terrian->print_map_blocked();//for debuggin the collision

	float x = location.x;
	float y = location.y;
	float z = location.z;

	float x_scale = 1;
	float y_scale = 1;
	float z_scale = 1;
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, location);
	modelMatrices[0] = trans;

	//creates the item data 
	item_info* temp_data = new item_info;
	temp_data->x = x;
	temp_data->y = y;
	temp_data->z = z;
	temp_data->x_scale = x_scale;
	temp_data->y_scale = y_scale;
	temp_data->z_scale = z_scale;
	temp_data->item_id = 0;
	temp_data->buffer_loc = 0;

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

	temp->item_data.push_back(temp_data);//add the data for the object

	items.push_back(temp);

}

