#include "beast_manager.h"

beast_manager::beast_manager(){
	amount = 1;
    max_amount = 4;
    beast_shader = NULL;
}

beast_manager::~beast_manager(){

}


void beast_manager::draw() {
    updateBuffer();

    beast_shader->use();
    beast_shader->setMat4("projection", projection);
    beast_shader->setMat4("view", view);
    beast_shader->setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, beast->textures_loaded[0].id);
    for (unsigned int i = 0; i < beast->meshes.size(); i++)
    {
        glBindVertexArray(beast->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, beast->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
}

float beast_manager::diff_btwn_pnt(float start, float end) {
    float output = 0;
    if (start < end) {
        output = end - start;
    }
    else {
        output = start - end;
    }
    return output;
}

//determins whther to move the loc in a positive or negative direction
bool beast_manager::determin_direction(float start, float end) {
    if (start < end) {
        return true;//possitive
    }
    return false;//negative
}

void beast_manager::update(float deltaTime) {
    float speed = deltaTime * 8;

    glm::mat4 trans = glm::mat4(1.0f);
    glm::vec3* nav_point;
    glm::vec3* current_loc;

    //procces the wandering beasts
    for (unsigned int i = 0; i < wandering.size(); i++) {
        trans = glm::mat4(1.0f);
        nav_point = wandering[i]->get_next_nav_point();


        if (nav_point != NULL) {

            current_loc = wandering[i]->get_loc();
            trans = glm::scale(trans, glm::vec3(0.25, 0.25, 0.25));

            bool reached_x = false;
            bool reached_z = false;
            float temp;//remaining distance
            //move x
            temp = diff_btwn_pnt(current_loc->x, nav_point->x);

            if (speed <= temp) {
                // std::cout << current_loc->x << " " << current_loc->y << " " << current_loc->z << " || " << speed << " <> " << temp << " && " << nav_point->x << std::endl;
                if (determin_direction(current_loc->x, nav_point->x)) {
                    current_loc->x += speed;
                }
                else {
                    current_loc->x -= speed;
                }
            }
            else {
                //std::cout << "can not move anymore on x axis" << std::endl;
                current_loc->x = nav_point->x;
                reached_x = true;
            }

            //move z
            temp = diff_btwn_pnt(current_loc->z, nav_point->z);

            if (speed <= temp) {
                // std::cout << current_loc->x << " " << current_loc->y << " " << current_loc->z << " || " << speed << " <> " << temp << " && " << nav_point->x << std::endl;
                if (determin_direction(current_loc->z, nav_point->z)) {
                    current_loc->z += speed;
                }
                else {
                    current_loc->z -= speed;
                }
            }
            else {
                //std::cout << "can not move anymore on x axis" << std::endl;
                current_loc->z = nav_point->z;
                reached_z = true;
            }

            trans = glm::translate(trans, glm::vec3(current_loc->x, current_loc->y, current_loc->z));
            beast_matrices[wandering[i]->get_buffer_loc()] = trans;
             //update held items
            if (wandering[i]->is_holding_item()) {
                objects->update_item_matrix(wandering[i]->generate_item_update());
            }
            if (reached_z && reached_x) {
                wandering[i]->pop_nav_point();
            }
        }
        else {//for debuggin only
          // std::cout << "no nav point for " << i<<std::endl;
        }

    }

}

void beast_manager::init() {
    std::cout << "creating the beast_manger class" << std::endl;
    
    if (beast_shader == NULL) {
        beast_shader = new Shader("asteroids.vs", "asteroids.fs");
    }
    else {
        std::cout << "using a premade shader for the beast_master object" << std::endl;
    }
    //beast = new Model("resources/objects/cube/cube.obj");
    beast = new Model("resources/objects/planet/planet.obj");

    beast_matrices = new glm::mat4[max_amount];

    float x = 0;
    float y = 3;
    float z = 0;

    std::vector<glm::vec3*> nav_points= map->find_path(8, 0, 0, 0, 3);

    for (unsigned int i = 0; i < amount; i++){
        creature* temp = new creature();
        temp->hold_item(objects->get_item_info());
        temp->set_scale(4);
        x = nav_points[0]->x * 4;
        z = nav_points[0]->z * 4;
        glm::vec3* start_loc = new glm::vec3(x, y, z);
        temp->set_loc(start_loc);
        temp->set_buffer_loc(i);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.25, 0.25, 0.25));
        trans = glm::translate(trans, glm::vec3(x, y, z));
        beast_matrices[i] = trans;

        for (size_t i = 0; i < nav_points.size(); i++)
        {
            glm::vec3* temp2 = nav_points[i];
            temp2->x *= 4;// compensate for the cubes scale
            temp2->z *= 4;
            temp->add_nav_point(nav_points[i]);
        }

        nav_points = map->find_path(0, 0, 8, 0, 3);

        for (size_t i = 0; i < nav_points.size(); i++)
        {
            glm::vec3* temp2 = nav_points[i];
            temp2->x *= 4;// compensate for the cubes scale
            temp2->z *= 4;
            temp->add_nav_point(nav_points[i]);
        }

        nav_points = map->find_path(9, 0, 4, 6, 3);

        for (size_t i = 0; i < nav_points.size(); i++)
        {
            glm::vec3* temp2 = nav_points[i];
            temp2->x *= 4;// compensate for the cubes scale
            temp2->z *= 4;
            temp->add_nav_point(nav_points[i]);
        }

        wandering.push_back(temp);
        
        x += 4;
    }

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, max_amount * sizeof(glm::mat4), &beast_matrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < beast->meshes.size(); i++)
    {
        unsigned int VAO = beast->meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    std::cout << "finished creating the beast_manger class" << std::endl;
}

