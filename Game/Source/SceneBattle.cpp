#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneBattle.h"
#include "EntityManager.h"
#include "Entity.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "Physics.h"
#include "Timmy.h"

#include "Defs.h"
#include "Log.h"

SceneBattle::SceneBattle(bool isActive) : Module(isActive)
{
	name.Create("sceneBattle");
}

// Destructor
SceneBattle::~SceneBattle()
{}

// Called before render is available
bool SceneBattle::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();
	
	//This reads some parameters from xml
	if (config.child("timmy")) {
		timmy = (Timmy*)app->entityManager->CreateEntity(EntityType::TIMMY);
		timmy->parameters = config.child("timmy");
	}
	if (config.child("bunny")) {
		bunny = (Bunny*)app->entityManager->CreateEntity(EntityType::BUNNY);
		bunny->parameters = config.child("bunny");
	}
	if (config.child("enemy_angryVillager")) {
		villager = (Enemy_AngryVillager*)app->entityManager->CreateEntity(EntityType::ANGRYVILLAGER);
		villager->parameters = config.child("enemy_angryVillager");
	}
	//This reads stats from xml
	if (config.parent().child("timmy")) {
		timmy->stats = config.parent().child("timmy");
	}
	if (config.parent().child("bunny")) {
		bunny->stats = config.parent().child("bunny");
	}
	if (config.parent().child("enemy_angryVillager")) {
		villager->stats = config.parent().child("enemy_angryVillager");
	}
	app->entityManager->Awake(config);
	
	return ret;
}


// Called before the first frame
bool SceneBattle::Start()
{
	//Load map
	bool retLoad = app->map->Load(mapName, mapFolder);
	move = false;
	originSelected = false;
	pathIndex = 1;
	length=1;
	turnstart = true;
	destination = iPoint(0, 0);
	 movepressed = false;
	 attackpressed = false;
	 abiltypressed = false;
	 endturnpressed = false;
	 moveenemy = false;
	app->entityManager->Start(); 
	//Load combat map
	

	MakeCombatMap();

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	moveanim = false;
	mouseTileTex = app->tex->Load("Assets/Maps/Scenes/Path.png");
	timmytexture = app->tex->Load("Assets/Characters/Medidas_sprites_anim-sombra_def.png");
	bunnytexture = app->tex->Load("Assets/Characters/F_sprites_bunny.png");
	villagertexture = app->tex->Load("Assets/Characters/F_sprites_angry_Villager.png"); 
	originTex = app->tex->Load("Assets/Maps/Scenes/Cruz.png");

	

	timmy->tilePos = app->map->WorldToMap(timmy->position.x - app->render->camera.x, timmy->position.y - app->render->camera.y);
	bunny->tilePos = app->map->WorldToMap(bunny->position.x - app->render->camera.x, bunny->position.y - app->render->camera.y);
	villager->tilePos = app->map->WorldToMap(villager->position.x - app->render->camera.x, villager->position.y - app->render->camera.y);

	/*timmy->position = iPoint(670, 420);*/
	allentities.Add(timmy);
	allentities.Add(bunny);
	allentities.Add(villager);
	GetTurns();
	

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = COMBAT;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	origin = characterTurn->tilePos;

	targets.Clear();
	DestroyListArea();
	CreateArea(characterTurn->AttArea, 1, characterTurn->tilePos);
	GetTargets();
	// UI Things
	isPaused = false;

	return true;
}


// Called each loop iteration
bool SceneBattle::PreUpdate()
{
	bool ret = true;
	return true;
}

// Called each loop iteration
bool SceneBattle::Update(float dt)
{

	

	// Menu appear
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->pads[0].start)
	{
		// If player is in pause, close it
		if (!isPaused)
		{
			app->uiModule->currentMenuType = COMBAT;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

			isPaused = true;
		}
		// If player is NOT in pause, open it
		else
		{
			app->uiModule->currentMenuType = COMBAT_PAUSE;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

			isPaused = false;
		}
	}

	if (movepressed == true) {

		if (characterTurn->stamina >= 3) {

			move = true;
			ability = false;
			atack = false;
		}
		movepressed = false;
	}

	if (attackpressed == true) {

		if (characterTurn->stamina >= 5) {
			targets.Clear();
			DestroyListArea();
			CreateArea(characterTurn->AttArea, 1, characterTurn->tilePos);
			GetTargets();
			atack = true;
			move = false;
			ability = false;
		}
		attackpressed = false;
	}

	if (abiltypressed == true) {
		if (characterTurn->stamina >= 10) {
			targets.Clear();
			DestroyListArea();
			CreateArea(characterTurn->AttArea, 2, characterTurn->tilePos);
			GetTargets();
			ability = true;
			atack = false;
			move = false;
		}
		attackpressed = false;

	}

	if (endturnpressed == true) {

		
		atack = false;
		ability = false;
		move = false;
		turnstart = false;
		characterTurn->GainStamina(10);
		endturnpressed = false;

	}

	app->map->Draw();

	return true;
}

