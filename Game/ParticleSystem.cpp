#include "ParticleSystem.h"
#include "Random.h"

ParticleSystem::ParticleSystem() {

    ParticleList.resize(1000);
	currentparticle = 999;
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::UpdateParticles(float dt) {
	for (size_t i = 0; i < ParticleList.size(); i++)
	{
		if (!ParticleList[i].Active) {
			continue;
		}
		if (ParticleList[i].lifetimeremaining <= 0.0f) {
			ParticleList[i].Active = false;
			continue;
		}
		ParticleList[i].lifetimeremaining -= dt;
		ParticleList[i].x += ParticleList[i].velocity_x * dt;
		ParticleList[i].y += ParticleList[i].velocity_y * dt;
	}
}

void ParticleSystem::Emit(ParticleProps& particleProps) {
	if (currentparticle < 0) {
		currentparticle = ParticleList.size() -1;
	}

	Particle& particle = ParticleList[currentparticle];
	particle.Active = true;

	particle.x = particleProps.x;
	particle.y = particleProps.y;

	particle.beginscale = particleProps.beginscale + particleProps.scaleVariation * (Random::RandomFloat() - 0.5f);
	particle.endscale = particleProps.endscale;

	particle.velocity_x = particleProps.velocity_x;
	particle.velocity_y = particleProps.velocity_y;
	particle.velocity_x += particleProps.spreadfactor * (Random::RandomFloat() - 0.5f);
	particle.velocity_y += particleProps.spreadfactor * (Random::RandomFloat() - 0.5f);

	particle.r = particleProps.r;
	particle.g = particleProps.g;
	particle.b = particleProps.b;
	particle.r2 = particleProps.r2;
	particle.g2 = particleProps.g2;
	particle.b2 = particleProps.b2;

	particle.lifetime = particleProps.lifetime;
	particle.lifetimeremaining = particleProps.lifetime;
	currentparticle--;
}

void ParticleSystem::render(SDL_Renderer* renderer)
{
	for (size_t i = 0; i < ParticleList.size(); i++)
	{
		float life = ParticleList[i].lifetimeremaining / ParticleList[i].lifetime;
		float scale = lerp(ParticleList[i].endscale, ParticleList[i].beginscale, life);

		SDL_Rect rect = { ParticleList[i].x, ParticleList[i].y, scale, scale };

		int alpha = 255 - (ParticleList[i].lifetimeremaining * 255) / FADE_DELAY;

		SDL_SetRenderDrawColor(renderer, ParticleList[i].r, ParticleList[i].g, ParticleList[i].b, alpha);
		SDL_RenderFillRect(renderer, &rect);
	}

}

float ParticleSystem::lerp(float a, float b, float f)
{
	return a * (1.0 - f) + (b * f);
}