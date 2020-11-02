#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>

class timing {
public:
	timing(bool frames = true);
	~timing();

	void update_time();
	float* get_time_change() { return deltaTime; }

	void toggle_frame_rates() { draw_frame_rate = !draw_frame_rate; }

private:

	//timing between frames
	double lastTime;
	float currentFrame;
	float lastFrame;
	float* deltaTime;
	int nbFrames;
	bool draw_frame_rate;
};