// Called each loop iteration
bool SceneBattle::PostUpdate()
{
	bool ret = true;
	
	timmy->tilePos = app->map->WorldToMap(timmy->position.x - app->render->camera.x , timmy->position.y - app->render->camera.y);
	bunny->tilePos = app->map->WorldToMap(bunny->position.x - app->render->camera.x, bunny->position.y - app->render->camera.y);
	villager->tilePos= app->map->WorldToMap(villager->position.x - app->render->camera.x, villager->position.y - app->render->camera.y);


	if (timmy->health <= 0) {

		timmy->isAlive = false;

	}
	
	if (bunny->health <= 0) {

		bunny->isAlive = false;
	
	}
	if (villager->health <= 0) {

		villager->isAlive = false;

	}

	if (characterTurn->isAlive == false) {
		turnstart = false;

	}



	if (turnstart == false ) {
		
		
		
		moveenemy = false;
		GetNext();
		
		origin = characterTurn->tilePos;
		if (characterTurn->isEnemy == true) {

			targets.Clear();
			DestroyListArea();
			CreateArea(characterTurn->AttArea, 1, characterTurn->tilePos);
			GetTargets();
			moveenemy = true;
		}
		
		turnstart = true;
		moveanim = false;
	}

	if (app->pathfinding->IsWalkable(origin)) {
		originSelected = true;
		

	}

	app->pathfinding->ClearLastPath();
	
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,mouseY - app->render->camera.y);
	LOG("%d %d", mouseTile.x, mouseTile.y);
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	if (app->pathfinding->IsWalkable(mouseTile) && combatMap[mouseTile.x][mouseTile.y].character!=false && move == false) {
		app->render->DrawRectangle({ highlightedTileWorld.x, highlightedTileWorld.y, 120, 120 }, 0, 143, 57, 100, true);
		
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		

		if (originSelected == true && length == 1 && move == true && moveenemy==false)
		{
			if (app->pathfinding->IsWalkable(origin) && combatMap[mouseTile.x][mouseTile.y].inRange == true && combatMap[mouseTile.x][mouseTile.y].character == false) {
				length = app->pathfinding->CreatePath(origin, mouseTile);
				destination.x = mouseTile.x;
				destination.y = mouseTile.y;
				originSelected = false;
				move = false;
				moveanim = true;
				characterTurn->UseStamina(3);

			}
			else {
				app->pathfinding->ClearLastPath();
			}
		}
		/*		else if(combatMap[mouseTile.x ][ mouseTile.y ].character != false &&length == 1)
				{
						origin = mouseTile;
						if (app->pathfinding->IsWalkable(origin)) {
							originSelected = true;
							move = true;
							characterTurn = combatMap[mouseTile.x][mouseTile.y].characterType;
						}
						app->pathfinding->ClearLastPath();

				}*/
		
	}


   const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();
	for (uint i = 0; i < lastpath->Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(lastpath->At(i)->x, lastpath->At(i)->y);

		    LOG("posTileY= %d", lastpath->At(i)->y);
		
			app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
		
	}

	// L12: Debug pathfinding
	iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
	if (app->pathfinding->IsWalkable(origin) && originSelected == true) {

	  app->render->DrawRectangle({ originScreen.x, originScreen.y, 120, 120 }, 250, 0, 0, 100, true);
	  app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	}


	


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {

		timmy->health = 0;
	}

	if (atack == true) {


		DisplayArea(1);
		DisplayEnemys();

		for (int i = 0; i < targets.Count(); i++) {
			

			if (targets.At(i)->data->tilePos == mouseTile) {
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {

					targets.At(i)->data->health = targets.At(i)->data->health - (characterTurn->attack - targets.At(i)->data->defense);
					atack = false;
					characterTurn->UseStamina(5);
					turnstart = false;
				}
			}
		}
	}
	if (ability == true) {


		DisplayArea(2);
		DisplayEnemys();

		for (int i = 0; i < targets.Count(); i++) {


			if (targets.At(i)->data->tilePos == mouseTile) {
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {

					targets.At(i)->data->health = targets.At(i)->data->health - (characterTurn->Ab1Power - targets.At(i)->data->defense);
					ability = false;
					characterTurn->UseStamina(10);
					turnstart = false;
				}
			}
		}
	}



	if (pathIndex != length) {
		length = app->pathfinding->CreatePath(origin, destination);
	}
	else {
		length = 1;
		pathIndex = 1;
		app->pathfinding->ClearLastPath();
		
		
	}

	if (length > 1) {

		Move( pathIndex, length);
	}
	else
	{
		if (characterTurn->tilePos == destination) {
			
			move = false;
			destination = iPoint(0, 0);
			turnstart = false;

		}
	}



	
	if (move == true || moveenemy==true) {
		int j = 0;
		int i = 0;
		iPoint nexTile;
		iPoint pos;
		for (i = 0; i < characterTurn->movement; i++ ) {
			for ( j = 0 ; j < characterTurn->movement-i; j++) {

				nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y + i);
				
				combatMap[nexTile.x][nexTile.y].inRange = true;
				/*arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				  
				 nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y + i);
				 combatMap[nexTile.x ][nexTile.y].inRange = true;
				/*arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/

				 nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y - i);
				 combatMap[nexTile.x][nexTile.y].inRange = true;
				/* arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/

				 nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y - i);
				 combatMap[nexTile.x][nexTile.y].inRange = true;
				/* arealist.Add(&combatMap[nexTile.x][nexTile.y]);*/
				
			}
	
		}

		for (i = 0; i < characterTurn->movement; i++) {
			for (j = 0; j < characterTurn->movement - i; j++) {

				nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y + i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
					app->pathfinding->IsWalkable(nexTile) == false) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}
				nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y + i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
					app->pathfinding->IsWalkable(nexTile) == false) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}

				nexTile = iPoint(characterTurn->tilePos.x - j, characterTurn->tilePos.y - i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
					app->pathfinding->IsWalkable(nexTile) == false) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}

				nexTile = iPoint(characterTurn->tilePos.x + j, characterTurn->tilePos.y - i);
				if (combatMap[nexTile.x + 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x - 1][nexTile.y].inRange == false &&
					combatMap[nexTile.x][nexTile.y + 1].inRange == false &&
					combatMap[nexTile.x][nexTile.y - 1].inRange == false ||
					app->pathfinding->IsWalkable(nexTile)==false) {

					combatMap[nexTile.x][nexTile.y].inRange = false;
					/* arealist.Add(&combatMap[nexTile.x ][nexTile.y]);*/
				}

			}

		}


		
		

	}
	
	if(move==false)
	{
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 9; j++) {

				combatMap[i][j].characterType = nullptr;
				combatMap[i][j].character = false;
				combatMap[i][j].inRange = false;
				combatMap[i][j].enemy = false;
				
				
			}
		}

		/*DestroyListArea(arealist);*/
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {
			if (combatMap[i][j].inRange==true && combatMap[i][j].character == false && atack==false) {
				iPoint pos = iPoint(i, j);
				if (app->pathfinding->IsWalkable(pos)) {
					pos = app->map->MapToWorld(pos.x, pos.y);
				   app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
				}
			}

		}

	}

	if (moveenemy == true) {
		if (characterTurn->stamina >= 5) {
			ListItem<Entity*>* entitylist;
			entitylist = targets.start;
			if (entitylist != NULL) {




				/*Combat(characterTurn, targets, 1);*/


				targets.At(0)->data->health = targets.At(0)->data->health - (characterTurn->attack - targets.At(0)->data->defense);

				targets.Clear();
				characterTurn->UseStamina(5);
				turnstart = false;
				atack = false;
				moveenemy = false;



			}
		}
		

		if (moveenemy == true && characterTurn->stamina>=3) {

			moveanim = true;
			move = true;
			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 9; j++) {

					if(moveenemy==true){
						if (combatMap[i][j].inRange == true && combatMap[i][j].character == false && atack == false) {
							iPoint pos = iPoint(i, j);

							if (app->pathfinding->IsWalkable(pos)) {
								CreateArea(characterTurn->AttArea, 1, pos);
								for (int i = 0; i < area.Count(); i++) {

									if (area.At(i)->data->character == true) {

										length = app->pathfinding->CreatePath(origin, pos);
										destination.x = pos.x;
										destination.y = pos.y;
										originSelected = false;
										moveenemy = false;
										characterTurn->UseStamina(3);
										i = area.Count();

									}

								}
							}
						}
						/*	if (app->pathfinding->IsWalkable(pos)) {
								pos = app->map->MapToWorld(pos.x, pos.y);
								app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
							}*/
					}

				}

			}

		}
		else if(moveenemy == true) {

			characterTurn->stamina += 10;
		}


	}

	std::cout << "Stamina Timmy: " << timmy->stamina << std::endl;

	std::cout << "Stamina Bunny: " << bunny->stamina << std::endl;

	std::cout << "Stamina Villager: " << villager->stamina << std::endl;

	//std::cout << "Vida Timmy: " << timmy->health<< std::endl;
	//std::cout << "Vida Bunny: " << bunny->health << std::endl;
	//std::cout << "Vida Villager: " << villager->health << std::endl;
	//std::cout << "Atakk Villager: " << villager->attack << std::endl;

	combatMap[villager->tilePos.x][villager->tilePos.y].enemy = true;
	combatMap[villager->tilePos.x][villager->tilePos.y].characterType = villager;

	app->render->DrawRectangle({ int(villager->position.x) + 35, int(villager->position.y) + 35, 50, 50 }, 255, 233, 0, 250, true);

	combatMap[bunny->tilePos.x][bunny->tilePos.y].character = true;
	combatMap[bunny->tilePos.x][bunny->tilePos.y].characterType = bunny;

	

	
	combatMap[timmy->tilePos.x][timmy->tilePos.y ].character = true;
	combatMap[timmy->tilePos.x][timmy->tilePos.y].characterType = timmy;
	
	



	
	if (characterTurn->id == 1) {
		bunny->currentAnimation = &bunny->idleAnim;
		SDL_Rect recta = bunny->currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &recta);
		SDL_Rect recti = villager->currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &recti);
		if (moveanim == false) {
			timmy->currentAnimation = &timmy->idleAnim;
			SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
		}
		if (moveanim == true) {
			if (xDir == 1) {
				timmy->currentAnimation = &timmy->walkRightAnim;
				SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
				timmy->currentAnimation->Update();
			}
			if (xDir == -1) {
				timmy->currentAnimation = &timmy->walkLeftAnim;
				SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
				timmy->currentAnimation->Update();
			}
			if (yDir == 1) {
				timmy->currentAnimation = &timmy->walkDownAnim;
				SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
				timmy->currentAnimation->Update();
			}
			if (yDir == -1) {
				timmy->currentAnimation = &timmy->walkUpAnim;
				SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
				timmy->currentAnimation->Update();
			}
			if (xDir == 0 || yDir == 0) {
				SDL_Rect rect = timmy->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &rect);
				timmy->currentAnimation->Update();
			}
		}

	}
	if (characterTurn->id == 2) {

		timmy->currentAnimation = &timmy->idleAnim;
		SDL_Rect recta = timmy->currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &recta);
		villager->currentAnimation = &villager->idleAnim;
		SDL_Rect recti = villager->currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &recti);
		if (moveanim == false) {
			bunny->currentAnimation = &bunny->idleAnim;
			SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
		}
		if (moveanim == true) {
			if (xDir == 1) {
				bunny->currentAnimation = &bunny->walkRightAnim;
				SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
				bunny->currentAnimation->Update();
			}
			if (xDir == -1) {
				bunny->currentAnimation = &bunny->walkLeftAnim;
				SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
				bunny->currentAnimation->Update();
			}
			if (yDir == 1) {
				bunny->currentAnimation = &bunny->walkDownAnim;
				SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
				bunny->currentAnimation->Update();
			}
			if (yDir == -1) {
				bunny->currentAnimation = &bunny->walkUpAnim;
				SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
				bunny->currentAnimation->Update();
			}
			if (xDir == 0 || yDir == 0) {
				SDL_Rect rect = bunny->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &rect);
				bunny->currentAnimation->Update();
			}
		}

	}
	
	if (characterTurn->id == 3) {

		timmy->currentAnimation = &timmy->idleAnim;
		SDL_Rect recti = timmy->currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(timmytexture, timmy->position.x - 13, timmy->position.y - 35, &recti);

		bunny->currentAnimation = &bunny->idleAnim;
		SDL_Rect recta = bunny->currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(bunnytexture, bunny->position.x - 13, bunny->position.y - 35, &recta);

		if (moveanim == false) {
			villager->currentAnimation = &villager->idleAnim;
			SDL_Rect rect = villager->currentAnimation->GetCurrentFrame();
			app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &rect);
		}
		if (moveanim == true) {
			if (xDir == 1) {
				villager->currentAnimation = &villager->walkRightAnim;
				SDL_Rect rect = villager->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &rect);
				villager->currentAnimation->Update();
			}
			if (xDir == -1) {
				villager->currentAnimation = &villager->walkLeftAnim;
				SDL_Rect rect = villager->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &rect);
				villager->currentAnimation->Update();
			}
			if (yDir == 1) {
				villager->currentAnimation = &villager->walkDownAnim;
				SDL_Rect rect = villager->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &rect);
				villager->currentAnimation->Update();
			}
			if (yDir == -1) {
				villager->currentAnimation = &villager->walkUpAnim;
				SDL_Rect rect = villager->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &rect);
				villager->currentAnimation->Update();
			}
			if (xDir == 0 || yDir == 0) {
				SDL_Rect rect = villager->currentAnimation->GetCurrentFrame();
				app->render->DrawTexture(villagertexture, villager->position.x - 13, villager->position.y - 35, &rect);
				villager->currentAnimation->Update();
			}
		}
	}
	
	return ret;
}

bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	default:
		break;
	}
	return true;
}

bool SceneBattle::ChekRangeEnemy() {
	for (int i = 0; i < targets.Count(); i++) {

		for (int i = 0; i < area.Count(); i++) {
			if (targets.At(i)->data->tilePos == iPoint(area.At(i)->data->x, area.At(i)->data->y)) {


				return true;
			}
		}
	}
	return false;
}

bool SceneBattle::MoveEnemy() {


	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {
			if (combatMap[i][j].character == true ) {
				iPoint pos = iPoint(i, j);
				if (app->pathfinding->IsWalkable(pos)) {

					pos = app->map->MapToWorld(pos.x, pos.y);
					app->render->DrawRectangle({ pos.x, pos.y, 120, 120 }, 0, 143, 57, 100, true);
					moveanim = true;
				}
			}

		}

	}

	return true;

}
bool SceneBattle::Move( int pathindex,int length) {

	iPoint dist;
	fPoint pixelPosition;
	fPoint finalPosition;
	float distance;
	b2Vec2 vel = b2Vec2(0, 0);

	const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();


	pixelPosition.x = lastpath->At(pathIndex)->x * app->map->mapData.tileWidth;
	pixelPosition.y = lastpath->At(pathIndex)->y * app->map->mapData.tileHeight;

	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
	LOG(" NextposX: %d", nextpos.x);
	LOG(" NextposY: %d", nextpos.y);

	dist.x = pixelPosition.x - characterTurn->position.x;
	LOG(" disX: %d", dist.x);
	dist.y = pixelPosition.y - characterTurn->position.y;
	LOG(" disY: %d", dist.y);


	xDir = 0;
	yDir = 0;
	xDir = (dist.x > 0) ? 1 : -1;
	yDir = (dist.y > 0) ? 1 : -1;
	if (dist.x == 0) {
		xDir = 0;
	}
	if (dist.y == 0) {
		yDir = 0;
	}
	if (dist.x == 0 && dist.y == 0) {
		pathIndex++;
		
	}
	else if (abs(dist.x) > 0) {
		vel.x = 5 * xDir;

	}
	else if (abs(dist.y) > 0) {
		vel.y = 5 * yDir;

	}

	characterTurn->position.x = characterTurn->position.x + vel.x;
	characterTurn->position.y = characterTurn->position.y + vel.y;

	return true;
}

