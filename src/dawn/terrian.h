#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <set>

#include "model.h"
#include "shader.h"
#include "time.h"
#include "object_manger.h"

using namespace std;
typedef pair<int, int> Pair;
typedef pair<double, pair<int, int>> pPair;

struct cell {
	int parent_i, parent_j;
	// f = g + h 
	double f, g, h;
};

struct map_tile {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int buffer_loc;
	int type;
	bool blocked;
	int g_cost;//move cost to move to this square
	zone* zoned;
	item_info* item_on_top;
};

struct map_loc {
	unsigned int x;
	unsigned int y;
	unsigned int z;
};

struct selection_buffer {
	unsigned int buffer;
	unsigned int cube_amount;
};

//the two objects that needed to generate tasks
//the job 
enum work_jobs { STOCK_OBJ, SACRIFICE_OBJ, START_SACRIFICE, TILL_SOIL,TEND_PLANT, HARVEST_PLANT, MOVE_C };//overall
enum job_type { AGRICULTURE, RELIGION, DUMB, NONE};//determins who can do these activities
enum action { PICK_UP, DROP, SAC_OBJ, START_SAC, MOVE, TILL, TEND, HARVEST };//the action required
//job and item that needs to be interacted with
struct work_order {
	work_jobs job;
	job_type job_t;
	action *action_rq;
	unsigned int currently_on;
	unsigned int action_numbers;
	unsigned int act_currently_on;
	unsigned int location_amount;
	unsigned int loc_currently_on;
	item_info* object;//the object that needs the interaction (also usally the start point
	zone_loc* zone_location;
	map_loc* destination;//where the object needs to go (if it needs to be moved 
	bool arrived;
	farm_tile* farm_t;
};

struct stockpile_zone;
struct stock_zone_info {
	item_info* stored_item;
	zone_loc* loc;
	int zone_loc_ID;
	int stack_number;
	int max_stack;
	stockpile_zone* origin;
};

struct stockpile_zone {
	zone* zones;
	std::vector<stock_zone_info*> filled_spots;
	std::vector<stock_zone_info*> partly_spots;
	int max_spots;
	int used_spots;
};

class terrian{
public:

	terrian();

	void draw();
	void draw_selection(Shader* shade);
	void update();

	void select(unsigned char PixelColor[3]);

	void space_init();
	void cubes_init();

	std::vector<glm::vec3*> find_path(int x1, int z1, int x2,int z2, float height);

	void block_spot(int x_loc, int z_loc);
	glm::vec3 get_coridents(int x_loc, int z_loc);

	//deal with item handling from creatures
	void remove_item_from_map(item_info* i);
	void add_item_to_map(item_info* i);
	void add_item_to_alter(item_info* i);
	bool start_sac() {return OBJM->start_sacrifice(); }
	bool is_alter_ready() { return  OBJM->is_alter_ready(); }
	bool is_alter_about_to_start(){ return  OBJM->is_alter_about_to_start(); }
	void toggle_alter_about_to_start() { OBJM->toggle_alter_about_to_start(); }
	item_info* spawn_item(item_type type, int x, int z);

	//zoning function
	zone* zone_land(type tp, unsigned int id, int x1, int y1, int z1, int x2, int y2, int z2);
	void return_zone_loc(zone_loc* i);
	void update_zones();
	void harvest_farm_tile(farm_tile* tile);
	void add_item_to_stock_pile(item_info* i);
	void remove_item_to_stock_pile(item_info* i);
	void print_stockpile_zone();

	//task creation function
	void print_work_order(work_order* wo);
	work_order* generate_work_order(work_jobs work_job, int x1, int y1, int z1, farm_tile* f_tile = NULL);
	std::vector<work_order*> generate_work_order_m(work_jobs work_job, int x1, int y1, int z1, int x2 = -1, int y2 = -1, int z2 = -1);
	void delete_work_order(work_order* work_job);
	std::vector< work_order*>* get_gen_jobs_pointer() { return gen_orders; }

	//settersand getters
	void set_projection(glm::mat4 i) { projection = i; update_projection = true; }
	void set_cam(glm::mat4 i) { view = i; update_cam = true; }

	int get_draw_mode() { return draw_mode; }

	map_tile** get_map() { return terrian_map; }
	unsigned int get_x_width() { return x_width; }
	unsigned int get_y_width() { return y_width; }
	unsigned int get_z_width() { return z_width; }

	void set_cube_shader(Shader* i) { cube_shader = i; }
	void set_object_manger(object_manger* i) { OBJM = i; }

	void set_time(timing* i) { Time = i; }

	//temp functions to get the three zone types for testing
	zone* get_spawn_zone() { return spawn_zone; }
	zone* get_alter_zone() { return alter_zone; }
	zone* get_gather_zone() { return gather_zone; }

	//needs to be out for debugging
	void print_map_blocked();
	void print_map_zoned();
	void print_map_items();
	void print_map_items_stacks();
	void print_map_blocked_zones();

private:
	void import_items();
	void draw_space();
	void draw_cubes();

	void update_cubes();

	void print_map();
	

	void inline updateBuffer_ter() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, cube_amount * sizeof(glm::mat4), &cube_matrices[0], GL_STATIC_DRAW);
	}

	void inline updateBuffer_ter_selected() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, cube_amount * sizeof(glm::mat4), &cube_matrices_selected[0], GL_STATIC_DRAW);
	}

	void inline updateBuffer_space() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	}

	bool update_projection;
	bool update_cam;
	int draw_mode;

	timing* Time;
	float* deltatime;

	//common vars
	unsigned int buffer;
	unsigned int buffer_slected;
	glm::mat4 view;
	glm::mat4 projection;
	object_manger* OBJM;

	//cube terrian vars
	Model* cube;
	glm::mat4* cube_matrices;//contains all the cubes
	glm::mat4* cube_matrices_selected;//only the selected cubes
	bool draw_selected;
	unsigned int cube_amount_selected;
	Shader* cube_shader;
	unsigned int cube_amount;
	unsigned int cube_buffer_size;//amount of spaces in the buffer
	unsigned int x_width;//rows
	unsigned int z_width;//collums
	unsigned int y_width;
	float cube_offset;
	map_tile** terrian_map;
	std::vector<selection_buffer> selection_buffers;
	map_loc* links;

	//space terrian vars
	Model *rock;
	Model* planet;
	Shader* planetShader;
	Shader* asteroidShader;
	unsigned int amount = 100000;
	glm::mat4* modelMatrices;

	//path finding functions/vars
	bool isValid(int row, int col);
	bool isUnBlocked(int row, int col);
	bool isDestination(int row, int col, Pair dest);
	double calculateHValue(int row, int col, Pair dest);
	void tracePath(cell** cellDetails, Pair dest);
	cell** aStarSearch(Pair src, Pair dest);
	bool** closedList;


	//zoning data for testing
	std::vector<zone*> zones;
	zone* spawn_zone;
	zone* alter_zone;
	zone* gather_zone;
	zone* stockpile_zone_old;
	stockpile_zone* stock_obj;

	//farming data
	zone* farm_zone;
	std::vector<zone_loc*>* items_to_add;
	std::vector<farm_tile*>* farm_tiles_need_work;//agriculture orders
	std::vector< work_order*>* gen_orders;
};