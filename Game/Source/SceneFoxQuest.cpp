#include "SceneFoxQuest.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "GuiManager.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

SceneFoxQuest::SceneFoxQuest(bool isActive) : Module(isActive)
{
	name.Create("sceneFoxQuest");
}

SceneFoxQuest::~SceneFoxQuest()
{

}

bool SceneFoxQuest::Awake(pugi::xml_node& config)
{
	mapLength = 10;
	mapHeigth = 10;
	return true;
}

bool SceneFoxQuest::Start()
{
	//Map init
	//CreateMap(); 

	for (int i = 0; i < mapLength; i++) {
		for (int j = 0; j < mapHeigth; j++) {
			map[i][j] = new Tile; 
			map[i][j]->x = i;
			map[i][j]->y = j;
			map[i][j]->state = TileState::EMPTY; 
			if (i == 0 || i == 9 || j == 0 || j == 9) {
				map[i][j]->state = TileState::WALL;
			}
		}
	}
	map[3][3]->state = TileState::PLAYER; 
	map[1][2]->state = TileState::ROCK;
	map[1][3]->state = TileState::ROCK;
	map[2][1]->state = TileState::ROCK;

	player = new TilePlayer(); 

	for (int i = 0; i < mapLength; i++) {
		for (int j = 0; j < mapHeigth; j++) {
			if (map[i][j]->state == TileState::PLAYER) {
				player->SetPlayerPos(i, j);
			}
		}
	}
	return true;
}

bool SceneFoxQuest::PreUpdate()
{
	return true;
}

bool SceneFoxQuest::Update(float dt)
{
	if (player->isMoving == false) {
		Movement();
	}
	
	if (player->isMoving == true) {
		switch (player->direction) {
		case Direction::RIGHT:
			player->tilePos.x += 3; 
			if (player->tilePos.x >= player->pos.x * 108 + 420) {
				player->isMoving = false; 
				player->currentAnimation = &player->idleAnim; 
			}
			break; 
		case Direction::LEFT:
			player->tilePos.x -= 3;
			if (player->tilePos.x <= player->pos.x * 108 + 420) {
				player->isMoving = false;
				player->currentAnimation = &player->idleAnim;
			}
			break; 
		case Direction::UP:
			player->tilePos.y -= 3;
			if (player->tilePos.y <= player->pos.y * 108) {
				player->isMoving = false;
				player->currentAnimation = &player->idleAnim;
			}
			break; 
		case Direction::DOWN:
			player->tilePos.y += 3;
			if (player->tilePos.y >= player->pos.y * 108) {
				player->isMoving = false;
				player->currentAnimation = &player->idleAnim;
			}
			break; 
		}
		
	}

	return true;
}

bool SceneFoxQuest::PostUpdate()
{
	//Print all the map
	for (int i = 0; i < mapLength; i++) {
		for (int j = 0; j < mapHeigth; j++) {
			if (map[i][j]->state == TileState::PLAYER) {
				player->Draw(); 
			}
			if (map[i][j]->state == TileState::ROCK) {

			}
		}
	}
	return true;
}

bool SceneFoxQuest::CleanUp()
{
	if (player != nullptr) {
		player->CleanUp();
	}
	return true;
}

void SceneFoxQuest::CreateMap()
{
	for (int i = 0; i < mapLength; i++) {
		for (int j = 0; j < mapHeigth; j++) {
			map[i][j]->x = i; 
			map[i][j]->y = j; 
			map[i][j]->state = (TileState)app->map->metadataLayer[i][j]; 
		}
	}
}

bool SceneFoxQuest::HitWall(int x, int y)
{
	return map[x][y]->state == TileState::WALL;
}

bool SceneFoxQuest::HitRock(int x, int y)
{
	return map[x][y]->state == TileState::ROCK; 
}

