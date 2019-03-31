#include <iostream>
#include <SDL2/SDL.h>
#include "button.h"
#include "onscreenobject.h"
#include "onscreenmanager.h"

using namespace std;

#define TOTAL_BUTTONS 2
int main(int argc, char* argv[])
{
	SDL_Event event;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *screen = SDL_CreateWindow("My Game Window",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  1280, 960, SDL_WINDOW_OPENGL);

	SDL_Renderer *renderer = SDL_CreateRenderer(screen, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	OnScreenObject** renderables;
	renderables = (OnScreenObject**)malloc(TOTAL_BUTTONS*sizeof(OnScreenObject*));
	for (int i = 0; i < TOTAL_BUTTONS; i++){
		renderables[i] = new Button(renderer);
	}
	((Button*) renderables[0])->setPosition(500,500);
	auto buttonEvent =[](int i){
		std::cout << "Hello World " <<i;
		std::flush(std::cout);
	};
	((Button*) renderables[0])->setActionEvent(buttonEvent);

	OnScreenManager manager(renderer);
	manager.addLayer();
	for (int i = 0; i < TOTAL_BUTTONS; i++){
		manager.addOnScreenObject(renderables[i],0);
	}
	manager.start();
	SDL_Delay(2000);
	SDL_Quit();
}
