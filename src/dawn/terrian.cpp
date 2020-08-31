#include "terrian.h"
#include <engine/shader.h>

terrian::terrian() {

	update_projection = false;
	update_cam = false;
	projection = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    draw_mode = -1;
	cube_shader = NULL;
	draw_selected = false;
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

void terrian::draw_selection(Shader* shade) {
	updateBuffer_ter();
	
	shade->use();
	shade->setMat4("projection", projection);
	shade->setMat4("view", view);
	shade->setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cube->textures_loaded[0].id);
	shade->setBool("u_color1", true);
	shade->setBool("u_color2", false);
	shade->setBool("u_color2", false);
	if (cube_amount - 255 <= 0) {
		for (unsigned int i = 0; i < cube->meshes.size(); i++)
		{
			glBindVertexArray(cube->meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, cube->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, cube_amount);
			glBindVertexArray(0);
		}
	}
	else {
		int start = 0;
		int render = 254;
		int total = 254;
		int increment = total;
		int remaining = cube_amount;


		while (remaining > 0) {

			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, total * sizeof(glm::mat4), &cube_matrices[start], GL_STATIC_DRAW);

			for (unsigned int i = 0; i < cube->meshes.size(); i++)
			{
				glBindVertexArray(cube->meshes[i].VAO);
				glDrawElementsInstanced(GL_TRIANGLES, cube->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, render);
				glBindVertexArray(0);
			}

			start += render;
			remaining -= render;
			if (remaining < 255) {
				render = remaining;
				total = remaining;
			}
			//std::cout << remaining << std::endl;
			shade->setBool("u_color2", true);
		}
		//std::cout << "out of loop" << std::endl;
	
	}
}

void terrian::update(float delta_time) {
    switch (draw_mode)
    {

    case 1:
        update_cubes( delta_time);
        break;
    default:
        std::cout << "no update for this type type init" << std::endl;
        break;
    }
}

void terrian::update_cubes(float delta_time) {
    static float pasted_time = 0;
    static float y = 0;
    pasted_time += delta_time;

    if (pasted_time >= (3.14159 * 15)) {
         y = 0;
         pasted_time = 0;
    }
    else {
        y += delta_time;
    }
    float x = 0;
    float z = 0;

    for (unsigned int i = 0; i < cube_amount; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, glm::vec3(x, sin((x+ pasted_time)), z));
       // model = glm::translate(model, glm::vec3(x, sin(pasted_time - (pasted_time / x)), z));
        model = glm::translate(model, glm::vec3(x, sin(pasted_time - (pasted_time / x) - (pasted_time / z)), z));
        cube_matrices[i] = model;
        x += cube_offset;

        if (x == (cube_offset * x_width)) {
            z += cube_offset;
            x = 0;
        }
    }
}

