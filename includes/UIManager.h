#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include <string>

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl2.h>

#include "MainMenuBar.h"
#include "Inspector.h"

using namespace std;

class UIManager {
public:
	UIManager(Inspector* inspector);

	int init();
	void render();
	void cleanUp();	
private:
	void preRender();
	void postRender();

	SDL_Window* window;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Panels
	MainMenuBar* mainMenuBar;
	Inspector* inspector;
};

#endif