#pragma once
#ifndef __SCENEFOXQUEST_H__
#define __SCENEFOXQUEST_H__

#include "Module.h"
#include "Animation.h"
#include "Point.h"

struct SDL_Texture;



enum class TileState {
	PLAYER, 
	WALL, 
	ROCK,
	PULLROCK,
	EMPTY,
	ENTRANCE, 
	EXIT,
	CHEST
};

enum class Direction {
	UP, 
	DOWN, 
	LEFT, 
	RIGHT
};

struct Tile {
	int x, y;

	TileState state;
};

class TilePlayer {
public:
	TilePlayer(); 
	~TilePlayer();

	void Move(int x, int y);

	void Draw(); 

	void SetAnimation(int x, int y); 

	void SetPlayerPos(int x, int y);

	bool CleanUp();  

public:
	iPoint pos; 
	iPoint tilePos; 
	SDL_Texture* texture; 
	Animation* currentAnimation;
	Animation walkDownAnim;
	Animation walkUpAnim;
	Animation walkRightAnim;
	Animation walkLeftAnim;
	Animation idleAnim;
	Animation idleUp; 
	Animation idleRight; 
	Animation idleLeft; 
	Direction direction; 
	bool isMoving; 


};

class SceneFoxQuest : public Module
{
public:

	SceneFoxQuest(bool isActive);

	// Destructor
	virtual ~SceneFoxQuest();

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

	void CreateMap(); 

	//Check if something hits with wall/rock
	bool HitWall(int x, int y); 
	bool HitRock(int x, int y); 
	
	void Movement(); 

	bool PushRock(int moveX, int moveY, int pX, int pY);

	bool PullRock(int moveX, int moveY, int pX, int pY);



public:
	Tile* map[10][10] = {0};

	TilePlayer* player;  

	SDL_Texture* rockTexture; 

	SDL_Texture* mapTexture; 

	bool movingRock;
	bool rockQuestCompleted;


	//SFX audios
	int cantmoveFx = -1;
	int sliderockFx = -1;
	int cavewalkFx = -1;

private:

	int mapLength; 

	int mapHeigth; 
};

#endif // __SCENEFOXQUEST_H__