void beast_manager::spawn_creature(zone* spawn_zone) {

    if (amount < max_amount && spawn_zone != NULL) {

        zone_loc* spawn_loc = spawn_zone->get_spawn_loc();

        std::cout << "spawning creature" << std::endl;

        if (spawn_zone == NULL) {
            std::cout << "failed to spawn a creature" << std::endl;
            return;
        }
        //the map locaions
        float x = spawn_loc->x;
        float y = spawn_loc->y + 7;
        float z = spawn_loc->z;
        glm::vec3* start_loc_map = new glm::vec3(x, y-7, z);
        //factor in the cube offset
        x *= 2.0;

        z *= 2.0;
        //factor in the scale offset
        x *= 4;

        z *= 4;

        glm::vec3* start_loc = new glm::vec3(x, y, z);


        std::cout << "x = " << x / 4 << " y = " << y << " z = " << z / 4 << std::endl;

        creature* temp = new creature();
        //temp->hold_item(objects->get_item_info());
        temp->set_scale(4);


        temp->set_loc(start_loc);
        temp->set_loc_map(start_loc_map);

        temp->set_buffer_loc(amount);
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::scale(trans, glm::vec3(0.25, 0.25, 0.25));
        trans = glm::translate(trans, glm::vec3(x, y, z));
        beast_matrices[amount] = trans;
        amount++;
        wandering.push_back(temp);
    }
    else {
        std::cout << "can not spawn another creature, buffer filled" << std::endl;
    }
}


void beast_manager::assign_task(int creature_id, task* Job) {

    wandering[creature_id]->set_current_job(Job);
    glm::vec3* start = wandering[creature_id]->get_loc_map();
    glm::vec3* destination = Job->dest;
    wandering[creature_id]->clear_travel();
    std::cout << std::endl;
    std::cout <<"giving task"<< std::endl;
    std::cout << start->x<<" "<< start->z << " " << destination->x << " " <<
        destination->z<<" "<< start->y << std::endl;
   // while (true);
    std::vector<glm::vec3*> nav_points = 
        map->find_path(start->z, start->x, destination->z, destination->x, start->y);

    for (size_t i = 0; i < nav_points.size(); i++)
    {
        glm::vec3* temp2 = nav_points[i];
        temp2->x *= 4;// compensate for the cubes scale
        temp2->z *= 4;
        wandering[creature_id]->add_nav_point(nav_points[i]);
    }
}