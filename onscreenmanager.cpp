#include "onscreenmanager.h"
#include <thread>
#include <SDL2/SDL.h>
#include <iostream>

OnScreenManager::OnScreenManager(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

void OnScreenManager::addOnScreenObject(OnScreenObject *object, int layer)
{
	layersMutex.lock();
	int index = layer<0? layers.size()+layer : layer;
	layers[index].push_back(object);
	layersMutex.unlock();
}

bool OnScreenManager::removeOnScreenObject(OnScreenObject *object, int layer)
{
	layersMutex.lock();
	if (layer<0){
		for (int i = 0; i < layers.size(); i++){
			if (removeOnScreenObject(object, i)){
				layersMutex.unlock();
				return true;
			}
		}
		layersMutex.unlock();
		return false;
	}else{
		std::vector <OnScreenObject*> activeLayer = layers[layer];
		auto it = activeLayer.begin();
		for (; it!=activeLayer.end(); it++){
			if (*it==object){
				activeLayer.erase(it);
				layersMutex.unlock();
				return true;
			}
		}
		layersMutex.unlock();
		return false;
	}
}

int OnScreenManager::addLayer()
{
	layersMutex.lock();
	int endPoint = layers.size();
	layers.push_back(std::vector <OnScreenObject*>());
	layersMutex.unlock();
	return endPoint;
}

int OnScreenManager::removeLayer(int layer)
{
	layersMutex.lock();
	if (layer<0){
		int endPoint = layers.size()-1;
		layers.erase(layers.end()-1);
		layersMutex.unlock();
		return endPoint;
	}else{
		layers.erase(layers.begin()+layer);
		layersMutex.unlock();
		return layer;
	}
}

int OnScreenManager::start()
{
	if (started){
		return -1;
	}
	stop = false;
	auto renderCallable = [](OnScreenManager* manager){
		manager->renderThread();
	};
	auto eventsCallable = [](OnScreenManager* manager){
		manager->eventThread();
	};
	std::thread renderThread(renderCallable, this);
	std::thread eventsThread(eventsCallable, this);
	renderThread.join();
	eventsThread.join();
	return 0;
}

void OnScreenManager::renderThread()
{
	uint frameTime;
	uint frameStart;
	while (!stop){
		frameStart = SDL_GetTicks();
		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(renderer);

		layersMutex.lock_shared();
		for(auto it = layers.begin(); it != layers.end(); it++ ){
			for (auto iti = it->begin();iti!=it->end();iti++) {
				(*iti)->render();
			}
		}
		layersMutex.unlock_shared();
		SDL_RenderPresent(renderer);
		frameTime = SDL_GetTicks() - frameStart;
		if (frameTarget > frameTime) {
			SDL_Delay(frameTarget - frameTime);
		}
	}
}

void OnScreenManager::eventThread()
{
	SDL_Event event;
	uint frameTime;
	uint frameStart;
	while (!stop){
		frameStart = SDL_GetTicks();
		while( SDL_PollEvent( &event ) != 0 ) {
			//User requests quit
			if( event.type == SDL_QUIT ) { stop = true; }
			//Handle button events
			layersMutex.lock_shared();
			for(auto it = layers.begin(); it != layers.end(); it++ ){
				for (auto iti = it->begin();iti!=it->end();iti++) {
					(*iti)->handleEvent(&event);
				}
			}
			layersMutex.unlock_shared();
		}
		frameTime = SDL_GetTicks() - frameStart;

		if (frameTarget > frameTime) {
			SDL_Delay(frameTarget - frameTime);
		}
	}
	std::cout << "DONE";
	std::flush(std::cout);
}





