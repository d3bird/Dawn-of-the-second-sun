#include "terrian.h"
#include <engine/shader.h>

terrian::terrian() {

	update_projection = false;
	update_cam = false;
	projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    draw_mode = -1;

}

void terrian::draw() {
    switch (draw_mode)
    {
    case 0:
        draw_space();
        break;
    case 1:
        draw_cubes();
        break;
    default:
        std::cout << "no draw type init" << std::endl;
        break;
    }
}


void terrian::draw_cubes() {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    cube_shader->use();
    cube_shader->setMat4("projection", projection);
    cube_shader->setMat4("view", view);
    cube_shader->setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cube->textures_loaded[0].id);
    for (unsigned int i = 0; i < cube->meshes.size(); i++)
    {
        glBindVertexArray(cube->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, cube->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, cube_amount);
        glBindVertexArray(0);
    }

}


void terrian::draw_space() {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    asteroidShader->use();
    asteroidShader->setMat4("projection", projection);
    asteroidShader->setMat4("view", view);
    planetShader->use();
    planetShader->setMat4("projection", projection);
    planetShader->setMat4("view", view);

    // draw planet
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    planetShader->setMat4("model", model);
    planet->Draw(planetShader);

    // draw meteorites
    asteroidShader->use();
    asteroidShader->setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rock->textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
    for (unsigned int i = 0; i < rock->meshes.size(); i++)
    {
        glBindVertexArray(rock->meshes[i].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }

}


void terrian::cubes_init() {
    std::cout << "creating the terrian class (cubes)" << std::endl;
    draw_mode = 1;

    cube_shader = new Shader("asteroids.vs", "asteroids.fs");

    cube = new Model("resources/objects/cube/cube.obj");
    z_width = 10;
    x_width = 10;

    cube_amount = x_width* z_width;
    cube_offset = 2.0f;

    cube_matrices = new glm::mat4[cube_amount];

    float x = 0;
    float y = 0;
    float z = 0;

    bool first = true;

    for (unsigned int i = 0; i < cube_amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, z));
        cube_matrices[i] = model;
        x += cube_offset;

        if (x == (cube_offset * x_width)) {
            z += cube_offset;
            x = 0;
        }

    }

    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, cube_amount * sizeof(glm::mat4), &cube_matrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < cube->meshes.size(); i++)
    {
        unsigned int VAO = cube->meshes[i].VAO;
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


    std::cout << "finished creating" << std::endl;
}

void terrian::space_init() {
	std::cout << "creating the terrian class (space)" << std::endl;
    draw_mode = 0;
	asteroidShader = new Shader("asteroids.vs", "asteroids.fs");
	planetShader = new Shader("planet.vs", "planet.fs");

	rock = new Model("resources/objects/rock/rock.obj");
	planet = new Model("resources/objects/planet/planet.obj");

    unsigned int amount = 100000;
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed	
    float radius = 150.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }


    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);


    for (unsigned int i = 0; i < rock->meshes.size(); i++)
    {
        unsigned int VAO = rock->meshes[i].VAO;
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


	std::cout << "finished creating" << std::endl;
}