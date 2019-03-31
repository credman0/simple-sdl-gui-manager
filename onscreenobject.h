#ifndef ONSCREENOBJECT_H
#define ONSCREENOBJECT_H
#include <SDL2/SDL.h>

class OnScreenObject
{
public:
	OnScreenObject(SDL_Renderer* renderer)
	{
		this->renderer = renderer;
	}
	virtual void render() = 0;
	virtual void handleEvent( SDL_Event* e ) = 0;
protected:
	SDL_Renderer* renderer;
};

#endif // ONSCREENOBJECT_H
