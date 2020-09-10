#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string>

enum type { SPAWN, ALTER, STOCKPILE, GATHER };

struct zone_loc { 
	int x, y, z; 
	zone_loc(int x1, int y1, int z1) { x = x1; y = y1; z = z1; }
};

class zone{
public:
	zone(type i);
	~zone();

	void add_spot(int x, int y, int z, bool blocked = false);
	void remove_spot(int x, int y, int z);

	void block_spot(int x, int y, int z);
	void unblock_spot(int x, int y, int z);

	std::vector<zone_loc*> get_openspts() { return open_spots; }

	void set_ID(unsigned int i) { ID = i; }
	void set_type(type i) { current_type = i; }
	type get_type() { return current_type; }

	zone_loc* get_spawn_loc();

	void print_info();

private:

	type current_type;
	unsigned int ID;
	
	float cube_offset;

	std::queue<zone_loc*> storeing_que;//should really be stored as a heap, where the unblocked are at the top
	std::vector<zone_loc*> open_spots;
	std::vector<zone_loc*> blocked_spots;

};

