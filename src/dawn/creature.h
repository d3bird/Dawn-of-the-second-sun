#pragma once

#include <string>
#include <iostream>
#include <queue>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class creature{
public:

	creature();
	~creature();

	void add_nav_point(glm::vec3* i) { travel_que.push(i); }
	void clear_travel();

	//not implemented yet
	unsigned int get_id() { return id; }
	void set_id(unsigned int i) { id = i; }

	unsigned int get_buffer_loc() { return buffer_loc; }
	void set_buffer_loc(unsigned int i) { buffer_loc = i; }

	void add_next_nav_point(glm::vec3* i);
	glm::vec3* get_next_nav_point();
	void pop_nav_point(){ travel_que.pop(); }

	glm::vec3* get_loc() { return loc; }
	void set_loc(glm::vec3* i) { loc = i; }

	bool is_wandering() { return wandering; }
	void set_wandering(bool i) { wandering = i; }

	bool need_new_point() { return new_point; }

private:
	std::string name;
	unsigned int id;
	unsigned int buffer_loc;

	bool wandering;
	bool new_point;
	glm::vec3* loc;

	std::queue<glm::vec3*> travel_que;
};

