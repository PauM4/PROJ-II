#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"



struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro(bool isActive);

	// Destructor
	virtual ~SceneIntro();

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
	
	SDL_Texture* logo_image;
	SDL_Texture* logo_background;
	SDL_Texture* logo_title;

	// Window width and height
	int w, h;

	Tween logo_animation_picture;
	Tween logo_animation_title;

private:

};

#endif // __SCENEINTRO_H__