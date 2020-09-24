#include "object_manger.h"

object_manger::object_manger() {
	update_projection = false;
	update_cam = false;
	using_custom_shaders = false;
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
	common = NULL;
	sac_time = 0;
	max_time = 3600000;
	float_sped = 8;
	in_possition = false;
	same_x = false;
	same_z = false;
	alter_draw = false;
	resultion = glm::vec2(800, 600);
	object_id = 0;
	init_sac = false;
	ready_to_sac = true;
	alter_about = false;
}

object_manger::~object_manger() {
	delete blocked_spots;
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
	if (alter_draw) {
		glEnable(GL_BLEND);
		//std::cout << "sac_time = " << sac_time << " u_resolutions = " << resultion.x << " " << resultion.y << std::endl;
		alter_affect->use();
		alter_affect->setMat4("projection", projection);
		alter_affect->setMat4("view", view);
		alter_affect->setFloat("u_time", sac_time);
		alter_affect->setVec2("u_resolutions", resultion);
		alter_affect->setMat4("model", sac_obj_mat);
		items[items_to_sac.front()->item_id]->model->Draw(alter_affect);
		glDisable(GL_BLEND);
	
		if (sac_time >= 10) {//rest the alter
			alter_draw = false;
			in_possition = false;
			same_x = false;
			same_z = false;
			items_to_sac.pop();
			sac_time = 0;
			init_sac = false;
			ready_to_sac = true;
			alter_about = false;
		}
	}
}

void object_manger::update(float deltaTime) {
	update_alter(deltaTime);
}

void object_manger::init() {
	std::cout << "creating the object manager" << std::endl;
	//create the differnt objects
	blocked_spots = new vector<block_loc*>();
	create_log_objects();
	create_alter_objects();
	std::cout << "finished creating the object manager" << std::endl;
}

void object_manger::increase_buffer_size() {


}

void object_manger::update_item_matrix(update_pak* data) {

	if (data != NULL && data->item_id < items.size()) {
		//std::cout << "updateing " << data->item_id << ", bufferloc = " << data->buffer_loc << std::endl;
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

//item_info* object_manger::get_item_info() {
//	item_info* output;// = new item_info;
//	output = items[0]->item_data[0];
//	return output;
//}

void object_manger::create_log_objects() {

	unsigned int buffer;
	unsigned int buffer_size;
	unsigned int amount;
	glm::mat4* modelMatrices;
	Shader* custom_shader;
	Model* model;
	std::string* item_name_t = new std::string("log object");
	//creating the log item
	buffer = 0;
	buffer_size = 10;
	amount = 2;
	modelMatrices = new glm::mat4[buffer_size];
	custom_shader = NULL;
	model = new Model("resources/objects/log/log.obj");

	int int_x_loc = 0;
	int int_y_loc = 7;
	int int_z_loc = 0;

	float x = int_x_loc * 2;
	float y = int_y_loc;
	float z = int_z_loc * 2;

	float x_scale = 1;
	float y_scale = 1;
	float z_scale = 1;
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x, y, z));
	modelMatrices[0] = trans;

	item_info* temp_data = new item_info;
	temp_data->type = LOG_T;
	temp_data->x = x;
	temp_data->y = y;
	temp_data->z = z;
	temp_data->x_m = int_x_loc;
	temp_data->y_m = int_y_loc;
	temp_data->z_m = int_z_loc;
	temp_data->x_scale = x_scale;
	temp_data->y_scale = y_scale;
	temp_data->z_scale = z_scale;
	temp_data->item_id = 0;
	temp_data->buffer_loc = 0;
	temp_data->item_name = item_name_t;
	temp_data->debug_id = object_id;
	temp_data->zone_location = NULL;
	object_id++;
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
	temp->item_name = item_name_t;
	temp->item_data.push_back(temp_data);//add the data for the object


	 int_x_loc = 2;
	 int_y_loc = 7;
	 int_z_loc = 0;

	 x = int_x_loc * 2;
	 y = int_y_loc;
	 z = int_z_loc * 2;

	temp_data = new item_info;
	temp_data->type = LOG_T;
	temp_data->x = x;
	temp_data->y = y;
	temp_data->z = z;
	temp_data->x_m = int_x_loc;
	temp_data->y_m = int_y_loc;
	temp_data->z_m = int_z_loc;
	temp_data->x_scale = x_scale;
	temp_data->y_scale = y_scale;
	temp_data->z_scale = z_scale;
	temp_data->item_id = 0;
	temp_data->buffer_loc = 1;
	temp_data->item_name = item_name_t;
	temp_data->debug_id = object_id;
	temp_data->zone_location = NULL;
	object_id++;
	 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(x, y, z));
	modelMatrices[1] = trans;


	temp->item_data.push_back(temp_data);//add the data for the object

	items.push_back(temp);

}