void terrian::draw_cubes() {
    updateBuffer_ter();

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
	cube_amount_selected = 0;

	if (cube_shader == NULL) {
		cube_shader = new Shader("asteroids.vs", "asteroids.fs");
	}
	else {
		std::cout << "using premade shader for the cubes" << std::endl;
	}
    cube = new Model("resources/objects/cube/cube.obj");
	
	x_width = 20;
	z_width = 20;
	y_width = 0;
    cube_offset = 2.0f;
	
    terrian_map = new map_tile * [x_width];
    for (int i = 0; i < x_width; i++) {
        terrian_map[i] = new map_tile[z_width];
    }

	/* testing the path finding by setting up a test grid
	1-- > The cell is not blocked
	0-- > The cell is blocked */
	int grid[20][20] = {
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1 },
		{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	float x = 0;
	float y = 0;
	float z = 0;

	//generate the extra cubes 
	unsigned int amount_extra = 0;

	//finding out the amount of extra cubes
	for (int xi = 0; xi < x_width; xi++) {
		for (int zi = 0; zi < z_width; zi++) {
			if (grid[xi][zi] == 0) {
				amount_extra++;
			}
		}
	}

	amount_extra++;//one more for good mesure
	cube_buffer_size = (x_width * z_width) + amount_extra;
	cube_amount = (x_width * z_width);
	cube_matrices = new glm::mat4[cube_buffer_size];// the main buffer
	cube_matrices_selected = new glm::mat4[cube_buffer_size];// the buffer for the selected cubes
	links = new map_loc[cube_buffer_size];

    bool first = true;

    unsigned int xloc = 0;
    unsigned int zloc = 0;

	//creates the plain of cubes
    for (unsigned int i = 0; i < cube_amount; i++) {
        map_tile temp;
        temp.x = x;
        temp.y = y;
        temp.z = z;
        temp.g_cost = 1;
        temp.blocked = false;
        temp.buffer_loc = i;
        temp.type = 1;
        terrian_map[xloc][zloc] = temp;
		//create the buffer to map link
		map_loc temp2;
		temp2.x = xloc;
		temp2.z = zloc;
		links[i] = temp2;

        glm::mat4 model = glm::mat4(1.0f);
		if (x == 0 && z == 0) {
			y = 2 * cube_offset;
		}else if (x == (cube_offset * (x_width-1)) && z == 0){
			y = 2 * cube_offset;
		}
		else {
			y = 0;
		}
        model = glm::translate(model, glm::vec3(x, y, z));
        cube_matrices[i] = model;
        x += cube_offset;
        xloc++;
        if (x == (cube_offset * x_width)) {
            z += cube_offset;
            zloc++;
            x = 0;
            xloc = 0;
        }

    }

	x = 0;
	y = 0;
	z = 0;
	y += cube_offset;

	vector<glm::mat4> additional_cubes;
	for (int xi = 0; xi < x_width; xi++) {
		for (int zi = 0; zi < z_width; zi++) {

			if (grid[xi][zi] == 0) {
				glm::mat4 blocked_cube = glm::mat4(1.0f);
				//the coridents need to be inverted
				z = xi * cube_offset;
				x = zi * cube_offset;
				terrian_map[xi][zi].blocked = true;
				blocked_cube = glm::translate(blocked_cube, glm::vec3(x, y, z));
				blocked_cube = glm::rotate(blocked_cube, 3.14f, glm::vec3(1.0, 0.0, 0.0));
				additional_cubes.push_back(blocked_cube);
			}
		}
	}

	std::cout << "creating " << cube_amount << " of total " << cube_buffer_size << " spots" << std::endl;
	std::cout << "need to add " << additional_cubes.size() << " more cubes" << std::endl;

	for (size_t i = 0; i < additional_cubes.size(); i++)
	{
		cube_matrices[cube_amount] = additional_cubes[i];
		cube_amount++;
	}

    print_map_blocked();

	Pair src = make_pair(8, 0);
	// Destination is the left-most top-most corner 
	Pair dest = make_pair(0, 0);

	//find_path(8, 0, 0, 0, 3);//conversion to a more accepted type
	//aStarSearch(src, dest);//private function 
	

	glGenBuffers(1, &buffer_slected);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_slected);
	glBufferData(GL_ARRAY_BUFFER, cube_amount * sizeof(glm::mat4), &cube_matrices_selected[0], GL_STATIC_DRAW);

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

void terrian::select(unsigned char PixelColor[3]) {
	bool color1 = false;
	bool color2 = false;
	bool color3 = false;
	int offset = 0;
	//0 - 254 range for each number
	int buffer_loc = int(PixelColor[0]);

	if (int(PixelColor[1]) != 0) {
		color2 = true;
		offset = 255;
	}
	if (int(PixelColor[2]) != 0) {
		color2 = true;
	}
	//gl_InstanceID%255;
	//float(id)/colors_amount
	buffer_loc += offset;
	std::cout << "cube located at " << buffer_loc<< std::endl;
	
	std::cout << "loc x = " << links[buffer_loc].x << ", loc z = " << links[buffer_loc].z << std::endl;
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

void terrian::print_map() {
    if (terrian_map != NULL) {
        std::cout << "printing out internal terrian_map representation" << std::endl;
        for (int x = 0; x < x_width; x++) {
            for (int z = 0; z < z_width; z++) {
                switch (terrian_map[x][z].type)
                {
                case 1:
                    std::cout << " * ";
                    break;
                case 2:
                    std::cout << " "<< terrian_map[x][z].buffer_loc;
                    break;
                default:
                    std::cout <<x<<" "<<z<<" not defined proporly"<< std::endl;
                    break;
                }
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "the terrian_map data structure was never created" << std::endl;
    }

}

void terrian::print_map_blocked() {
    if (terrian_map != NULL) {
        std::cout << "printing out internal terrian_map representation" << std::endl;
        for (int x = 0; x < x_width; x++) {
            for (int z = 0; z < z_width; z++) {
                if (!terrian_map[x][z].blocked) {
                    std::cout << "1 ";
                }
                else {
                    std::cout << "0 ";
                }
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "the terrian_map data structure was never created" << std::endl;
    }
}

//path finding functions

std::vector<glm::vec3*> terrian::find_path(int x1, int z1, int x2,int z2, float height) {
    vector<glm::vec3*> output;

	Pair src = make_pair(x1, z1);
	Pair dest = make_pair(x2, z2);

	cell** cellDetails = aStarSearch(src, dest);

	//printf("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
		&& cellDetails[row][col].parent_j == col))
	{
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	glm::vec3* temp;
	Path.push(make_pair(row, col));
	while (!Path.empty())
	{
		pair<int, int> p = Path.top();
		Path.pop();
		float x = p.first * cube_offset;
		float z = p.second * cube_offset;
		//printf("-> (%f,%f) ",x, z);
		temp = new glm::vec3(z, height, x);
		output.push_back(temp);
	}

	//std::cout << "current path" << std::endl;
	//for (int i = 0; i < output.size(); i++) {
	//	std::cout << "x: " << output[i]->x << " y: " << output[i]->y << " z: " << output[i]->z << std::endl;
	//}

    return output;
}

bool terrian::isValid(int row, int col)
{
	// Returns true if row number and column number 
	// is in range 
	return (row >= 0) && (row < x_width) &&
		(col >= 0) && (col < z_width);
}

bool terrian::isUnBlocked(int row, int col)
{
	return !terrian_map[row][col].blocked;
}

// A Utility Function to check whether destination cell has 
// been reached or not 
bool terrian::isDestination(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
		return (true);
	else
		return (false);
}

// A Utility Function to calculate the 'h' heuristics. 
double terrian::calculateHValue(int row, int col, Pair dest)
{
	// Return using the distance formula 
	return ((double)sqrt((row - dest.first) * (row - dest.first)
		+ (col - dest.second) * (col - dest.second)));
}

// A Utility Function to trace the path from the source 
// to destination 
void terrian::tracePath(cell** cellDetails, Pair dest)
{
	printf("\nThe Path is ");
	int row = dest.first;
	int col = dest.second;

	stack<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
		&& cellDetails[row][col].parent_j == col))
	{
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	while (!Path.empty())
	{
		pair<int, int> p = Path.top();
		Path.pop();
		printf("-> (%d,%d) ", p.first, p.second);
	}

	return;
}


cell** terrian::aStarSearch(Pair src, Pair dest)
{

	if (isValid(src.first, src.second) == false)
	{
		printf("Source is invalid\n");
		return NULL;
	}


	if (isValid(dest.first, dest.second) == false)
	{
		printf("Destination is invalid\n");
		return NULL;
	}


	if (isUnBlocked(src.first, src.second) == false ||
		isUnBlocked(dest.first, dest.second) == false)
	{
		printf("Source or the destination is blocked\n");
		return NULL;
	}


	if (isDestination(src.first, src.second, dest) == true)
	{
		printf("We are already at the destination\n");
		return NULL;
	}



	// Create a closed list and initialise it to false which means 
	// that no cell has been included yet 


	bool** closedList = new bool* [x_width];
	for (int i = 0; i < x_width; i++) {
		closedList[i] = new bool[z_width];
	}

	for (int x = 0; x < x_width; x++) {
		for (int z = 0; z < z_width; z++) {
			closedList[x][z] = false;
		}
	}

	// Declare a 2D array of structure to hold the details 
	//of that cell 
	cell** cellDetails = new cell * [x_width];
	for (int i = 0; i < x_width; i++) {
		cellDetails[i] = new cell[z_width];
	}


	int i, j;

	for (i = 0; i < x_width; i++)
	{
		for (j = 0; j < z_width; j++)
		{
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	// Initialising the parameters of the starting node 
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	/*
	Create an open list having information as-
	<f, <i, j>>
	where f = g + h,
	and i, j are the row and column index of that cell
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
	This open list is implenented as a set of pair of pair.*/
	set<pPair> openList;

	// Put the starting cell on the open list and set its 
	// 'f' as 0 
	openList.insert(make_pair(0.0, make_pair(i, j)));

	// We set this boolean value as false as initially 
	// the destination is not reached. 
	bool foundDest = false;

	while (!openList.empty())
	{
		pPair p = *openList.begin();

		// Remove this vertex from the open list 
		openList.erase(openList.begin());

		// Add this vertex to the closed list 
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		/*
			Generating all the 8 successor of this cell

				N.W N N.E
				\ | /
				\ | /
				W----Cell----E
					/ | \
				/ | \
				S.W S S.E

			Cell-->Popped Cell (i, j)
			N --> North	 (i-1, j)
			S --> South	 (i+1, j)
			E --> East	 (i, j+1)
			W --> West		 (i, j-1)
			N.E--> North-East (i-1, j+1)
			N.W--> North-West (i-1, j-1)
			S.E--> South-East (i+1, j+1)
			S.W--> South-West (i+1, j-1)*/

			// If it isn’t on the open list, add it to 
			// the open list. Make the current square 
			// the parent of this square. Record the 
			// f, g, and h costs of the square cell 
			//			 OR 
			// If it is on the open list already, check 
			// to see if this path to that square is better, 
			// using 'f' cost as the measure. 

					// To store the 'g', 'h' and 'f' of the 8 successors 
		double gNew, hNew, fNew;

		//----------- 1st Successor (North) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i - 1, j) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i - 1, j, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i - 1][j] == false &&
				isUnBlocked(i - 1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i - 1, j, dest);
				fNew = gNew + hNew;


				if (cellDetails[i - 1][j].f == FLT_MAX ||
					cellDetails[i - 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i - 1, j)));

					// Update the details of this cell 
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}

		//----------- 2nd Successor (South) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i + 1, j) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i + 1, j, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i + 1][j] == false &&
				isUnBlocked(i + 1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i + 1, j, dest);
				fNew = gNew + hNew;


				if (cellDetails[i + 1][j].f == FLT_MAX ||
					cellDetails[i + 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j)));
					// Update the details of this cell 
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		//----------- 3rd Successor (East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i, j + 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j + 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j + 1] == false &&
				isUnBlocked(i, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j + 1, dest);
				fNew = gNew + hNew;


				if (cellDetails[i][j + 1].f == FLT_MAX ||
					cellDetails[i][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i, j + 1)));

					// Update the details of this cell 
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

		//----------- 4th Successor (West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i, j - 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j - 1, dest) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j - 1] == false &&
				isUnBlocked(i, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j - 1, dest);
				fNew = gNew + hNew;


				if (cellDetails[i][j - 1].f == FLT_MAX ||
					cellDetails[i][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i, j - 1)));

					// Update the details of this cell 
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}

		//----------- 5th Successor (North-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i - 1, j + 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i - 1, j + 1, dest) == true && isUnBlocked(i, j + 1) == true && isUnBlocked(i - 1, j) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i - 1][j + 1].parent_i = i;
				cellDetails[i - 1][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i - 1][j + 1] == false &&
				(isUnBlocked(i - 1, j + 1) && isUnBlocked(i, j + 1) == true && isUnBlocked(i - 1, j) == true))//prevent cornner cutting
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j + 1, dest);
				fNew = gNew + hNew;


				if (cellDetails[i - 1][j + 1].f == FLT_MAX ||
					cellDetails[i - 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i - 1, j + 1)));

					// Update the details of this cell 
					cellDetails[i - 1][j + 1].f = fNew;
					cellDetails[i - 1][j + 1].g = gNew;
					cellDetails[i - 1][j + 1].h = hNew;
					cellDetails[i - 1][j + 1].parent_i = i;
					cellDetails[i - 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 6th Successor (North-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i - 1, j - 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i - 1, j - 1, dest) == true && isUnBlocked(i, j - 1) == true && isUnBlocked(i - 1, j) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i - 1][j - 1].parent_i = i;
				cellDetails[i - 1][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i - 1][j - 1] == false &&
				(isUnBlocked(i - 1, j - 1) == true && isUnBlocked(i, j - 1) == true && isUnBlocked(i - 1, j) == true))
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j - 1, dest);
				fNew = gNew + hNew;


				if (cellDetails[i - 1][j - 1].f == FLT_MAX ||
					cellDetails[i - 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
					// Update the details of this cell 
					cellDetails[i - 1][j - 1].f = fNew;
					cellDetails[i - 1][j - 1].g = gNew;
					cellDetails[i - 1][j - 1].h = hNew;
					cellDetails[i - 1][j - 1].parent_i = i;
					cellDetails[i - 1][j - 1].parent_j = j;
				}
			}
		}

		//----------- 7th Successor (South-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i + 1, j + 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i + 1, j + 1, dest) == true && isUnBlocked(i, j + 1) == true && isUnBlocked(i + 1, j) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i + 1][j + 1].parent_i = i;
				cellDetails[i + 1][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i + 1][j + 1] == false &&
				(isUnBlocked(i + 1, j + 1) == true && isUnBlocked(i, j + 1) == true && isUnBlocked(i + 1, j) == true))
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j + 1, dest);
				fNew = gNew + hNew;


				if (cellDetails[i + 1][j + 1].f == FLT_MAX ||
					cellDetails[i + 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i + 1, j + 1)));

					// Update the details of this cell 
					cellDetails[i + 1][j + 1].f = fNew;
					cellDetails[i + 1][j + 1].g = gNew;
					cellDetails[i + 1][j + 1].h = hNew;
					cellDetails[i + 1][j + 1].parent_i = i;
					cellDetails[i + 1][j + 1].parent_j = j;
				}
			}
		}

		//----------- 8th Successor (South-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i + 1, j - 1) == true)
		{
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i + 1, j - 1, dest) == true && isUnBlocked(i, j - 1) == true && isUnBlocked(i + 1, j) == true)
			{
				// Set the Parent of the destination cell 
				cellDetails[i + 1][j - 1].parent_i = i;
				cellDetails[i + 1][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return cellDetails;
			}

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i + 1][j - 1] == false &&
				(isUnBlocked(i + 1, j - 1) == true && isUnBlocked(i, j - 1) == true && isUnBlocked(i + 1, j) == true))
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j - 1, dest);
				fNew = gNew + hNew;


				if (cellDetails[i + 1][j - 1].f == FLT_MAX ||
					cellDetails[i + 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew,
						make_pair(i + 1, j - 1)));

					// Update the details of this cell 
					cellDetails[i + 1][j - 1].f = fNew;
					cellDetails[i + 1][j - 1].g = gNew;
					cellDetails[i + 1][j - 1].h = hNew;
					cellDetails[i + 1][j - 1].parent_i = i;
					cellDetails[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}

	// When the destination cell is not found and the open 
	// list is empty, then we conclude that we failed to 
	// reach the destiantion cell. This may happen when the 
	// there is no way to destination cell (due to blockages) 
	if (foundDest == false)
		printf("Failed to find the Destination Cell\n");

	return NULL;
}
