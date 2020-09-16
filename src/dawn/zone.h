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
	int x, y, z, ID; 
	zone_loc(int x1, int y1, int z1, unsigned 
		int  i) { x = x1; y = y1; z = z1; ID = i; }
};

class zone {
public:
	zone(type i, unsigned int id);
	~zone();

	void add_spot(int x, int y, int z, bool blocked = false);
	void remove_spot(int x, int y, int z);

	void block_spot(int x, int y, int z);
	void unblock_spot(int x, int y, int z);

	std::vector<zone_loc*> get_openspts() { return open_spots; }

	void add_item_to_spot(int x, int y, int z);
	void add_item_to_spot(zone_loc* loc);
	void remove_item_from_spot(int x, int y, int z);
	void remove_item_from_spot(zone_loc* loc);

	void set_ID(unsigned int i) { ID = i; }
	void set_type(type i) { current_type = i; }
	type get_type() { return current_type; }

	zone_loc* get_spawn_loc();
	zone_loc* get_alter_loc();
	zone_loc* get_stockpile_loc();

	void print_info();

private:

	type current_type;
	unsigned int ID;

	float cube_offset;
	
	std::queue<zone_loc*> storeing_que;//should really be stored as a heap, where the unblocked are at the top
	std::vector<zone_loc*> open_spots;//all spots that items could be sotred
	std::vector<zone_loc*> blocked_spots;//spots blocked by objects not stored in the zone
	std::vector<zone_loc*> filled_spots;
};

