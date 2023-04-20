#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include "Log.h"

#include<string.h>
#include<SDL_ttf/include/SDL_ttf.h>

Fonts::Fonts(bool isActive) : Module(isActive)
{

}

Fonts::~Fonts()
{

}

bool Fonts::Start() {
	//Initialize ttf
	LOG("TTF status %i", TTF_Init());

	if (TTF_Init() == -1) {
		LOG("Can't init ttf library");
		LOG(TTF_GetError());
	}

	for (int i = 0; i < MAX_FONTS; ++i) {
		fontsTTF[i] = nullptr;
	}

	gameFont = LoadTTF("Assets/fonts/SwanTales.ttf", 18); 

	return true; 

}

bool Fonts::CleanUp()
{
	
	UnloadAllTTF();
	TTF_Quit();
	

	return true;
}

bool Fonts::Update(float dt)
{

	return true;
}


// Funtcions for fonts loaded with png
// Load new texture from file path
int Fonts::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;


	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	font.totalLength = strlen(characters);
	font.columns = fonts[id].totalLength / rows;

	uint tex_w, tex_h;
	app->tex->GetSize(tex, tex_w, tex_h);
	font.char_w = tex_w / font.columns;
	font.char_h = tex_h / font.rows;

	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void Fonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr)
	{
		app->tex->UnLoad(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void Fonts::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 0.0f, false);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}

// Functions for fonts loaded with TTF

int Fonts::LoadTTF(const char* fontPath, int fontSize) {
	int id = -1;
	if (fontPath == NULL || fontSize == NULL) {
		LOG("Could not load font...");
		return id;
	}

	id = 0;

	for (; id < MAX_FONTS; ++id)
		if (fontsTTF[id] == nullptr)
			break;

	if (id == MAX_FONTS) {
		LOG("Cannot load font %s. Array is full (max %d).", fontPath, MAX_FONTS);
		return id;
	}

	//Open font and set size
	fontsTTF[id] = TTF_OpenFont(fontPath, fontSize);

	if (fontsTTF[id] == nullptr) {
		LOG("can't load font");
		LOG(TTF_GetError());

		return -1;
	}
	else {
		LOG("font loaded succesfully");
	}

	return id;
}

void Fonts::UnloadTTF(int font_id) {
	if (font_id >= 0 && font_id < MAX_FONTS && fontsTTF[font_id] != nullptr)
	{
		TTF_CloseFont(fontsTTF[font_id]);
		fontsTTF[font_id] = nullptr;
		LOG("Successfully Unloaded font_id %d", font_id);
	}
}

void Fonts::UnloadAllTTF() {
	for (int i = 0; i < MAX_FONTS; i++)
	{
		if (fontsTTF[i] != nullptr)
		{
			TTF_CloseFont(fontsTTF[i]);
			fontsTTF[i] = nullptr;
			LOG("Successfully Unloaded font_id %d", i);
		}
	}
}

SDL_Texture* Fonts::LoadRenderedText(SDL_Rect& rect, int font_id, const char* text, SDL_Color color) {

	if (fontsTTF[font_id] == NULL)
	{
		LOG("The font %i is empty", font_id);
		return nullptr;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(fontsTTF[font_id], text, color);
	SDL_Texture* tex = nullptr;

	if (surface == NULL)
	{
		LOG(TTF_GetError());
		return nullptr;
	}
	else {

		tex = SDL_CreateTextureFromSurface(app->render->renderer, surface);

		if (tex == nullptr)
		{
			LOG("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());

		}
		else {
			//Get image dimensions
			rect = { 0, 0, surface->w, surface->h };
		}

		SDL_FreeSurface(surface);
	}
	return tex;
}

SDL_Texture* Fonts::LoadRenderedParagraph(SDL_Rect& rect, int font_id, const char* text, SDL_Color color, uint32 wrapedLength) {
	if (fontsTTF[font_id] == NULL)
	{
		LOG("The font %i is empty", font_id);
		return nullptr;
	}

	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(fontsTTF[font_id], text, color, wrapedLength);
	SDL_Texture* tex = nullptr;

	if (surface == NULL)
	{
		LOG(TTF_GetError());
		return nullptr;
	}
	else {

		//creates the texture with the text
		tex = SDL_CreateTextureFromSurface(app->render->renderer, surface);

		if (tex == nullptr)
		{
			LOG("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());

		}
		else {
			//Get image dimensions
			rect = { 0, 0, surface->w, surface->h };
		}

		SDL_FreeSurface(surface);

	}
	return tex;
}


bool Fonts::DrawText(const char* text, int posx, int posy, int w, int h, SDL_Color color, int font_id, bool followCam) {
	
	bool ret = true; 
	int result = -1; 
	SDL_Rect rect = { posx, posy, w, h };
	SDL_Texture* texture = LoadRenderedText(rect, font_id, text, color);

	if (followCam)
	{
		result = app->render->DrawTexture(texture, posx - app->render->camera.x, posy - app->render->camera.y, NULL);
	}
	else
	{
		result = app->render->DrawTexture(texture, posx, posy, NULL);
	}

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	SDL_DestroyTexture(texture);
	return ret;
}