void SceneFoxQuest::Movement()
{
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		player->Move(1, 0);
		if (player->isMoving == true) {
			player->direction = Direction::RIGHT;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		player->Move(-1, 0);
		if (player->isMoving == true) {
			player->direction = Direction::LEFT;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		player->Move(0, -1);
		if (player->isMoving == true) {
			player->direction = Direction::UP;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		player->Move(0, 1);
		if (player->isMoving == true) {
			player->direction = Direction::DOWN;
		}
	}
}

bool SceneFoxQuest::PushRock(int moveX, int moveY, int pX, int pY)
{
	int newRockX = pX + moveX; 
	int newRockY = pY + moveY; 

	//If rock is pushed into wall or rock, dont move it
	if (HitWall(newRockX, newRockY)||HitRock(newRockX, newRockY)) {
		return false; 
	}
	map[pX][pY]->state = TileState::EMPTY; 
	map[newRockX][newRockY]->state = TileState::ROCK; 
	LOG("PUSH ROCK");

	return true;
}


TilePlayer::TilePlayer() {
	pos.x = 0; 
	pos.y = 0;  
	texture = app->tex->Load("Assets/Characters/Medidas_sprites_anim-sombra_def.png");
	isMoving = false; 

	//Animations

	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;

	idleUp.PushBack({0, 600, 150, 150});
	idleUp.loop = true; 

	idleLeft.PushBack({ 0, 300, 150, 150 });
	idleLeft.loop = true; 

	idleRight.PushBack({ 0, 450, 150, 150 });
	idleRight.loop = true; 

	for (int i = 0; i < 10; i++) 
	{
		walkDownAnim.PushBack({ (i * 150), 150, 150, 150 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkUpAnim.PushBack({ (i * 150), 600, 150, 150 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkRightAnim.PushBack({ (i * 150), 450, 150, 150 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.15f;

	for (int i = 0; i < 10; i++)
	{
		walkLeftAnim.PushBack({ (i * 150), 300, 150, 150 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.15f;

	currentAnimation = &idleAnim; 

}

TilePlayer::~TilePlayer()
{
	currentAnimation = nullptr; 
}

void TilePlayer::Move(int x, int y) {

	SetAnimation(x, y);  
	isMoving = true;

	int newPlayerX = pos.x + x; 
	int newPlayerY = pos.y + y; 

	if (app->sceneFoxQuest->HitWall(newPlayerX, newPlayerY)) {
		LOG("HIT WALL");
		isMoving = false;
		switch (direction) {
		case Direction::RIGHT:
			currentAnimation = &idleRight; 
			break; 
		case Direction::LEFT:
			currentAnimation = &idleLeft; 
			break; 
		case Direction::UP:
			currentAnimation = &idleUp;
			break; 
		case Direction::DOWN:
			currentAnimation = &idleAnim;
			break; 
		}
		return; 
	}
	if (app->sceneFoxQuest->HitRock(newPlayerX, newPlayerY)) {
		if (!app->sceneFoxQuest->PushRock(x, y, newPlayerX, newPlayerY)){
			LOG("CANT PUSH ROCK");
			isMoving = false; 
			switch (direction) {
			case Direction::RIGHT:
				currentAnimation = &idleRight;
				break;
			case Direction::LEFT:
				currentAnimation = &idleLeft;
				break;
			case Direction::UP:
				currentAnimation = &idleUp;
				break;
			case Direction::DOWN:
				currentAnimation = &idleAnim;
				break;
			}
			return;
		}
	}

	pos.x = newPlayerX; 
	pos.y = newPlayerY; 

	 
}

void TilePlayer::Draw() {
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, tilePos.x, tilePos.y, &rect);
	currentAnimation->Update(); 
}

void TilePlayer::SetAnimation(int x, int y)
{
	if (x > 0) {
		currentAnimation = &walkRightAnim; 
		LOG("Walking right");
	}
	else if (x < 0) {
		currentAnimation = &walkLeftAnim; 
		LOG("Walking left");
	}
	else if (y < 0) {
		currentAnimation = &walkUpAnim;
		LOG("Walking up");
	}
	else if (y > 0) {
		currentAnimation = &walkDownAnim; 
		LOG("Walking down");
	}
}

void TilePlayer::SetPlayerPos(int x, int y)
{
	int newPlayerX = pos.x + x;
	int newPlayerY = pos.y + y;

	pos.x = newPlayerX;
	pos.y = newPlayerY;

	tilePos.x = pos.x * 108 + 420; 
	tilePos.y = pos.y * 108;
}

bool TilePlayer::CleanUp()
{
	app->tex->UnLoad(texture);
	return true; 
}
