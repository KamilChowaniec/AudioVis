#include "Window.hpp"

int main() {
	Window window("elo",1280, 720);
	while (window.shouldClose()) { 
		window.pollEvents();
	}
}