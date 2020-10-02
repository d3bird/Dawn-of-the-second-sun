#include "world_map.h"


world_map::world_map() {
    tile_map = NULL;

}

world_map::~world_map() {
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void world_map::draw() {

    shader->use();
    set_uniforms();
    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, translations_amount); // translations_amount triangles of 6 vertices each
    glBindVertexArray(0);
}

void world_map::set_uniforms() {
    //shader->setVec3("test", glm::vec3(1.0, 0, 1.0));
    //shader->setVec3("colors[0]", glm::vec3(1.0, 0, 0.0));
   // shader->setVec3("colors[0]", area_colors[0]);
    for (int i = 0; i < total_biome_types; i++) {
        shader->setVec3(biomes[i].loc, area_colors[i]);
    }

    shader->setInt("color_bounds[0]", color_bounds[0]);
    shader->setInt("color_bounds[1]", color_bounds[1]);
    shader->setInt("color_bounds[2]", color_bounds[2]);

}

void world_map::update(float deltaTime) {

}

void world_map::init() {
	std::cout << "generating world_map" << std::endl;
	//shader = new Shader("shaders/world_map.vs", "shaders/world_map.fs");
	shader = new Shader("world_map.vs", "world_map.fs");

    total_biome_types = 3;
    area_colors = new glm::vec3[total_biome_types];
    color_bounds = new int[total_biome_types];

    area_colors[0] = glm::vec3(1.0, 0.0, 0.0);
    area_colors[1] = glm::vec3(0.0, 1.0, 0.0);
    area_colors[2] = glm::vec3(0.0, 0.0, 1.0);
    color_bounds[0] = 10;
    color_bounds[1] = 20;
    color_bounds[2] = 30;
 
    biomes = new uniform_data[total_biome_types];

    for (int i = 0; i < total_biome_types; i++) {
        std::stringstream ss;
        ss << "colors[" << i << "]";
        biomes[i].loc = ss.str();
        biomes[i].buffer_loc = i;
        //std::cout << ss.str() << std::endl;
    }

    map_tile_size = 0.1f;
    map_tile_size = 0.05f;

    height = 20;
    width = 20;
    translations_amount = height * width;
    std::cout << "total tiles " << translations_amount << std::endl;
    tile_map = new world_map_tile*[width];

    for (int x = 0; x < width; x++) {
        tile_map[x] = new world_map_tile[height];
    }

    int debug_id = 0;
    //create the map and debug ids the map is always referenced x y (x = width, y = height)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            tile_map[x][y].debug_id = debug_id;
            tile_map[x][y].biome = WATER;
            debug_id++;
        }
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    print_map(1);
    create_location_for_tiles();
    create_buffers_biomes();

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * translations_amount, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float tile_cord_size = map_tile_size / 2;
    float quadVertices[] = {
        // positions     // colors
        -tile_cord_size,  tile_cord_size,  1.0f, 0.0f, 0.0f,
         tile_cord_size, -tile_cord_size,  0.0f, 1.0f, 0.0f,
        -tile_cord_size, -tile_cord_size,  0.0f, 0.0f, 1.0f,

        -tile_cord_size,  tile_cord_size,  1.0f, 0.0f, 0.0f,
         tile_cord_size, -tile_cord_size,  0.0f, 1.0f, 0.0f,
         tile_cord_size,  tile_cord_size,  0.0f, 1.0f, 1.0f
    };
 
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    
    //instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

	std::cout << "finished generating world_map" << std::endl;
}

void world_map::create_buffers_biomes() {


}

void world_map::create_location_for_tiles() {


    index = 0;
    offset = 0.1f;//space between 
    offset = map_tile_size;
    translations = new glm::vec2[translations_amount];

    std::cout << "calculated xsize " << index << " tiles" << std::endl;

    float xloc = -1;
    float yloc = 1;

    std::cout << "calculated xsize " << xloc + (offset * width) << std::endl;
    std::cout << "calculated ysize " << yloc - (offset * height) << std::endl;
    //generate the offsets
    for (int y = 0; y < height; y++) {
        yloc -= offset;
        for (int x = 0; x < width; x++) {

            glm::vec2 translation;
            xloc += offset;
            translation.x = xloc;
            translation.y = yloc;
            translations[index] = translation;
            index++; 
        }
       // std::cout << "true xsize " << xloc<< std::endl;
        xloc = -1;
    }
   // std::cout << "true ysize " << yloc << std::endl;

    std::cout << "generated " << index << " tiles" << std::endl;

}

void world_map::print_map(int data) {
    if (tile_map == NULL) {
        return;
    }
    //print debug ids
    if (data == 0) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::cout << tile_map[x][y].debug_id<<" ";
            }
            std::cout << std::endl;
        }
    }
    else if (data == 1) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                switch (tile_map[x][y].biome){
                case WATER:
                    std::cout <<"* ";
                    break;
                case PLAIN:
                    std::cout << "1 ";
                    break;
                case FOREST:
                    std::cout << "2 ";
                    break;
                }
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "not a valid print type" << std::endl;
    }
}