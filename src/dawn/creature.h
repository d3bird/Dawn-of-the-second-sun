#pragma once

#include <string>
#include <iostream>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "terrian.h"

#include <string>

//sub jobs that break up the main job
enum sub_job { PICKUP_OBJECT, GATHER_OBJECT, DROP_OBJECT, SACRIFICE };

struct task {
	sub_job Job;
	glm::vec3* dest;
};

class creature{
public:

	creature();
	~creature();

	void add_nav_point(glm::vec3* i) { travel_que.push(i); }
	void clear_travel();

	unsigned int get_buffer_loc() { return buffer_loc; }
	void set_buffer_loc(unsigned int i) { buffer_loc = i; }

	void add_next_nav_point(glm::vec3* i);
	glm::vec3* get_next_nav_point();
	void pop_nav_point(){ travel_que.pop(); }
	int get_travel_que_size() { return travel_que.size(); }

	glm::vec3* get_loc() { return loc; }//returns the location in regards to screen cords
	void set_loc(glm::vec3* i) { loc = i; }
	glm::vec3* get_loc_map() { return loc_map; }//returns the location in regards to map
	void set_loc_map(glm::vec3* i) { loc_map = i; }

	bool is_wandering() { return wandering; }
	void set_wandering(bool i) { wandering = i; }

	bool need_new_point() { return new_point; }

	void hold_item(item_info* i) { held_item = i; holding_item = true; }
	bool is_holding_item() { return holding_item; }

	update_pak* generate_item_update();
	void set_scale(int i) { scale_factor = i; }

	void set_current_job(task*i) { current_job = i; }
	task* get_current_job() { return current_job; }

	void set_current_work_order(work_order* i) { work_or = i; }
	work_order* get_current_work_order() { return work_or; }


	void set_loc_map_int(int x1, int y1, int z1) { x = x1; y = y1; z = z1; }
	void set_loc_map_x(int xi) { x = xi; }
	void set_loc_map_y(int yi) { y = yi; }
	void set_loc_map_z(int zi) { z = zi; }

	void set_loc_map_x_d(int xi) { dx = xi; }
	void set_loc_map_y_d(int yi) { dy = yi; }
	void set_loc_map_z_d(int zi) { dz = zi; }

	int get_loc_map_x() { return x; }
	int get_loc_map_y() { return y; }
	int get_loc_map_z() { return z; }

	int get_loc_map_x_d() { return dx; }
	int get_loc_map_y_d() { return dy; }
	int get_loc_map_z_d() { return dz; }

	void swap_dest_loc();

	void set_has_job_buffer_loc(unsigned int i) { has_job_buffer_loc = i; }
	unsigned int get_has_job_buffer_loc() { return has_job_buffer_loc; }

	//not implemented yet
	unsigned int get_id() { return id; }
	void set_id(unsigned int i) { id = i; }

private:

	int x, y, z;
	int dx, dy, dz;

	std::string name;
	unsigned int id;
	unsigned int buffer_loc;

	unsigned int has_job_buffer_loc;

	bool wandering;
	bool new_point;
	bool holding_item;
	glm::vec3* loc;//in regards to the screen
	glm::vec3* loc_map;//in regards to the map
	item_info* held_item;
	//if the the scale of the model is no 1 then the coridents of the objects need to be changed
	//by the scale factor
	int scale_factor;//scale of the model

	std::queue<glm::vec3*> travel_que;

	task* current_job;
	work_order* work_or;
};

