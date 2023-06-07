#ifndef __MODULE_FONTS_H__
#define __MODULE_FONTS_H__

#include "Module.h"
#include "Fonts.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 6
#define MAX_FONT_CHARS 256

struct SDL_Texture;

class Fonts : public Module
{
public:

	// Constructor
	Fonts(bool isActive);

	// Destructor
	virtual ~Fonts();

	bool Start();
	bool CleanUp();
	bool Update(float dt);

	//TTF Fonts
	int LoadTTF(const char* fontPath, int fontSize);

	void UnloadTTF(int id);

	void UnloadAllTTF();

	// Create a surface from font
	// Returns a font index from the fonts array
	// Param rect			- The rectangle where the texture dimensions will be stored
	// Param fontIndex		- The index to refer to a font 
	// Param text			- The text to dispaly
	// Param color			- The color to render the text
	SDL_Texture* LoadRenderedText(SDL_Rect& rect, int font_id, const char* text, SDL_Color color);

	SDL_Texture* LoadRenderedParagraph(SDL_Rect& rect, int font_id, const char* text, SDL_Color color, uint32 wrapedLength);

	bool DrawText(const char* text, int posx, int posy, int w, int h, SDL_Color color, int font_id = app->fonts->gameFont, bool followCam = false);

	int gameFont;
	int gameFontNotThatBig;
	int gameFontBig;
	int battleFont;

private:

	TTF_Font* fontsTTF[MAX_FONTS];

	SDL_Texture* texture; 
};



#endif // __MODULE_FONTS_H__