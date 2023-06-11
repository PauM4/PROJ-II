#ifndef __MODULEPARTICLES_H__
#define __MODULEPARTICLES_H__

#include "Module.h"

#include "SDL/include/SDL.h"

class ParticleSystem;

class ModuleParticles : public Module
{

public:

	ModuleParticles(bool isActive);

	// Destructor
	virtual ~ModuleParticles();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	std::vector<ParticleSystem*> emiters;
	
};

#endif // __MODULEPARTICLES_H__