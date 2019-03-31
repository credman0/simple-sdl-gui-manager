#ifndef BUTTON_H
#define BUTTON_H
#include <SDL2/SDL.h>
#include "onscreenobject.h"


//The mouse button
class Button: public OnScreenObject{
    public:
    //Sets top left position
    void setPosition( int x, int y );
    //Handles mouse event
	void handleEvent( SDL_Event* e ) override;
    Button(SDL_Renderer *renderer);
	void render() override;
	void setActionEvent(void (*funct)(int));
private:
	enum ButtonState {
        Hovered,
        Pressed,
        None
	};
	void (*eventAction)(int) = nullptr;
    SDL_Rect rect;
    ButtonState state = None;
    SDL_Texture** stateTextures;
};

#endif // BUTTON_H
