#include "zone.h"


zone::zone(type i, unsigned int id){
	current_type = i;
	ID = id;
	cube_offset = 2.0f;
}

zone::~zone(){

}

//spots are in respect to the terrian map
void zone::add_spot(int x, int y, int z, bool blocked) {
	//zone_loc* temp = new zone_loc(x* cube_offset, y * cube_offset, z * cube_offset);
	zone_loc* temp = new zone_loc(x, y , z, ID);
	if (!blocked) {
		storeing_que.push(temp);
		open_spots.push_back(temp);
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
	default:
		type_s = "DEF";
		break;
	}
	std::cout << "type " << type_s << " " << std::endl;
	std::cout << "open spots, " << storeing_que.size() << " " << std::endl;
}