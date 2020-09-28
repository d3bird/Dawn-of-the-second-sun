#include "world_map.h"


world_map::world_map() {


}

world_map::~world_map() {
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void world_map::draw() {

    shader->use();
    set_uniforms();
    glBindVertexArray(quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100); // 100 triangles of 6 vertices each
    glBindVertexArray(0);
}

void world_map::set_uniforms() {
    //shader->setVec3("test", glm::vec3(1.0, 0, 1.0));
    //shader->setVec3("colors[0]", glm::vec3(1.0, 0, 0.0));
   // shader->setVec3("colors[0]", area_colors[0]);
    for (int i = 0; i < total_biome_types; i++) {
        shader->setVec3(biomes[i].loc, area_colors[i]);
    }
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
    color_bounds[0] = 1;
    color_bounds[1] = 1;
    color_bounds[2] = 1;
 
    biomes = new uniform_data[total_biome_types];

    for (int i = 0; i < total_biome_types; i++) {
        std::stringstream ss;
        ss << "colors[" << i << "]";
        biomes[i].loc = ss.str();
        biomes[i].buffer_loc = i;
        //std::cout << ss.str() << std::endl;
    }

    index = 0;
    offset = 0.1f;

    //generate the offsets
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f
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