#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "shader.h"
#include "model.h"
#include "creature.h"
#include "time.h"

//work order that need a condition to met 
enum condition{ START_ALTER };
struct work_order_C{
	work_order* wo;
	condition cond;
};

class beast_manager{
public:
	beast_manager();
	~beast_manager();
	
	void draw();
	void update();

	void spawn_creature(zone* spawn_zone);

	void init();

	void set_map_size(unsigned int x, unsigned int z) { x_width = x; z_width = z; }
	
	void set_projection(glm::mat4 i) { projection = i; }
	void set_cam(glm::mat4 i) { view = i;  }
	void set_terrian(terrian* i) { map = i; }
	void set_object_manger(object_manger* i) { objects = i; }
	void set_def_shader(Shader* i) { beast_shader = i; }

	void create_tasks(work_order* Job);
	void assign_backlog_task(work_order* Job);

	void set_time(timing* i) { Time = i; }

private:

	void check_conditional_jobs();

	float diff_btwn_pnt(float start, float end);

	void preform_action(work_order* Job, creature* npc);
	void remove_from_has_jobs(creature* npc);

	void inline updateBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &beast_matrices[0], GL_STATIC_DRAW);
	}
	bool determin_direction(float start, float end);

	glm::mat4 view;
	glm::mat4 projection;
	unsigned int buffer;

	unsigned int x_width;
	unsigned int z_width;

	timing* Time;
	float* deltatime;

	std::vector<creature*> all_creatures;//all the creatures 
	std::vector<creature*> need_jobs;
	std::vector<creature*> have_jobs;

	std::queue< work_order*> jobs_backlog;
	std::vector< work_order_C*> condional_jobs;

	terrian* map;
	object_manger* objects;
	Model* beast;
	glm::mat4* beast_matrices;
	Shader* beast_shader;
	unsigned int amount;
	unsigned int max_amount;
};