// Loads combat map from Map module using GID tile metadata
bool SceneBattle::MakeCombatMap() {
	
	bool ret = true;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].character = false;
			combatMap[i][j].enemy = false;
			combatMap[i][j].characterType = nullptr;
			combatMap[i][j].inRange = false;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	return ret;
}

bool SceneBattle:: GetTargets(){


	ListItem<TileDataa*>* tileListItem;
	tileListItem = area.start;


	while (tileListItem != NULL) {


		iPoint pos = iPoint(tileListItem->data->x, tileListItem->data->y);
		
		if (combatMap[pos.x][pos.y].enemy == true && characterTurn->isEnemy==false) {

			targets.Add(combatMap[pos.x][pos.y].characterType);
			
			std::cout << "posicion: " << pos.x << "x "<< pos.y <<"y" << std::endl;

		}
		else if (combatMap[pos.x][pos.y].character == true && characterTurn->isEnemy == true) {

			targets.Add(combatMap[pos.x][pos.y].characterType);

			std::cout << "posicion: " << pos.x << "x " << pos.y << "y" << std::endl;

		}

		tileListItem = tileListItem->next;
	}

	return true;
}

bool SceneBattle::DisplayEnemys() {



	ListItem<Entity*>* entitylist;
	entitylist = targets.start;

	while (entitylist != NULL) {
		iPoint pos = iPoint(entitylist->data->position.x, entitylist->data->position.y);

		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 250, 0, 0, 100);

		entitylist = entitylist->next;
	}


	return true;
}

