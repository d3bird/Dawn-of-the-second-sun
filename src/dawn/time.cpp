#include "time.h"

timing::timing(bool frames) {
	deltaTime = new float(0.0f);
	lastFrame = 0.0f;
	currentFrame = 0.0f;
	lastTime = glfwGetTime();
	nbFrames = 0;
	draw_frame_rate = frames;
	time_state = 1;
	daytime = true;
	set_day();
}

timing::~timing() {
	if (deltaTime != NULL) {
		delete deltaTime;
	}
}

void timing::update_time() {
	currentFrame = glfwGetTime();
	*deltaTime = (currentFrame - lastFrame)* time_state;
	lastFrame = currentFrame;

	nbFrames++;
	if (currentFrame - lastTime >= 1.0) {
		if (draw_frame_rate) {
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));//the number of milliseconds needed to reder the frame
		}
		nbFrames = 0;
		lastTime += 1.0;
	}
}

void timing::set_day() {
	daytime = true;
	std::cout << "it is now day time" << std::endl;
}

void timing::set_night() {
	daytime = false;
	std::cout << "it is now night time" << std::endl;
}