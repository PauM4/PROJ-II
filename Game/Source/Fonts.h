#ifndef __MODULE_FONTS_H__
#define __MODULE_FONTS_H__

#include "Module.h"
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 10
#define MAX_FONT_CHARS 256

struct SDL_Texture;

struct Font
{
	// Lookup table. All characters displayed in the same order as the texture
	char table[MAX_FONT_CHARS];

	// The font texture
	SDL_Texture* texture = nullptr;

	//Used only to TTF
	const char* text;
	SDL_Rect dpsRect; //To store the text texture dimensions

	// Font setup data
	uint totalLength;
	uint rows, columns;
	uint char_w, char_h;
};

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

	// Loads a font file from a texture
	// Returns a font index from the fonts array
	// Param texturePath	- The path to the texture file
	// Param characters		- The lookup table. All characters displayed in the same order as the texture
	// Param rows			- The amount of character rows in the texture
	int Load(const char* texturePath, const char* characters, uint rows = 1);

	// Removes a font by its index
	// Unloads the texture and removes it from the fonts array
	void UnLoad(int fontIndex);

	// Create a surface from text
	void BlitText(int x, int y, int fontIndex, const char* text) const;



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

	bool DrawText(const char* text, int posx, int posy, int w, int h, SDL_Color color, int font_id);

	int gameFont;

private:
	// An array to keep track and store all loaded fonts
	Font fonts[MAX_FONTS];

	TTF_Font* fontsTTF[MAX_FONTS];
};



#endif // __MODULE_FONTS_H__