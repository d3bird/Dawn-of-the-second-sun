#include "zone.h"


zone::zone(type i, unsigned int id){
	current_type = i;
	ID = id;
	cube_offset = 2.0f;
	grown_items = NULL;
	farm_tiles_need_work = NULL;
	farm_tiles = NULL;
	time_passed = 0;
	if (i == FARM) {
		grown_items = new std::vector<zone_loc*>;
		grow_time = 5;
		farm_tiles = new std::vector<farm_tile*>;
		farm_tiles_need_work = new std::vector<farm_tile*>;

	}
}

zone::~zone(){

}

void zone::update(float deltaTime) {

	if (!needs_update) {
		std::cout << "this zone should not need to be updated" << std::endl;	
		return;
	}

	for (int i = 0; i < farm_tiles->size(); i++){
		if (!farm_tiles[0][i]->work_order_given) {
			if (farm_tiles[0][i]->tilled) {//needs to be tilled
				if (!farm_tiles[0][i]->halted_growth && !farm_tiles[0][i]->needs_harvest) {
					farm_tiles[0][i]->grow_time += deltaTime;
				}
				//std::cout << "plant tending time: "<< farm_tiles[0][i]->grow_time << std::endl;
				if (farm_tiles[0][i]->needs_tendning && //need tending
					farm_tiles[0][i]->grow_time >= farm_tiles[0][i]->needed_grow_time / 2 ) {
					
					farm_tiles[0][i]->work_order_given = true;
					farm_tiles[0][i]->tending_action = 2;
					farm_tiles_need_work->push_back(farm_tiles[0][i]);
				}
				else if (farm_tiles[0][i]->grow_time >= farm_tiles[0][i]->needed_grow_time) {//the thing to harvestible
					farm_tiles[0][i]->work_order_given = true;
					farm_tiles[0][i]->tending_action = 3;
					farm_tiles_need_work->push_back(farm_tiles[0][i]);
					farm_tiles[0][i]->needs_harvest = true;
				}
			}
			else {
				farm_tiles[0][i]->work_order_given = true;
				farm_tiles[0][i]->tending_action = 1;
				farm_tiles_need_work->push_back(farm_tiles[0][i]);
			}
		}
	}

	/*time_passed += deltaTime;
	if (time_passed >= grow_time) {
		time_passed = 0;
		std::cout << "grown an item" << std::endl;
		if (open_spots.size() > 0) {
			if (current_type == FARM) {
				int temp = open_spots.size();
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> distribution(0.0, open_spots.size());
				int NumLines = int(distribution(mt));
				srand(time(NULL) + 1);
				int spots_slot = rand() % temp;

				//std::cout << "spots_slot = " << NumLines << std::endl;
				grown_items->push_back( open_spots[NumLines]);
			}
			else {
				std::cout << "this is not a zone that can grow" << std::endl;
			}
		}
		else {
			std::cout << "this zone has no open spots" << std::endl;
		}
	}*/
}

//spots are in respect to the terrian map
void zone::add_spot(int x, int y, int z, bool blocked) {
	//zone_loc* temp = new zone_loc(x* cube_offset, y * cube_offset, z * cube_offset);
	zone_loc* temp = new zone_loc(x, y , z, ID);
	if (!blocked) {
		storeing_que.push(temp);
		open_spots.push_back(temp);
		if (current_type == FARM) {
			farm_tile* temp_f = new farm_tile;
			temp_f->loc = temp;
			temp_f->tilled = false;
			temp_f->needs_tendning = true;
			temp_f->tending_action = 0;
			temp_f->grow_time = 0;
			temp_f->needed_grow_time = 5;
			temp_f->grown_item = 1;
			temp_f->work_order_given = false;
			temp_f->halted_growth = false;
			temp_f->needs_harvest = false;
			farm_tiles->push_back(temp_f);
		}
	} else{
		blocked_spots.push_back(temp);
	}
}

void zone::remove_spot(int x, int y, int z) {

}

void zone::block_spot(int x, int y, int z){

}

void zone::unblock_spot(int x, int y, int z) {

}

void zone::add_item_to_spot(int x, int y, int z) {
	for (int i = 0; i < open_spots.size(); i++) {

	}

	std::cout << "no open spots to add items" << std::endl;
}

void zone::add_item_to_spot(zone_loc* loc) {

}

void zone::remove_item_from_spot(int x, int y, int z) {


}

void zone::remove_item_from_spot(zone_loc* loc) {
	storeing_que.push(loc);
}

zone_loc* zone::get_spawn_loc() {
	
	//std::cout << "spawning function, "<< open_spots.size()<<" " << std::endl;
	if (open_spots.size() > 0) {
		if (current_type == SPAWN) {
			int temp = open_spots.size();
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<double> distribution(0.0, open_spots.size());
			int NumLines = int(distribution(mt));
			srand(time(NULL)+1);
			int spots_slot = rand()% temp;

			//std::cout << "spots_slot = " << NumLines << std::endl;
			return open_spots[NumLines];
		}
		else {
			std::cout << "this is not a zone that can spawn" << std::endl;
		}
	}
	else {
		std::cout << "this zone has no open spots" << std::endl;
	}
	print_info();
	while (true);
	return NULL;
}

zone_loc* zone::get_alter_loc() {
	std::cout << "getting open spot in alter zone" << std::endl;
	print_info();
	if (current_type == ALTER) {
		if (storeing_que.size() > 0) {
			zone_loc* output =storeing_que.front();
			remove_item_from_spot(output);
			storeing_que.pop();
			return output;
		}
		else {
			std::cout << "no open spots in alter zone" << std::endl;
		}
	}
	else {
		std::cout << "not a alter zone" << std::endl;
	}

	//while (true);
	return NULL;
}

zone_loc* zone::get_stockpile_loc() {
	print_info();
	if (current_type == STOCKPILE) {
		if (storeing_que.size() > 0) {
			zone_loc* output = storeing_que.front();
			remove_item_from_spot(output);
			storeing_que.pop();
			return output;
		}
		else {
			std::cout << "no open spots in stockpile zone" << std::endl;
		}
	}
	else {
		std::cout << "not a stockpile zone" << std::endl;
	}
	return NULL;
}

void zone::print_info() {
	std::string type_s;
	switch (current_type)
	{
	case SPAWN:
		type_s = "SPAWN";
		break;
	case ALTER:
		type_s = "ALTER";
		break;
	case GATHER:
		type_s = "GATHER";
		break;
	case FARM:
		type_s = "FARM";
		break;
	default:
		type_s = "DEF";
		break;
	}
	std::cout << "type " << type_s << " " << std::endl;
	std::cout << "open spots, " << storeing_que.size() << " " << std::endl;
}