//alter fucntion
void object_manger::create_alter_objects() {

	unsigned int buffer;
	unsigned int buffer_size;
	unsigned int amount;
	glm::mat4* modelMatrices;
	Shader* custom_shader;
	Model* model;
	alter_affect = new Shader("shaders/alter.vs", "shaders/alter.fs");

	std::string* item_name_t = new std::string("alter object");

	//creating the alter object
	buffer = 0;
	buffer_size = 1;
	amount = 1;
	modelMatrices = new glm::mat4[buffer_size];
	custom_shader = NULL;
	model = new Model("resources/objects/alter/alter.obj");

	//get the location in respect to the bottom right correner of hte map
	int int_x_loc = 15;
	int int_y_loc = 1;
	int int_z_loc = 15;
	//*2.0 is the cube offset vaule
	glm::vec3 location = glm::vec3(int_x_loc * 2.0, int_y_loc * 2.0, int_z_loc * 2.0);
	//block the locations
	block_loc *loc_temp;
	loc_temp = new block_loc(int_x_loc, int_z_loc);
	blocked_spots->push_back(loc_temp);
	loc_temp = new block_loc(int_x_loc - 1, int_z_loc);
	blocked_spots->push_back(loc_temp);
	loc_temp = new block_loc(int_x_loc + 1, int_z_loc);
	blocked_spots->push_back(loc_temp);
	loc_temp = new block_loc(int_x_loc, int_z_loc - 1);
	blocked_spots->push_back(loc_temp);
	loc_temp = new block_loc(int_x_loc, int_z_loc + 1);
	blocked_spots->push_back(loc_temp);

	//Terrian->print_map_blocked();//for debuggin the collision

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
	temp_data->type = ALTER_T;
	temp_data->x_m = int_x_loc;
	temp_data->y_m = int_y_loc;
	temp_data->z_m = int_z_loc;
	temp_data->x_scale = x_scale;
	temp_data->y_scale = y_scale;
	temp_data->z_scale = z_scale;
	temp_data->item_id = 1;
	temp_data->buffer_loc = 0;
	temp_data->item_name = item_name_t;
	temp_data->debug_id = object_id;
	temp_data->zone_location = NULL;
	object_id++;
	alter = temp_data;
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
	temp->item_name = item_name_t;
	temp->item_data.push_back(temp_data);//add the data for the object

	items.push_back(temp);

}

std::vector< item_loc>  object_manger::place_items_init() {
	std::cout << "placing the items in the world" << std::endl;
	std::vector< item_loc> output;
	for (int i = 0; i < items.size(); i++) {//each of the differnt items types
		for (int q = 0; q < items[i]->item_data.size(); q++) {
			item_info* unplaced_item = items[i]->item_data[q];
			item_loc temp(unplaced_item, unplaced_item->x_m, unplaced_item->y_m, unplaced_item->z_m);
			output.push_back(temp);
			//objects that take up more than one spot
			switch (unplaced_item->type) {
			case ALTER_T:
				temp = item_loc(unplaced_item, unplaced_item->x_m + 1, unplaced_item->y_m, unplaced_item->z_m);
				output.push_back(temp);
				temp = item_loc(unplaced_item, unplaced_item->x_m - 1, unplaced_item->y_m, unplaced_item->z_m);
				output.push_back(temp);
				temp = item_loc(unplaced_item, unplaced_item->x_m, unplaced_item->y_m, unplaced_item->z_m + 1);
				output.push_back(temp);
				temp = item_loc(unplaced_item, unplaced_item->x_m, unplaced_item->y_m, unplaced_item->z_m - 1);
				output.push_back(temp);
				break;
			}
		}
	}
	std::cout << "finished placing the items in the world" << std::endl;
	return output;
}


void object_manger::preform_sacrifice(item_info* sac) {
	//std::cout << "sacrificing " << items[sac->item_id]->item_name << std::endl;
	//std::cout << "buffer size " << items[sac->item_id]->buffer_size << std::endl;
	//std::cout << "buffer amount " << items[sac->item_id]->amount << std::endl;

	sac->x = sac->x_m * 2;
	sac->y = sac->y_m;
	sac->z = sac->z_m * 2;
	//init_sac = true;
	//ready_to_sac = false;
	items_to_sac.push(sac);
}

//returns true if sac is starting 
//returns false if it needs to be triggered latter
bool object_manger::start_sacrifice() {
	if (ready_to_sac && !init_sac) {
		init_sac = true;
		ready_to_sac = false;
		return true;
	}
	return false;
}

float diff_btwn_pnt(float start, float end) {
	float output = 0;
	if (start < end) {
		output = end - start;
	}
	else {
		output = start - end;
	}
	return output;
}

//determins whther to move the loc in a positive or negative direction
bool determin_direction(float start, float end) {
	if (start < end) {
		return true;//possitive
	}
	return false;//negative
}

