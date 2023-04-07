#include "Enemy_LRRH.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


Enemy_LRRH::Enemy_LRRH(iPoint pos) : Entity(EntityType::LRRH){
	//texture = app->tex->Load("");

	level = 6; 
	health = 25;
	maxHealth = 25;
	attack = 13;
	defense = 7; 
	magic = 0;
	resistance = 4;
	speed = 13;
	skill = 15; 
	stamina = 15;
	maxStamina = 15;
}

bool Enemy_LRRH::Awake()
{
	return true;
}

bool Enemy_LRRH::Start()
{
	return true;
}

bool Enemy_LRRH::Update()
{
	return true;
}

bool Enemy_LRRH::PostUpdate()
{
	//Render
	return true;
}

bool Enemy_LRRH::CleanUp()
{
	return true;
}

bool Enemy_LRRH::LoadState(pugi::xml_node&)
{
	return true;
}

bool Enemy_LRRH::SaveState(pugi::xml_node&)
{
	return true;
}

void Enemy_LRRH::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

int Enemy_LRRH::GetState()
{
	return 0;
}

int Enemy_LRRH::Attack()
{
	return 0;
}

int Enemy_LRRH::Ability(int id)
{
	return 0;
}

void Enemy_LRRH::Movement()
{

}
