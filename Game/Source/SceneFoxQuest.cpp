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
#include "SceneManager.h"

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
	app->render->camera.x = 0; 
	app->render->camera.y = 0; 
	rockTexture = app->tex->Load("Assets/Maps/World_02/Assets/Rocas_cueva.png");
	mapTexture = app->tex->Load("Assets/Maps/World_02/Assets/Cueva_puzzle.png");
	movingRock = false; 
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
	map[2][0]->state = TileState::EXIT; 
	map[5][9]->state = TileState::ENTRANCE;
	map[5][8]->state = TileState::PLAYER; 

	map[1][7]->state = TileState::ROCK;
	map[2][7]->state = TileState::ROCK;
	map[3][7]->state = TileState::ROCK;

	map[3][6]->state = TileState::PULLROCK; 
	map[6][6]->state = TileState::ROCK;
	map[7][6]->state = TileState::ROCK;

	map[1][5]->state = TileState::ROCK;
	map[2][5]->state = TileState::ROCK;
	map[4][5]->state = TileState::ROCK;
	map[5][5]->state = TileState::ROCK;
	map[6][5]->state = TileState::PULLROCK; 
	map[8][5]->state = TileState::ROCK;

	map[1][4]->state = TileState::ROCK;
	map[3][4]->state = TileState::ROCK; 
	map[5][4]->state = TileState::ROCK;
	map[7][4]->state = TileState::ROCK;

	map[2][3]->state = TileState::ROCK;
	map[3][3]->state = TileState::PULLROCK;
	map[5][3]->state = TileState::ROCK;
	map[6][3]->state = TileState::ROCK;
	map[7][3]->state = TileState::CHEST;
	map[8][3]->state = TileState::ROCK;

	map[1][2]->state = TileState::ROCK;
	map[3][2]->state = TileState::ROCK;
	map[4][2]->state = TileState::ROCK;
	map[5][2]->state = TileState::ROCK;
	map[6][2]->state = TileState::ROCK;
	map[7][2]->state = TileState::ROCK;
	map[8][2]->state = TileState::ROCK;



	player = new TilePlayer(); 

	for (int i = 0; i < mapLength; i++) {
		for (int j = 0; j < mapHeigth; j++) {
			if (map[i][j]->state == TileState::PLAYER) {
				player->SetPlayerPos(i, j);
			}
		}
	}
	app->audio->PlayMusic("Assets/Sounds/Music/music_cave.ogg", 0.1f);
	cavewalkFx = app->audio->LoadFx("Assets/Sounds/FX/fx_cave_walk.wav");
	cantmoveFx = app->audio->LoadFx("Assets/Sounds/FX/fx_cantmove.wav");
	sliderockFx = app->audio->LoadFx("Assets/Sounds/FX/fx_rock_slide.wav");

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
				if (movingRock == true) {
					player->currentAnimation = &player->idleLeft;
					player->direction = Direction::LEFT;
					movingRock = false; 
				}
				else {
					player->currentAnimation = &player->idleRight;
				}
			}
			break; 
		case Direction::LEFT:
			player->tilePos.x -= 3;
			if (player->tilePos.x <= player->pos.x * 108 + 420) {
				player->isMoving = false;
				if (movingRock == true) {
					player->currentAnimation = &player->idleRight;
					player->direction = Direction::RIGHT;
					movingRock = false;
				}
				else {
					player->currentAnimation = &player->idleLeft;
				}
			}
			break; 
		case Direction::UP:
			player->tilePos.y -= 3;
			if (player->tilePos.y <= player->pos.y * 108) {
				player->isMoving = false;
				if (movingRock == true) {
					player->currentAnimation = &player->idleAnim;
					player->direction = Direction::DOWN;
					movingRock = false;
				}
				else {
					player->currentAnimation = &player->idleUp;
				}
			}
			break; 
		case Direction::DOWN:
			player->tilePos.y += 3;
			if (player->tilePos.y >= player->pos.y * 108) {
				player->isMoving = false;
				if (movingRock == true) {
					player->currentAnimation = &player->idleUp;
					player->direction = Direction::UP;
					movingRock = false;
				}
				else {
					player->currentAnimation = &player->idleAnim;
				}
			}
			break; 
		}
		
	}

	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		CleanUp(); 
		Start();
	}

	return true;
}

