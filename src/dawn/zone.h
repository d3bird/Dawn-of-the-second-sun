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

enum type { SPAWN, ALTER, STOCKPILE, GATHER, FARM };

struct zone_loc;//at the bottom of this file because it needs to have a pointer to the zone class

struct farm_tile {
	zone_loc* loc;
	bool tilled;
	bool needs_tendning;
	bool halted_growth;
	bool needs_harvest;
	int tending_action;//1 tilling 2 watering 3 harvest
	float grow_time;
	float needed_grow_time;
	int grown_item;
	bool work_order_given;
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
	int get_max_spots() { return storeing_que.size(); }

	//farm function
	void update(float deltaTime);//returns a list of 
	std::vector<zone_loc*>* get_grown_items() { return  grown_items; }
	std::vector<farm_tile*>* get_farm_tiles_need_work() { return farm_tiles_need_work; }

private:

	bool needs_update;

	type current_type;
	unsigned int ID;

	float cube_offset;
	
	std::queue<zone_loc*> storeing_que;//should really be stored as a heap, where the unblocked are at the top
	std::vector<zone_loc*> open_spots;//all spots that items could be sotred
	std::vector<zone_loc*> partly_filled_spots;
	std::vector<zone_loc*> filled_spots;
	std::vector<zone_loc*> blocked_spots;//spots blocked by objects not stored in the zone
	

	//farming data
	std::vector<zone_loc*> *grown_items;//items that need to be added to the world that have spawned in this zone
	float time_passed;
	float grow_time;
	std::vector<farm_tile*>* farm_tiles;
	std::vector<farm_tile*>* farm_tiles_need_work;
};

struct zone_loc {
	int x, y, z, ID;
	zone* origin;//where the zone loc came from
	zone_loc(int x1, int y1, int z1, unsigned
		int  i) {
		x = x1; y = y1; z = z1; ID = i;
		origin = NULL;
	}
};