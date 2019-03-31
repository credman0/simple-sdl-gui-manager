#include "button.h"

inline bool XYInRect( const SDL_Rect& rect, int x, int y )
{
	return ( ( x >= rect.x && x <= rect.x + rect.w ) && ( y >= rect.y && y <= rect.y + rect.h ) );
}

Button::Button(SDL_Renderer* renderer) : OnScreenObject(renderer)
{
	rect.x = 0;
	rect.y = 0;
	rect.w = 400;
	rect.h = 300;
	stateTextures = (SDL_Texture**)malloc(3*sizeof (SDL_Texture*));
	SDL_Surface* surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
	stateTextures[0] = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 255, 0));
	stateTextures[1] = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_CreateRGBSurface(0, rect.w, rect.h, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 255));
	stateTextures[2] = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Button::setPosition(int x, int y){
	rect.x = x;
	rect.y = y;
}

void Button::handleEvent(SDL_Event *e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEBUTTONDOWN){
		int x;
		int y;
		SDL_GetMouseState(&x, &y);
		bool inside = XYInRect(rect, x, y);
		if (!inside){
			state = None;
		}else{
			switch (e->type) {
			case SDL_MOUSEMOTION:
				state = Hovered;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (state!=Pressed){
					if (eventAction!=nullptr){
						eventAction(0);
					}
				}
				state = Pressed;
				break;
			case SDL_MOUSEBUTTONUP:
				state = Hovered;
				break;
			default:
				break;
			}
		}
	}
}

void Button::render()
{
	SDL_Rect srcRect = SDL_Rect(rect);
	srcRect.x = 0;
	srcRect.y = 0;
	SDL_Texture* activeTexture;
	switch (state) {
	case None:
		activeTexture = stateTextures[0];
		break;
	case Pressed:
		activeTexture = stateTextures[1];
		break;
	case Hovered:
		activeTexture = stateTextures[2];
		break;
	default:
		break;
	}
	SDL_RenderCopy(renderer, activeTexture, &srcRect, &rect);
}

void Button::setActionEvent(void (*funct)(int))
{
	this->eventAction = funct;
}