bool SceneBattle::GetTurns() {
	if (allentities.At(0)->data->speed >= allentities.At(1)->data->speed && allentities.At(0)->data->speed >= allentities.At(2)->data->speed)
	{
		characterTurn = allentities.At(0)->data;
		turnqueue.Add(allentities.At(0)->data);

		if (allentities.At(1)->data->speed >= allentities.At(2)->data->speed)
		{
			turnqueue.Add(allentities.At(1)->data);
			turnqueue.Add(allentities.At(2)->data);
		}
		else 
		{
			turnqueue.Add(allentities.At(2)->data);
			turnqueue.Add(allentities.At(1)->data);
		}
	}
	else if (allentities.At(1)->data->speed > allentities.At(0)->data->speed && allentities.At(1)->data->speed >= allentities.At(2)->data->speed)
	{
		characterTurn = allentities.At(1)->data;
		turnqueue.Add(allentities.At(1)->data);

		if (allentities.At(0)->data->speed >= allentities.At(2)->data->speed)
		{
			turnqueue.Add(allentities.At(0)->data);
			turnqueue.Add(allentities.At(2)->data);
		}
		else
		{
			turnqueue.Add(allentities.At(2)->data);
			turnqueue.Add(allentities.At(0)->data);
		}
	}
	else if (allentities.At(2)->data->speed > allentities.At(0)->data->speed && allentities.At(2)->data->speed > allentities.At(1)->data->speed)
	{
		characterTurn = allentities.At(2)->data;
		turnqueue.Add(allentities.At(2)->data);

		if (allentities.At(0)->data->speed >= allentities.At(1)->data->speed)
		{
			turnqueue.Add(allentities.At(0)->data);
			turnqueue.Add(allentities.At(1)->data);
		}
		else {
			turnqueue.Add(allentities.At(1)->data);
			turnqueue.Add(allentities.At(0)->data);

		}


	}


	return true;
}

