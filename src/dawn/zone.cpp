#include "zone.h"


zone::zone(type i){
	current_type = i;
}

zone::~zone(){

}


void zone::add_spot(int x, int y, int z, bool blocked) {
	zone_loc* temp = new zone_loc(x, y, z);
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