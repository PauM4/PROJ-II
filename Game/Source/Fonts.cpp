#include "App.h"

#include "Textures.h"
#include "Render.h"
#include "Fonts.h"
#include "Log.h"

#include<string.h>
#include<SDL_ttf/include/SDL_ttf.h>

Fonts::Fonts(bool isActive) : Module(isActive)
{
	name.Create("Fonts");
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

	gameFont = LoadTTF("Assets/fonts/Cartoonero.ttf", 32); 
	gameFontBig = LoadTTF("Assets/fonts/Strange Tales.ttf", 128);

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

	if (texture != nullptr) {
		SDL_DestroyTexture(texture);
	}

	if (fontsTTF[font_id] == NULL)
	{
		LOG("The font %i is empty", font_id);
		return nullptr;
	}

	SDL_Surface* surface = TTF_RenderText_Blended(fontsTTF[font_id], text, color);
	texture = nullptr;

	if (surface == NULL)
	{
		LOG(TTF_GetError());
		return nullptr;
	}
	else {

		texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

		if (texture == nullptr)
		{
			LOG("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());

		}
		else {
			//Get image dimensions
			rect = { 0, 0, surface->w, surface->h };
		}

		SDL_FreeSurface(surface);
	}
	return texture;
}

SDL_Texture* Fonts::LoadRenderedParagraph(SDL_Rect& rect, int font_id, const char* text, SDL_Color color, uint32 wrapedLength) {
	if (texture != nullptr) {
		SDL_DestroyTexture(texture); 
	}

	if (fontsTTF[font_id] == NULL)
	{
		LOG("The font %i is empty", font_id);
		return nullptr;
	}

	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(fontsTTF[font_id], text, color, wrapedLength);
	texture = nullptr;

	if (surface == NULL)
	{
		LOG(TTF_GetError());
		return nullptr;
	}
	else {

		//creates the texture with the text
		texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

		if (texture == nullptr)
		{
			LOG("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());

		}
		else {
			//Get image dimensions
			rect = { 0, 0, surface->w, surface->h };
		}

		SDL_FreeSurface(surface);

	}
	return texture;
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