bool SceneBattle::GetNext() {

	Entity* temp;
	temp = turnqueue.At(0)->data;
	turnqueue.At(0)->data = turnqueue.At(1)->data;
	turnqueue.At(1)->data = turnqueue.At(2)->data;
	turnqueue.At(2)->data = temp;
	characterTurn = turnqueue.At(0)->data;
	return true;

}
bool SceneBattle::CreateArea(int range, int type, iPoint posTile) {

	
	
	
	switch (type) {

	case 0:
		//attack
		if (combatMap[posTile.x + 1][posTile.y].type == TILE_TYPE::FLOOR) {
			
			area.Add(&combatMap[posTile.x + 1][posTile.y]);
		}
		if (combatMap[posTile.x - 1][posTile.y].type == TILE_TYPE::FLOOR) {
			area.Add(&combatMap[posTile.x - 1][posTile.y]);
		}
		if (combatMap[posTile.x][posTile.y + 1].type == TILE_TYPE::FLOOR) {
			area.Add(&combatMap[posTile.x][posTile.y + 1]);
		}
		if (combatMap[posTile.x][posTile.y-1].type == TILE_TYPE::FLOOR) {
			area.Add(&combatMap[posTile.x][posTile.y - 1]);
		}
		break;
	case 1:
		//lineal
		for (int i = 1; i <= range; i++) {
			if (combatMap[posTile.x + i][posTile.y].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x + i][posTile.y]);
			}
			if (combatMap[posTile.x - i][posTile.y].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x - i][posTile.y]);
			}
			if (combatMap[posTile.x][posTile.y + i].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x][posTile.y + i]);
			}
			if (combatMap[posTile.x][posTile.y - i].type == TILE_TYPE::FLOOR) {
				area.Add(&combatMap[posTile.x][posTile.y - i]);
			}
		}
		break;
	case 2:
		//circular
		int i;
		int j;
		for (i = 0; i <=range; i++) {
			for (j = 0; j <= range - i; j++) {
				if (combatMap[posTile.x+j][posTile.y +i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x + j][posTile.y + i]);
				}
				if (combatMap[posTile.x - j][posTile.y + i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x - j][posTile.y + i]);
				}
				if (combatMap[posTile.x - j][posTile.y - i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x - j][posTile.y - i]);
				}
				if (combatMap[posTile.x + j][posTile.y - i].type == TILE_TYPE::FLOOR) {
					area.Add(&combatMap[posTile.x + j][posTile.y - i]);
				}
			}

		}

		break;
	case 3:
		//global
		for (int i = 0; i < COMBAT_MAP_HEIGHT; i++) {
			for (int j = 0; j < COMBAT_MAP_WIDTH; j++) {
				iPoint pos= iPoint(i,j);
				if (combatMap[j][i].type == TILE_TYPE::FLOOR) {
				
					area.Add(&combatMap[j][i]);

				}
				
			}
		}
		break;
	}

	return true;

}

