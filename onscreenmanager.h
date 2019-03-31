#ifndef ONSCREENMANAGER_H
#define ONSCREENMANAGER_H
#include "onscreenobject.h"
#include <vector>
#include <shared_mutex>
#include <SDL2/SDL.h>

class OnScreenManager
{
public:
	void addOnScreenObject(OnScreenObject* object, int layer = -1);
	bool removeOnScreenObject(OnScreenObject* object, int layer = -1);
	int addLayer();
	int removeLayer(int layer = -1);
	int start();
	OnScreenManager(SDL_Renderer *renderer);
private:
	SDL_Renderer* renderer;
	uint frameTarget = 30;
	uint eventTimeTarget = 20;
	bool started = false;
	bool stop = false;
	std::shared_mutex layersMutex;
	std::vector<std::vector<OnScreenObject*>> layers;
	void renderThread();
	void eventThread();
};

#endif // ONSCREENMANAGER_H
