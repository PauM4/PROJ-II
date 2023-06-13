#ifndef __ENDSCENE_H__
#define __ENDSCENE_H__

#include "Module.h"



struct SDL_Texture;

class EndScene : public Module
{
public:

	EndScene(bool isActive);

	// Destructor
	virtual ~EndScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


public:

	SDL_Texture* endComicTexture;

private:

};

#endif // __ENDSCENE_H__