bool SceneBattle::DisplayArea(int type) {

	bool ret = true;

	ListItem<TileDataa*>*tileListItem;
	tileListItem = area.start;

	uint color[3];

	switch (type)
	{
	case 0:
		color[0] = 255;
		color[1] = 0;
		color[2] = 0;
		break;
	case 1:
		color[0] = 0;
		color[1] = 255;
		color[2] = 0;
		break;
	case 2:
		color[0] = 0;
		color[1] = 0;
		color[2] = 255;
		break;
	default:
		break;
	}

	while (tileListItem != NULL) {

		
		iPoint pos = app->map->MapToWorld(tileListItem->data->x, tileListItem->data->y);
		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 0, 0, 250, 100);

		tileListItem = tileListItem->next;
	}

	return ret;
}

// Starts combat, id=1 --> attack, id=2 --> ability 1, id=3 --> ability 2
bool SceneBattle::Combat(Entity* inturn, List<Entity*> target, int id) {
	
	bool ret = true;

	//id = 1 --> attack
	if (id == 1) {
		for (int i = 0;  i < target.Count(); i++) {
			target.At(i)->data->TakeDamage(inturn->Attack());
		}
	}
	//id = 2 --> ability 1
	if (id == 2) {
		for (int i = 0; i < target.Count(); i++) {
			if (inturn->Ab1Type != 3) {
				target.At(i)->data->TakeDamage(inturn->Ability(1));
			}
			if (inturn->Ab1Type == 3) {
				target.At(i)->data->TakeHealing(inturn->Ability(1));
			}
		}
	}
	//id = 3 --> ability 2
	if (id == 3) {
		for (int i = 0; i < target.Count(); i++) {
			if (inturn->Ab1Type != 3) {
				target.At(i)->data->TakeDamage(inturn->Ability(2));
			}
			if (inturn->Ab1Type == 3) {
				target.At(i)->data->TakeHealing(inturn->Ability(2));
			}
		}
	}
	return ret;
}
void SceneBattle::DestroyListArea()
{
	//ListItem<TileDataa*>* item;
	//int i = 0;
	//for (item = area.start; item != NULL; item = item->next)
	//{
	//	if (item->data ==area.At(i)->data) area.Del(item);
	//	i++;
	//}

	area.Clear();
}
//Called before quitting
bool SceneBattle::CleanUp()
{
	LOG("Freeing sceneBattle");
	allentities.Clear();
	area.Clear();
	turnqueue.Clear();
	targets.Clear();
	
	app->map->CleanUp();
	app->entityManager->CleanUp(); 
	return true;
}
