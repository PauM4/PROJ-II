#pragma once

#include <iostream>
#include <vector>
#include "SDL/include/SDL.h"

#define FADE_DELAY 700;

struct ParticleProps {
	float x;
	float y;
	int velocity_x;
	int velocity_y;
	float spreadfactor;
	float lifetime;
	float beginscale;
	int r, g, b;
	int r2, g2, b2;
	float scaleVariation;
	float endscale;
	int particlepersecond;
	SDL_Texture* particletexture;
};

struct Particle {
	float x;
	float y;
	int velocity_x;
	int velocity_y;
	float lifetime;
	float lifetimeremaining;
	float beginscale;
	float endscale;
	int r, g, b;
	int r2, g2, b2;
	bool Active;
};

class ParticleSystem
{
public:

	ParticleSystem(ParticleProps particleProps);

	~ParticleSystem();

	void UpdateParticles(float dt);

	void Emit();
	void render();
	float lerp(float a, float b, float f);

	std::vector<Particle> ParticleList;
	ParticleProps particleProps;
	float acumulator;

private:
	int currentparticle;
};

