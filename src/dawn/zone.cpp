#include "zone.h"


zone::zone(type i){
	current_type = i;
	zone_loc* temp = new zone_loc(-1, -1, -1);
	// storeing_que.push(temp);
	//open_spots.push_back(temp);
	//blocked_spots.push_back(temp);
	cube_offset = 2.0f;
}

zone::~zone(){

}

//spots are in respect to the terrian map
void zone::add_spot(int x, int y, int z, bool blocked) {
	//zone_loc* temp = new zone_loc(x* cube_offset, y * cube_offset, z * cube_offset);
	zone_loc* temp = new zone_loc(x, y , z );
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
		if (open_spots.size() > 0) {

			return open_spots[0];
		}
		else {
			std::cout << "no open spots in alter zone" << std::endl;
		}
	}
	else {
		std::cout << "not a alter zone" << std::endl;
	}
	return NULL;
}

zone_loc* zone::get_stockpile_loc() {

	if (current_type == STOCKPILE) {

	}
	else {
		std::cout << "not a stockpile zone" << std::endl;
	}
	return NULL;
}

void zone::print_info() {
	std::string type_s ="Asd";
	std::cout << "switch" << std::endl;
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
	std::cout << "after switch" << std::endl;
	std::cout << "type " << type_s << " " << std::endl;
	std::cout << "open spots, " << open_spots.size() << " " << std::endl;
}