bool SceneFoxQuest::PostUpdate()
{
	//Print all the map
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	SDL_Rect rectMap = { 0, 0, 1080, 1080 };
	app->render->DrawTexture(mapTexture, 420, 0, &rectMap);

	for (int i = 0; i < mapLength; i++) {
		for (int j = 0; j < mapHeigth; j++) {
			if (map[i][j]->state == TileState::ROCK) {
				SDL_Rect rect = { 326,431,108,108 };
				app->render->DrawTexture(rockTexture, i*108 + 420, j*108, &rect);
			}
			if (map[i][j]->state == TileState::PULLROCK) {
				SDL_Rect rect = { 490,436,108,108 };
				app->render->DrawTexture(rockTexture, i * 108 + 420, j * 108, &rect);
			}
		}
	}

	player->Draw();
	return ret;
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
	return map[x][y]->state == TileState::ROCK || map[x][y]->state == TileState::PULLROCK;
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
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		switch (player->direction) {
		case Direction::LEFT:
			if (map[player->pos.x - 1][player->pos.y]->state == TileState::PULLROCK) {
				PullRock(1, 0, player->pos.x, player->pos.y);
			}
			break; 
		case Direction::RIGHT:
			if (map[player->pos.x + 1][player->pos.y]->state == TileState::PULLROCK) {
				PullRock(-1, 0, player->pos.x, player->pos.y);
			}
			break; 
		case Direction::UP:
			if (map[player->pos.x][player->pos.y - 1]->state == TileState::PULLROCK) {
				PullRock(0, 1, player->pos.x, player->pos.y);
			}
			break;
		case Direction::DOWN:
			if (map[player->pos.x][player->pos.y +1]->state == TileState::PULLROCK) {
				PullRock(0, -1, player->pos.x, player->pos.y);
			}
			break;
		}
	}

	if (map[player->pos.x][player->pos.y]->state == TileState::EXIT) {
		app->sceneManager->LoadScene(GameScene::W2_SCENE); //Change to world 2
	}

	if (map[player->pos.x][player->pos.y]->state == TileState::ENTRANCE) {
		app->sceneManager->LoadScene(GameScene::W2_SCENE); //Change to world 2
	}
}

bool SceneFoxQuest::PushRock(int moveX, int moveY, int pX, int pY)
{
	int newRockX = pX + moveX; 
	int newRockY = pY + moveY; 

	//If rock is pushed into wall or rock, dont move it
	if (HitWall(newRockX, newRockY)||HitRock(newRockX, newRockY)) {
		//Audio no pot moure
		app->audio->PlayFx(cantmoveFx);
		return false; 
	}
	if (map[pX][pY]->state == TileState::ROCK) {
		map[pX][pY]->state = TileState::EMPTY;
		map[newRockX][newRockY]->state = TileState::ROCK;
	}
	else if (map[pX][pY]->state == TileState::PULLROCK) {
		map[pX][pY]->state = TileState::EMPTY;
		map[newRockX][newRockY]->state = TileState::PULLROCK;
	}
	//Audio moure roca
	app->audio->PlayFx(sliderockFx);
	LOG("PUSH ROCK");

	return true;
}

bool SceneFoxQuest::PullRock(int moveX, int moveY, int pX, int pY)
{
	int newPlayerX = pX + moveX; 
	int newPlayerY = pY + moveY; 
	movingRock = true; 

	//If rock is pushed into wall or rock, dont move it
	if (HitWall(newPlayerX, newPlayerY) || HitRock(newPlayerX, newPlayerY)) {
		//Audio no pot moure
		app->audio->PlayFx(cantmoveFx);
		return false;
	}
	player->isMoving = true; 
	
	map[pX][pY]->state = TileState::PULLROCK;
	map[pX-moveX][pY- moveY]->state = TileState::EMPTY;
	LOG("PULLED ROCK"); 
	//Audio moure roca
	app->audio->PlayFx(sliderockFx);
	//Audio moure player
	app->audio->PlayFx(cavewalkFx);
	if (moveX == 1) {
		player->direction = Direction::RIGHT;
	}
	else if (moveX == -1) {
		player->direction = Direction::LEFT;
	}
	else if (moveY == 1) {
		player->direction = Direction::DOWN;
	}
	else if (moveY == -1) {
		player->direction = Direction::UP;
	}
	player->pos.x = newPlayerX;
	player->pos.y = newPlayerY;

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

	idleLeft.PushBack({ 1350, 300, 150, 150 });
	idleLeft.loop = true; 

	idleRight.PushBack({ 1350, 450, 150, 150 });
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
		//Audio no pot moure
		app->audio->PlayFx(app->sceneFoxQuest->cantmoveFx);
		isMoving = false;
		if (x == 1) {
			direction = Direction::RIGHT; 
		}
		else if(x == -1) {
			direction = Direction::LEFT; 
		}
		else if (y == 1) {
			direction = Direction::DOWN;
		}
		else if (y == -1) {
			direction = Direction::UP; 
		}

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
			//Audio no pot moure
			app->audio->PlayFx(app->sceneFoxQuest->cantmoveFx);
			isMoving = false; 
			if (x == 1) {
				direction = Direction::RIGHT;
			}
			else if (x == -1) {
				direction = Direction::LEFT;
			}
			else if (y == 1) {
				direction = Direction::DOWN;
			}
			else if (y == -1) {
				direction = Direction::UP;
			}
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
	//Audio moure caminar
	app->audio->PlayFx(app->sceneFoxQuest->cavewalkFx);
	pos.x = newPlayerX; 
	pos.y = newPlayerY; 
	 
}

void TilePlayer::Draw() {
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, tilePos.x - 20, tilePos.y - 15, &rect);
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
