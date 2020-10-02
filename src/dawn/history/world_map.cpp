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
   // color_bounds[0] = 30;
    //std::cout << color_bounds[0] << " " << color_bounds[1] << " " << color_bounds[2] << std::endl;
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

    create_location_for_tiles();
    create_map();
    create_buffers_biomes();

    print_map(1);

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

void world_map::create_map() {


    struct loc {//temp structure used only for world generation to store open spots
        loc(int x1, int y1) { x = x1; y = y1; }
        int x;
        int y;
    };

    std::cout << "filling in the map" << std::endl;
    std::random_device rd;
    std::mt19937 mt(rd());

    std::vector<loc> openSpots;

    int open_land = (height * width) / 4;
    int placed_land = 1;

    int xmid = width / 2;
    int ymid = height / 2;

    openSpots.push_back(loc(xmid - 1, ymid));
    openSpots.push_back(loc(xmid + 1, ymid));
    openSpots.push_back(loc(xmid, ymid + 1));
    openSpots.push_back(loc(xmid, ymid - 1));

    tile_map[xmid][ymid].biome = FOREST;
    
    while (placed_land < open_land) {
        //std::cout << "openspots: " << openSpots.size() << std::endl;
        std::uniform_real_distribution<double> distribution(0, openSpots.size());
        int selection = distribution(mt);
        int x = openSpots[selection].x;
        int y = openSpots[selection].y;
        //std::cout <<"picked "<< selection <<" openspots: " << openSpots.size() << std::endl;

        if (selection != 0) {
            openSpots[selection] = openSpots[0];
        }
        openSpots.erase(openSpots.begin());

        tile_map[x][y].biome = FOREST;

        if (x + 1 < width && tile_map[x + 1][y].biome == WATER) {
            openSpots.push_back(loc(x + 1, y));
        }
        if (x - 1 >= 0 && tile_map[x - 1][y].biome == WATER) {
            openSpots.push_back(loc(x - 1, y));
        }

        if (y + 1 < height && tile_map[x][y + 1].biome == WATER) {
            openSpots.push_back(loc(x, y + 1));
        }
        if (y - 1 >= 0 && tile_map[x][y - 1].biome == WATER) {
            openSpots.push_back(loc(x, y - 1));
        }

        placed_land++;
    }
    
    //tile_map[10][0].biome = FOREST;
    //tile_map[9][0].biome = PLAIN;
}

void world_map::create_buffers_biomes() {

    //creating a swap buffer
    glm::vec2*  swap_trans = new glm::vec2[translations_amount];

    total_biome_types = 3;
    area_colors = new glm::vec3[total_biome_types];
    color_bounds = new int[total_biome_types];


    area_colors[0] = glm::vec3(1.0, 0.0, 0.0);//plains 
    area_colors[1] = glm::vec3(0.0, 1.0, 0.0);//forest
    area_colors[2] = glm::vec3(0.0, 0.0, 1.0);//water

    biomes = new uniform_data[total_biome_types];

    for (int i = 0; i < total_biome_types; i++) {
        std::stringstream ss;
        ss << "colors[" << i << "]";
        biomes[i].loc = ss.str();
        biomes[i].buffer_loc = i;
        //std::cout << ss.str() << std::endl;
    }

    std::vector< world_map_tile> water_biomes;
    std::vector< world_map_tile> plain_biomes;
    std::vector< world_map_tile> forest_biomes;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (tile_map[x][y].biome) {
            case WATER:
                water_biomes.push_back(tile_map[x][y]);
                break;
            case PLAIN:
                plain_biomes.push_back(tile_map[x][y]);
                break;
            case FOREST:
                forest_biomes.push_back(tile_map[x][y]);
                break;
            }

        }
    }

    std::cout << "there are " << water_biomes.size() << " water biome tiles" << std::endl;
    std::cout << "there are " << plain_biomes.size() << " plain biome tiles" << std::endl;
    std::cout << "there are " << forest_biomes.size() << " forest biome tiles" << std::endl;

    //creating the buffer
    int buffer_loc = 0;

    std::cout << "start buffer_loc for plains " << buffer_loc << " to ";
    for (int i = 0; i < plain_biomes.size(); i++) {
        swap_trans[buffer_loc] = plain_biomes[i].trans;
        plain_biomes[i].buffer_loc = buffer_loc;
        buffer_loc++;
    }
    std::cout << buffer_loc << std::endl;
    if (buffer_loc == 0) {
        color_bounds[0] = buffer_loc;
    }
    else {
        color_bounds[0] = buffer_loc;// +1;//plains
    }

    std::cout << "start buffer_loc for forest " << buffer_loc << " to ";
    for (int i = 0; i < forest_biomes.size(); i++) {
        swap_trans[buffer_loc] = forest_biomes[i].trans;
        forest_biomes[i].buffer_loc = buffer_loc;
        buffer_loc++;
    }
    std::cout << buffer_loc << std::endl;

    if (buffer_loc == 0) {
        std::cout << "problem with forest" << std::endl;
        color_bounds[1] = buffer_loc;
    }
    else {
        color_bounds[1] = buffer_loc;// +1;//forest
    }

    std::cout << "start buffer_loc for water " << buffer_loc << " to ";

    for (int i = 0; i < water_biomes.size(); i++) {
        swap_trans[buffer_loc] = water_biomes[i].trans;
        water_biomes[i].buffer_loc = buffer_loc;
        buffer_loc++;
    }
    std::cout << buffer_loc << std::endl;
    color_bounds[2] = buffer_loc;// +1;//water
    translations = swap_trans;
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
            tile_map[x][y].trans = translation;
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