#include "App.h"
#include "ModuleParticles.h"
#include "Window.h"
#include "ParticleSystem.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"

ModuleParticles::ModuleParticles(bool isActive) : Module(isActive)
{
	name.Create("moduleparticles");
}

ModuleParticles::~ModuleParticles()
{
}

bool ModuleParticles::Awake(pugi::xml_node&)
{
	return true;
}

bool ModuleParticles::Start()
{
	return true;
}

bool ModuleParticles::Update(float dt)
{
	for (size_t i = 0; i < emiters.size(); i++)
	{
		emiters[i]->acumulator += dt;
		while (emiters[i]->acumulator > 1.0f / emiters[i]->particleProps.particlepersecond) {
			emiters[i]->Emit();
			emiters[i]->acumulator -= 1.0f / emiters[i]->particleProps.particlepersecond;
		}
		emiters[i]->UpdateParticles(dt);
	}
	return true;
}

bool ModuleParticles::PostUpdate()
{
	for (size_t i = 0; i < emiters.size(); i++)
	{
		emiters[i]->render();
		
	}
	return true;
}

bool ModuleParticles::CleanUp()
{
	for (size_t i = 0; i < emiters.size(); i++)
	{
		RELEASE(emiters[i]);
	}
	emiters.clear();

	return true;
}
