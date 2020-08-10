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

	unsigned int get_id() { return id; }
	unsigned int get_buffer_loc() { return buffer_loc; }
	glm::vec3* get_next_nav_point() { return  travel_que.front(); travel_que.pop(); }

	bool is_wandering() { return wandering; }

private:
	std::string name;
	unsigned int id;
	unsigned int buffer_loc;

	bool wandering;

	std::queue<glm::vec3*> travel_que;
};

