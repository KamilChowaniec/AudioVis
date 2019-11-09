#include "Window.hpp"
#include "Audio.hpp"

int main() {
	Window window("elo", 1280, 720);
	Audio::init();
	Audio audio("res/audio/Running Away.mp3");
	audio.play();
	while (!window.shouldClose()) {
		window.pollEvents();
	}
}