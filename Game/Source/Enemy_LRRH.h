#pragma once
#include "Entity.h"
#include "Pathfinding.h"

class Enemy_LRRH : public Entity {
	Enemy_LRRH(iPoint pos);

	bool Awake() override; 

	bool Start() override; 
	
	bool Update() override; 
	

	bool PostUpdate() override; 

	bool CleanUp() override; 

	bool LoadState(pugi::xml_node&) override; 
	
	bool SaveState(pugi::xml_node&) override; 

	void OnCollision(PhysBody* physA, PhysBody* physB) override; 

	int GetState() override; 

	//In Combat Functions

	int Attack() override; 

	int Ability(int id) override; 

	void Movement() override; 
private: 
	SDL_Texture* texture;

	

};