void object_manger::update_alter(float deltatTime) {
	if (init_sac) {
		if (items_to_sac.size() > 0) {

			item_info* sac_item = items_to_sac.front();
			glm::mat4 trans = glm::mat4(1.0f);
			if (in_possition) {
				sac_time += deltatTime;
				if (sac_time >= max_time) {
					sac_time = 0;
				}

				//std::cout << sac_item->x << " " << sac_item->y << " " << sac_item->z << std::endl;
				trans = glm::translate(trans, glm::vec3(sac_item->x, sac_item->y, sac_item->z));
				trans = glm::rotate(trans, sin(sac_time), glm::vec3(1, 1, 1));
				sac_obj_mat = trans;
				//items[sac_item->item_id]->modelMatrices[sac_item->buffer_loc] = trans;
			}
			else {
				float dist_diff;
				float move_amount = deltatTime * 2;
				if (!same_x) {
					dist_diff = diff_btwn_pnt(sac_item->x, alter->x);
					if (dist_diff <= move_amount) {
						sac_item->x = alter->x;
						same_x = true;
					}
					else {
						if (determin_direction(sac_item->x, alter->x)) {
							sac_item->x += move_amount;
						}
						else {
							sac_item->x -= move_amount;
						}
					}
				}
				if (!same_z) {
					dist_diff = diff_btwn_pnt(sac_item->z, alter->z);
					if (dist_diff <= move_amount) {
						sac_item->z = alter->z;
						same_z = true;
					}
					else {
						if (determin_direction(sac_item->z, alter->z)) {
							sac_item->z += move_amount;
						}
						else {
							sac_item->z -= move_amount;
						}
					}
					trans = glm::translate(trans, glm::vec3(sac_item->x, sac_item->y, sac_item->z));
					items[sac_item->item_id]->modelMatrices[sac_item->buffer_loc] = trans;
					//sac_obj_mat = trans;
				}
				if (same_x && same_z) {

					delete_item_from_buffer(sac_item);
					alter_draw = true;
					in_possition = true;
				}
			}
		}
	}
}

void object_manger::delete_item_from_buffer(item_info* it) {
	//std::cout << "buffer size " << items[it->item_id]->buffer_size << std::endl;
	//std::cout << "buffer amount " << items[it->item_id]->amount << std::endl;


	if (it->buffer_loc == items[it->item_id]->amount - 1) {//if it is the last one 
		//std::cout << "last one" << std::endl;
		items[it->item_id]->item_data.pop_back();
		items[it->item_id]->amount--;
	}
	else {
		//std::cout << "not last one" << std::endl;
		unsigned int buffer_loc = it->buffer_loc;
		unsigned int buffer_loc_end = items[it->item_id]->amount - 1;
		std::cout << buffer_loc << std::endl;
		std::cout << buffer_loc_end << std::endl;
		items[it->item_id]->item_data[buffer_loc] = items[it->item_id]->item_data[buffer_loc_end];
		items[it->item_id]->item_data[buffer_loc]->buffer_loc = buffer_loc;
		items[it->item_id]->item_data.pop_back();
		items[it->item_id]->amount--;
		items[it->item_id]->modelMatrices[buffer_loc] = items[it->item_id]->modelMatrices[buffer_loc_end];
	}
	
}

item_info* object_manger::spawn_item(item_type type, int x, int z) {
	unsigned int buffer_loc;
	unsigned int item_id;
	//check to see if the buffer is large enough
	switch (type) {
	case LOG_T:
		if (items[0]->amount >= items[0]->buffer_size) {
			return NULL;
		}
		item_id = 0;
		buffer_loc = items[0]->amount;
		items[0]->amount++;
		break;
	case ALTER_T:
		if (items[1]->amount >= items[1]->buffer_size) {
			return NULL;
		}
		item_id = 1;
		buffer_loc = items[1]->amount;
		items[1]->amount++;
		break;
	}

	float x_f = x * 2;
	float y_f = 7;
	float z_f = z * 2;

	item_info* output = new item_info;
	output->amount = 1;
	output->x_m = x;
	output->y_m = 7;
	output->z_m = z;
	output->type = type;
	output->x_scale = 1;
	output->y_scale = 1;
	output->z_scale = 1;
	output->x = x_f;
	output->y = y_f;
	output->z = z_f;
	output->zone_location = NULL;
	output->item_name = items[item_id]->item_name;
	output->item_id = item_id;
	output->buffer_loc = buffer_loc;
	output->debug_id = object_id;
	object_id++;


	glm::mat4 trans = glm::mat4(1.0f);
	std::cout << "item at " << x_f << "," << y_f << "," << z_f << std::endl;
	trans = glm::translate(trans, glm::vec3(x_f, y_f, z_f));
	items[item_id]->modelMatrices[buffer_loc] = trans;

	items[item_id]->item_data.push_back(output);
	return output;
}

