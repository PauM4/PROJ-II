#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window(bool isActive);

	// Destructor
	virtual ~Window();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

	uint GetHeight() const;

	uint GetWidth() const;
public:
	// The window we'll be rendering to
	SDL_Window* window;

	// The surface contained by the window
	SDL_Surface* screenSurface;
	uint width;
	uint height;

private:
	SString title;
	uint scale;
};

#endif // __WINDOW_H__