#include "BattleManager.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiButton.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "UIModule.h"
#include "Fonts.h"
#include "Defs.h"
#include "Physics.h"
#include "Log.h"
#include"Enemy_AngryVillager.h"
#include "SceneBattle.h"
#include "SceneCombatLHHR.h"
#include"BattleScene_Pigs.h"
#include "BattleScene_Wolf.h"


BattleManager::BattleManager(bool isActive) : Module(isActive) {

	name.Create("battleManager");
}

// Destructor
BattleManager::~BattleManager() {}

bool BattleManager::Awake(pugi::xml_node& config) {

	enemyAreaTimer.Start(0.5f);
	enemyAttackTimer.Start(0.3f);


	return true;
}

bool BattleManager::Start() {

	moveFx = app->audio->LoadFx("Assets/Sounds/FX/fx_wooden_walk.wav");
	deathFx = app->audio->LoadFx("Assets/Sounds/FX/fx_death.wav");
	looseFx = app->audio->LoadFx("Assets/Sounds/FX/fx_loose.wav");
	victoryFx = app->audio->LoadFx("Assets/Sounds/FX/fx_victory.wav");
	rechargemanaFx = app->audio->LoadFx("Assets/Sounds/FX/fx_recharge_mana.wav");
	winScreen = app->tex->Load("Assets/UI/Win_screen.png");
	loseScreen = app->tex->Load("Assets/UI/lose_screen.png");

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = COMBAT;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	MakeTurnList();

	
	currentTurn = turnList.start->data;
	origin = currentTurn->tilePos;
	pathIndex = 1;
	triggerMoveTimer = false;
	win = false;
	lose = false;
	finish = false;
	timercharge = 0;
	battleState = BattleState::THINKING;
	app->SaveGameRequest();

	lose_screen_animation.Set();
	lose_screen_animation.smoothness = 4;
	lose_screen_animation.AddTween(100, 50, EXPONENTIAL_OUT);

	win_screen_animation.Set();
	win_screen_animation.smoothness = 4;
	win_screen_animation.AddTween(100, 50, EXPONENTIAL_OUT);

	app->uiModule->menu_pause = false;

	mouseSpeed = 15; 

	return true;
}

bool BattleManager::PreUpdate() {

	MousePosition();


	return true;
}

bool BattleManager::Update(float dt) {

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN) {
		app->uiModule->pausecombat_menu_animation_bool = true;
		PauseMenuAppear();
	}
		

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
	}

	if (godMode) {
		GodMode();
	}
	if (timercharge > 0) {
		timercharge--;
	}

	app->input->GetMousePosition(mouseX, mouseY);
	app->input->HandlePadMouse(mouseX, mouseY, mouseSpeed);

	UpdateEntitiesTilePos();

	switch (battleState)
	{
	case BattleState::UNKNOWN:
		break;
	case BattleState::THINKING:
		actionfinish = false;
		timercharge = 0;
		origin = currentTurn->tilePos;
		targets.Clear();
		actionArea.Clear();
		area.Clear();
		enemyAreaTimer.Start(1.0f);

		if (currentTurn->isEnemy)
		{

			enemyAttackTimer.Start(0.9f);
			displayEnemyAttackAreaOnce = false;
			battleState = BattleState::ENEMY;
			actionType = ActionType::UNKNOWN;

		}
		break;
	case BattleState::SELCETED:


		actionArea.Clear();
		targets.Clear();
		GetActionArea(currentTurn, actionType);

		if (!currentTurn->isEnemy) {

			if (actionType == ActionType::MOVE) {

				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
				{
					for (ListItem<TileData*>* area = actionArea.start; area != NULL; area = area->next) {
						iPoint pos = iPoint(area->data->x, area->data->y);
						if (app->pathfinding->IsWalkable(origin) && mouseTile == pos && combatMap[mouseTile.x][mouseTile.y].character == nullptr && actionfinish==false) {
							length = app->pathfinding->CreatePath(origin, mouseTile);
							destination.x = mouseTile.x;
							destination.y = mouseTile.y;
							moveanim = true;
							currentTurn->UseStamina(3);
							actionfinish = true;
							battleState = BattleState::INACTION;
						}
					}
				}
			}
			else if (actionType == ActionType::END_TURN) {

				currentTurn->GainStamina(10);
				app->audio->PlayFx(rechargemanaFx);
				timercharge = 50;
				battleState = BattleState::INACTION;

			}
			else {


				SelectTargets();

				for (int i = 0; i < targets.Count(); i++) {


					if (targets.At(i)->data->tilePos == mouseTile && actionfinish == false) {
						if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN) {



							app->sceneBattle->TakeDamageAnimation(targets.At(i)->data->name.GetString());
							if (actionType == ActionType::ABILITY) {
								app->audio->PlayFx(currentTurn->abilityFx);
								targets.At(i)->data->health = targets.At(i)->data->health - (currentTurn->Ab1Power - targets.At(i)->data->defense);
								//audio ability
								currentTurn->UseStamina(10);
							}
							if (actionType == ActionType::ATTACK) {
								app->audio->PlayFx(currentTurn->attackFx);
								targets.At(i)->data->health = targets.At(i)->data->health - (currentTurn->attack - targets.At(i)->data->defense);
								//adio attack
								currentTurn->UseStamina(5);
							}
							targetPosForAnimation = targets.At(i)->data->position;

							actionfinish = true;
							battleState = BattleState::INACTION;

						}
					}
				}
			}
		}


		break;
	case BattleState::INACTION:

		if (actionType == ActionType::MOVE) {

			if (pathIndex != length) {
				Move(pathIndex, length);
			}
			else
			{
				if (currentTurn->tilePos == destination) {
					triggerMoveTimer = false;

					destination = iPoint(0, 0);
					length = 1;
					pathIndex = 1;
					app->pathfinding->ClearLastPath();
					battleState = BattleState::THINKING;
					UpdateTurnList();
				}
			}

		}
		else if (timercharge == 0 && finish==false) {
		
			battleState = BattleState::THINKING;
			UpdateTurnList();

		}

		break;
	case BattleState::ENEMY:

		////Miramos si tiene que atacar o tiene que moverse
		//app->sceneBattle->conditionToRangeChecker = IaEnemyAttack();

		////Miramos si tiene suficiente stamina para que se mueva
		//app->sceneBattle->noStaminaToMove = IaEnemyMove();

		//if (enemyTimer > 300) {

		//	//Se Ejecuta el arbol
		//	app->sceneBattle->RunTree();
		//	enemyTimer = 0;
		//	battleState = BattleState::INACTION;
		//}
		//else {
		//	enemyTimer++;
		//}




		if (currentTurn->stamina >= 5 && battleState == BattleState::ENEMY)
		{

			if (!displayEnemyAttackAreaOnce)
			{
				GetActionArea(currentTurn, ActionType::ATTACK);
				SelectTargets();
			}

			ListItem<Entity*>* entitylist;
			entitylist = targets.start;

			while (entitylist != NULL)
			{
				if (entitylist->data->isAlive == true)
				{
					actionType = ActionType::ATTACK;
					displayEnemyAttackAreaOnce = true;
					if (enemyAttackTimer.Test() == FIN) {
						entitylist->data->health = entitylist->data->health - (currentTurn->attack - entitylist->data->defense);
						app->sceneBattle->TakeDamageAnimation(targets.start->data->name.GetString());
						targetPosForAIAnimation = entitylist->data->position;
						targets.Clear();
						currentTurn->UseStamina(5);
						entitylist = NULL;
						app->audio->PlayFx(currentTurn->abilityFx);
						battleState = BattleState::INACTION;
						actionfinish = true;

						break;
					}
				}

				entitylist = entitylist->next;

			}

		}
		if (currentTurn->stamina >= 3 && battleState == BattleState::ENEMY && actionType != ActionType::ATTACK) {

			targets.Clear();
			actionArea.Clear();

			/*		iPoint pos = iPoint(currentTurn->tilePos.x, currentTurn->tilePos.y+1);
					length = app->path  finding->CreatePath(origin, pos);
					destination.x = pos.x;
					destination.y = pos.y;
					currentTurn->UseStamina(3);
					actionType = ActionType::MOVE;
					battleState = BattleState::INACTION;*/

			GetActionArea(currentTurn, ActionType::MOVE);

			if (battleState == BattleState::ENEMY) {
				ListItem<TileData*>* tiledata;
				for (tiledata = actionArea.start; tiledata != NULL; tiledata = tiledata->next) {

					if (battleState == BattleState::ENEMY && tiledata->data->character == NULL) {

						iPoint pos = iPoint(tiledata->data->x, tiledata->data->y);
						CreateArea(currentTurn->AttArea, 1, pos);
						ListItem<TileData*>* tileListItem;

						if (battleState == BattleState::ENEMY) {
							for (tileListItem = area.start; tileListItem != NULL; tileListItem = tileListItem->next) {

								if (tileListItem->data->character != NULL && tileListItem->data->character->isEnemy == false && actionfinish == false) {

									length = app->pathfinding->CreatePath(origin, pos);
									destination.x = pos.x;
									destination.y = pos.y;
									currentTurn->UseStamina(3);
									actionfinish = true;
									actionType = ActionType::MOVE;
									battleState = BattleState::INACTION;

								}

							}
						}

					}

				}
			}



		}
		else if (currentTurn->stamina >= 3 && battleState == BattleState::ENEMY && actionType != ActionType::ATTACK && actionfinish == false) {

			targets.Clear();
			actionArea.Clear();

			GetActionArea(currentTurn, ActionType::MOVE);

			iPoint pos = iPoint(currentTurn->tilePos.x - 3, currentTurn->tilePos.y);
			length = app->pathfinding->CreatePath(origin, pos);
			destination.x = pos.x;
			destination.y = pos.y;
			currentTurn->UseStamina(3);
			actionfinish = true;
			actionType = ActionType::MOVE;
			battleState = BattleState::INACTION;


		}

		
		if (battleState == BattleState::ENEMY && actionType != ActionType::ATTACK) {
			currentTurn->GainStamina(10);
			timercharge = 100;
			battleState = BattleState::INACTION;
		}

		break;

	case BattleState::WIN:
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN) {
			app->teamManager->arasiva = true;
			if (app->sceneManager->scene == GameScene::BATTLE || app->sceneManager->scene == GameScene::COMBATLHHR) {
				app->sceneManager->LoadScene(GameScene::SCENE);
			}
			else if(app->sceneManager->scene == GameScene::COMBATOINK){
				app->sceneManager->LoadScene(GameScene::W2_SCENE);
			}
			else if (app->sceneManager->scene == GameScene::COMBATWOLF) {
				app->sceneManager->LoadScene(GameScene::W3_SCENE);
			}

		}

		break;
	case BattleState::LOSE:
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN) {
			if (app->sceneManager->scene == GameScene::BATTLE || app->sceneManager->scene == GameScene::COMBATLHHR) {
				app->sceneManager->LoadScene(GameScene::SCENE);
			}
			else if (app->sceneManager->scene == GameScene::COMBATOINK) {
				app->sceneManager->LoadScene(GameScene::W2_SCENE);
			}
			else if (app->sceneManager->scene == GameScene::COMBATWOLF) {
				app->sceneManager->LoadScene(GameScene::W3_SCENE);
			}
		}
		break;
	default:
		break;
	}

	if (battleState != BattleState::WIN && battleState != BattleState::LOSE) {
		CheckWinCondition();
	}
	UpdateCombatMap();

	if (!app->sceneBattle->active) {
		app->scene->battleTutorialCounter = 3;
	}

	return true;
}

bool BattleManager::PostUpdate() {
	std::cout << "Y: " << app->uiModule->offset2 + app->uiModule->point2 * (-app->render->camera.y - app->uiModule->offset2) << std::endl;

	if ((-(app->uiModule->offset2 + app->uiModule->point2 * (app->uiModule->offset2))) >= 1080 && app->uiModule->menu_pause_combat) {
		if (app->uiModule->currentMenuType == COMBAT_PAUSE) {
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
			app->uiModule->menu_pause_combat = false;
		}

	}

	if (battleState == BattleState::SELCETED /*|| battleState == BattleState::ENEMY*/ || enemyAreaTimer.Test()==EJECUTANDO || enemyAttackTimer.Test() == EJECUTANDO) {

		DisplayAtackArea(actionType);
		DisplayEnemys();	

	}

	for (ListItem<Entity*>* entiyItem = enemies.start; entiyItem != NULL; entiyItem = entiyItem->next){
		if(app->sceneBattle->active)app->sceneBattle->UpdateAnimation(entiyItem->data->name.GetString());
		if(app->sceneCombatLHHR->active)app->sceneCombatLHHR->UpdateAnimation(entiyItem->data->name.GetString());
		if (app->battleScene_Pigs->active)app->battleScene_Pigs->UpdateAnimation(entiyItem->data->name.GetString());
		if (app->battleScene_Wolf->active)app->battleScene_Wolf->UpdateAnimation(entiyItem->data->name.GetString());
		
    }
	
	//Moved to UI Module in PostUpdate()
	//UIStatsForBattle();

	//DisplayTurnList();



	//app->render->DrawRectangle({ int(allies.start->data->position.x) + 35, int(allies.start->data->position.y) + 35, 50, 50 }, 0, 233, 0, 250, true);
	//app->render->DrawRectangle({ int(enemies.start->data->position.x) + 35, int(enemies.start->data->position.y) + 35, 50, 50 }, 255, 233, 0, 250, true);
	return true;
}

void BattleManager::DrawResult() {

	if (win) {
		win_screen_animation.Step(1, false);
		win_screen_animation.Foward();

		int offset = 720;
		float point = win_screen_animation.GetPoint();

		app->render->DrawTexture(winScreen, 0, -(offset + point * (0 - offset)));

		//app->render->DrawTexture(winScreen, 0, 0);
	}
	if (lose) {
		lose_screen_animation.Step(1, false);
		lose_screen_animation.Foward();

		int offset = 720;
		float point = lose_screen_animation.GetPoint();

		app->render->DrawTexture(loseScreen, 0, offset + point * (0 - offset));
		//app->render->DrawTexture(loseScreen, 0, 0);
	}
}

bool BattleManager::CleanUp() {

	turnList.Clear();
	allies.Clear();
	enemies.Clear();
	area.Clear();
	actionArea.Clear();

	app->tex->UnLoad(winScreen); 
	app->tex->UnLoad(loseScreen);
	return true;
}

Entity* BattleManager::GetCurrentTurn() { return currentTurn;}



bool BattleManager::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	if (battleState != BattleState::INACTION && !currentTurn->isEnemy && app->scene->battleTutorialCounter==3) {
		switch (control->id)
		{
			// Attack
		case 16:
			if (currentTurn->stamina >= 5) {
				app->battleManager->buttonPressed = CombatButtons::ATTACK;
				app->battleManager->battleState = BattleState::SELCETED;
				app->battleManager->actionType = ActionType::ATTACK;
			}
			break;
			// Ability
		case 17:
			if (currentTurn->stamina >= 10) {
				app->battleManager->buttonPressed = CombatButtons::ABILITY;
				app->battleManager->battleState = BattleState::SELCETED;
				app->battleManager->actionType = ActionType::ABILITY;
			}
			break;
			// Move
		case 18:
			if (currentTurn->stamina >= 3) {
				app->battleManager->buttonPressed = CombatButtons::MOVE;
				app->battleManager->battleState = BattleState::SELCETED;
				app->battleManager->actionType = ActionType::MOVE;
			}
			break;

			// End turn
		case 19:
			app->battleManager->buttonPressed = CombatButtons::END;
			app->battleManager->battleState = BattleState::THINKING;
			app->battleManager->actionType = ActionType::END_TURN;
			app->battleManager->battleState = BattleState::SELCETED;


			break;
		}

		
	}
	return true;

}

void BattleManager::UIStatsForBattle()
{
	int i = 0;
	int j = 0;
	// UI Stats for Battle

	//app->fonts->DrawText("--- NEXT  TURN --- ", 1640, 900, 200, 200, { 255,255,255 }, app->fonts->gameFont);


	// Mini character photos
	SDL_Rect timmyPicRect = { 13, 16, 142, 111 };
	SDL_Rect bunnyPicRect = { 158, 16, 142, 111 };
	SDL_Rect villagerPicRect = { 303, 16, 142, 111 };
	SDL_Rect corrPicLRRHRect = { 450, 16, 142, 111 };
	SDL_Rect LRRHPicRect = { 595, 16, 142, 111 };
	SDL_Rect corrLPigPicRect = { 13, 142, 142, 111 };
	SDL_Rect corrMPigPicRect = { 158, 142, 142, 111 };
	SDL_Rect LPigPicRect = { 303, 142, 142, 111 };
	SDL_Rect MPigPicRect = { 450, 142, 142, 111 };
	SDL_Rect corrWolfPicRect = { 595, 142, 142, 111 };
	SDL_Rect corrSproutRect = { 12, 267, 142, 111 };


	// Next Turn Name
	if (turnList.Count() > 1) {
		
	/*	int x = 1;

			while (turnList.At(x)==nullptr && x <=turnList.Count())
			{
				x++;
			}*/
		//if (turnList.At(1) != nullptr)
		//{
		//	app->fonts->DrawText(turnList.At(1)->data->namechar.GetString(), 1400, 950, 200, 200, { 255,0,255 }, app->fonts->gameFont);
		//}
		//if (turnList.At(2) != nullptr)
		//{
		//	app->fonts->DrawText(turnList.At(2)->data->namechar.GetString(), 1400, 980, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		//}

		if (turnList.At(0) != nullptr)
		{
			//Current Trun
			if (turnList.At(0)->data->name == "timmy")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &timmyPicRect);
			}
			else if (turnList.At(0)->data->name == "bunny")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &bunnyPicRect);
			}
			else if (turnList.At(0)->data->name == "enemy_angryVillager")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &villagerPicRect);
			}
			else if (turnList.At(0)->data->name == "enemy_lrrh")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &corrPicLRRHRect);
			}
			else if (turnList.At(0)->data->name == "lrrh")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &LRRHPicRect);
			}
			else if (turnList.At(0)->data->name == "sprout")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &corrSproutRect);
			}
			else if (turnList.At(0)->data->name == "enemy_littlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &corrLPigPicRect);
			}
			else if (turnList.At(0)->data->name == "enemy_middlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &corrMPigPicRect);
			}
			else if (turnList.At(0)->data->name == "littlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &LPigPicRect);
			}
			else if (turnList.At(0)->data->name == "middlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &MPigPicRect);
			}
			// CAL CANVIAR EL NOM QUAN ES TINGUI
			else if (turnList.At(0)->data->name == "enemy_wolf")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 315, 945, &corrWolfPicRect);
			}
		}

		// Next turn 1 photos
		if (turnList.At(1) != nullptr)
		{
			if (turnList.At(1)->data->name == "timmy")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &timmyPicRect);
			}
			else if (turnList.At(1)->data->name == "bunny")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &bunnyPicRect);
			}
			else if (turnList.At(1)->data->name == "enemy_angryVillager")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &villagerPicRect);
			}
			else if (turnList.At(1)->data->name == "enemy_lrrh")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &corrPicLRRHRect);
			}
			else if (turnList.At(1)->data->name == "lrrh")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &LRRHPicRect);
			}
			else if (turnList.At(1)->data->name == "sprout")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &corrSproutRect);
			}
			else if (turnList.At(1)->data->name == "enemy_littlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &corrLPigPicRect);
			}
			else if (turnList.At(1)->data->name == "enemy_middlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &corrMPigPicRect);
			}
			else if (turnList.At(1)->data->name == "littlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &LPigPicRect);
			}
			else if (turnList.At(1)->data->name == "middlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &MPigPicRect);
			}
			else if (turnList.At(1)->data->name == "enemy_wolf")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 660, &corrWolfPicRect);
			}

		}


		// TURN LIST SECOND
		if (turnList.At(2) != nullptr)
		{
			if (turnList.At(2)->data->name == "timmy")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &timmyPicRect);
			}
			else if (turnList.At(2)->data->name == "bunny")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &bunnyPicRect);
			}
			else if (turnList.At(2)->data->name == "enemy_angryVillager")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &villagerPicRect);
			}
			else if (turnList.At(2)->data->name == "enemy_lrrh")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &corrPicLRRHRect);
			}
			else if (turnList.At(2)->data->name == "sprout")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &corrSproutRect);
			}
			else if (turnList.At(2)->data->name == "lrrh")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &LRRHPicRect);
			}
			else if (turnList.At(2)->data->name == "enemy_littlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &corrLPigPicRect);
			}
			else if (turnList.At(2)->data->name == "enemy_middlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &corrMPigPicRect);
			}
			else if (turnList.At(2)->data->name == "littlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &LPigPicRect);
			}
			else if (turnList.At(2)->data->name == "middlePig")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &MPigPicRect);
			}
			else if (turnList.At(2)->data->name == "enemy_wolf")
			{
				app->render->DrawTexture(app->uiModule->smallCharPicTexture, 1735, 800, &corrWolfPicRect);
			}
		}

		// HOVER ATTACK BUTTON INFO DRAW:
		if (app->uiModule->combat_attack_button->state == GuiControlState::FOCUSED)
		{
			uint attackNum = turnList.At(0)->data->attack;
			std::string attackString = std::to_string(attackNum);
			const char* attackChar = attackString.c_str();

			app->fonts->DrawText(attackChar, 1500, 962, 100, 100, { 194, 168, 130 });

			// Stamina
			uint staminaNum = turnList.At(0)->data->stamina;
			std::string staminaString = std::to_string(staminaNum);
			const char* staminaChar = staminaString.c_str();
			app->fonts->DrawText(staminaChar, 1616, 962, 100, 100, { 194, 168, 130 });

			app->input->GetMousePosition(mouseX, mouseY);
			app->render->DrawTexture(app->uiModule->descriptionScrollTexture, mouseX - 185, mouseY - 185, NULL);
			app->fonts->DrawTextParagraph(turnList.At(0)->data->attackDescription, mouseX - 145, mouseY - 150, 300, 100, { 0, 0, 0 });

			if (turnList.At(0)->data->Ab1RangeType == 1)
			{
				// Lineal
				app->fonts->DrawText("Lineal", 1550, 1016, 100, 100, { 194, 168, 130 });
			}


		}

		// HOVER ABILITY BUTTON INFO DRAW:
		if (app->uiModule->combat_ability_button->state == GuiControlState::FOCUSED)
		{
			uint powerNum = turnList.At(0)->data->Ab1Power;
			std::string powerString = std::to_string(powerNum);
			const char* powerChar = powerString.c_str();

			app->fonts->DrawText(powerChar, 1500, 962, 100, 100, { 194, 168, 130 });

			// Stamina
			uint staminaNum = turnList.At(0)->data->stamina;
			std::string staminaString = std::to_string(staminaNum);
			const char* staminaChar = staminaString.c_str();
			app->fonts->DrawText(staminaChar, 1616, 962, 100, 100, { 194, 168, 130 });

			app->input->GetMousePosition(mouseX, mouseY);
			app->render->DrawTexture(app->uiModule->descriptionScrollTexture, mouseX - 185, mouseY - 185, NULL);

			app->fonts->DrawTextParagraph(turnList.At(0)->data->abilityDescription, mouseX - 145, mouseY - 150, 300, 100, { 0, 0, 0 });

			if (turnList.At(0)->data->Ab1RangeType == 1)
			{
				// Lineal
				app->fonts->DrawText("Lineal", 1550, 1016, 100, 100, { 194, 168, 130 });
			}
			else if (turnList.At(0)->data->Ab1RangeType == 2)
			{
				// Circular
				app->fonts->DrawText("Area", 1550, 1016, 100, 100, { 194, 168, 130 });
			}
		}

	}

	for (ListItem<Entity*>* allyItem = allies.start; allyItem != NULL; allyItem = allyItem->next) {


		// UI
		// Lateral Stats IU
		// Party UI
		app->render->DrawTexture(app->uiModule->combatStatsTexture, 0, 7 + i);

		uint stamina = allyItem->data->stamina;
		std::string staminaString = std::to_string(stamina);
		const char* staminaChar = staminaString.c_str();

		uint hp = allyItem->data->health;
		std::string hpString = std::to_string(hp);
		const char* hpChar = hpString.c_str();
	    
		const char* nameChar = allyItem->data->namechar.GetString();
		app->fonts->DrawText(nameChar, 25, 80 + i, 200, 200, {255,255,255}, app->fonts->gameFont);
		//app->fonts->DrawText("- HP: ", 80, 280 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		app->fonts->DrawText(hpChar, 70, 150 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		//app->fonts->DrawText("- Stamina: ", 80, 310 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		app->fonts->DrawText(staminaChar, 70, 225 + i, 200, 200, { 255,255,255 }, app->fonts->gameFont);


		i+= 270;
	}

	i = 0;
	for (ListItem<Entity*>* enemyItem = enemies.start; enemyItem != NULL; enemyItem = enemyItem->next) {

		// Lateral Stats
		// Enemy UI
		app->render->DrawTexture(app->uiModule->combatEnemyStatsTexture, 1755, 7 + i);

		uint stamina = enemyItem->data->stamina;
		std::string staminaString = std::to_string(stamina);
		const char* staminaChar = staminaString.c_str();

		uint hp = enemyItem->data->health;
		std::string hpString = std::to_string(hp);
		const char* hpChar = hpString.c_str();

		const char* nameChar = enemyItem->data->namechar.GetString();
		app->fonts->DrawText(nameChar, 1770, 80+i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		//app->fonts->DrawText("- HP: ", 1690+30, 230+i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		app->fonts->DrawText(hpChar, 1820, 150+i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		//app->fonts->DrawText("- Stamina: ", 1690+30, 260+i, 200, 200, { 255,255,255 }, app->fonts->gameFont);
		app->fonts->DrawText(staminaChar, 1820, 225+i, 200, 200, { 255,255,255 }, app->fonts->gameFont);



		i += 270;
	}
	

	
	
	
}

// Loads combat map from Map module using GID tile metadata
bool BattleManager::MakeCombatMap() {

	bool ret = true;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].inRange = false;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
			combatMap[i][j].type = combatMap[i][j].type;
		}
	}

	return ret;
}

void BattleManager::UpdateCombatMap() {

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].character= nullptr;
			combatMap[i][j].inRange = false;
			combatMap[i][j].isCharacter = false;
	
		}
	}

	for (ListItem<Entity*>* entity = turnList.start; entity != NULL; entity = entity->next) {

         combatMap[entity->data->tilePos.x][entity->data->tilePos.y].character = entity->data;
		 combatMap[entity->data->tilePos.x][entity->data->tilePos.y].isCharacter = true;
	}

}

void BattleManager::UpdateEntitiesTilePos()
{

	ListItem<Entity*>* entity;
	entity = turnList.start;
	while(entity!=NULL) {
		
		entity->data->tilePos = app->map->WorldToMap(entity->data->position.x - app->render->camera.x, entity->data->position.y - app->render->camera.y);
		
		entity = entity->next;
	}
}

bool BattleManager::GetActionArea(Entity* character, ActionType type) {

	iPoint nexTile;
	iPoint pos;
	switch (type) {
	case ActionType::MOVE:
		//actionArea = character.attackArea

		for (int i = 0; i < character->movement; i++) {
			for (int j = 0; j < character->movement - i; j++) {

				nexTile = iPoint(currentTurn->tilePos.x + j, currentTurn->tilePos.y + i);
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}
				nexTile = iPoint(currentTurn->tilePos.x - j, currentTurn->tilePos.y + i);
				
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}

				nexTile = iPoint(currentTurn->tilePos.x - j, currentTurn->tilePos.y - i);
			
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}

				nexTile = iPoint(currentTurn->tilePos.x + j, currentTurn->tilePos.y - i);
				
				if (combatMap[nexTile.x][nexTile.y].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[nexTile.x][nexTile.y]);
				}

			}

		}

		break;
	case ActionType::ATTACK:
		//actionArea = character.abiltyArea;
		for (int i = 1; i <= character->AttArea; i++) {
			if (combatMap[character->tilePos.x + i][character->tilePos.y].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x + i][character->tilePos.y]);
			}
			if (combatMap[character->tilePos.x - i][character->tilePos.y].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x - i][character->tilePos.y]);
			}
			if (combatMap[character->tilePos.x][character->tilePos.y + i].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x][character->tilePos.y + i]);
			}
			if (combatMap[character->tilePos.x][character->tilePos.y - i].type == TILE_TYPE::FLOOR) {
				actionArea.Add(&combatMap[character->tilePos.x][character->tilePos.y - i]);
			}
		}
		break;
	case ActionType::ABILITY:
		//actionArea = character.moveArea;


		for (int i = 0; i <= character->Ab1Area; i++) {
			for (int j = 0; j <= character->Ab1Area - i; j++) {
				if (combatMap[character->tilePos.x + j][character->tilePos.y + i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x + j][character->tilePos.y + i]);
				}
				if (combatMap[character->tilePos.x - j][character->tilePos.y + i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x - j][character->tilePos.y + i]);
				}
				if (combatMap[character->tilePos.x - j][character->tilePos.y - i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x - j][character->tilePos.y - i]);
				}
				if (combatMap[character->tilePos.x + j][character->tilePos.y - i].type == TILE_TYPE::FLOOR) {
					actionArea.Add(&combatMap[character->tilePos.x + j][character->tilePos.y - i]);
				}
			}

		}
		break;
	default:
		break;
	}

	return true;
}

bool BattleManager::MousePosition() {
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);


	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x, mouseY - app->render->camera.y);



	return true;
}

bool BattleManager::Move(int pathindex, int length) {

	iPoint dist;
	fPoint pixelPosition;
	fPoint finalPosition;
	float distance;
	b2Vec2 vel = b2Vec2(0, 0);

	const DynArray<iPoint>* lastpath = app->pathfinding->GetLastPath();

	if (enemyAreaTimer.Test() == FIN || !currentTurn->isEnemy)
	{
		triggerMoveTimer = true;
		enemyAttackTimer.Test();
	}
	
	if (triggerMoveTimer)
	{

		pixelPosition.x = lastpath->At(pathIndex)->x * app->map->mapData.tileWidth;
		pixelPosition.y = lastpath->At(pathIndex)->y * app->map->mapData.tileHeight;

		finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
		finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;


		dist.x = pixelPosition.x - currentTurn->position.x;
		dist.y = pixelPosition.y - currentTurn->position.y;


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

		currentTurn->position.x = currentTurn->position.x + vel.x;
		currentTurn->position.y = currentTurn->position.y + vel.y;
		app->audio->PlayFx(moveFx);
	}
	return true;
}

bool BattleManager::AddCharacter(Entity* character, int x, int y, bool isEnemy) {

	if (isEnemy) enemies.Add(character);
	else allies.Add(character);
	character->position = iPoint(x * 120, y * 120);
	character->position = character->position;
	combatMap[x][y].character = character;

	return true;
}

bool BattleManager::MakeTurnList() {

	List<Entity*> auxList;

	//Fill auxList
	for (ListItem<Entity*>* allyItem = allies.start; allyItem != NULL; allyItem = allyItem->next) {
		auxList.Add(allyItem->data);
	}
	for (ListItem<Entity*>* enemyItem = enemies.start; enemyItem != NULL; enemyItem = enemyItem->next) {
		auxList.Add(enemyItem->data);
	}

	//Sort auxList into turnList based on Entity->speed
	while (auxList.Count() > 0) {
		ListItem<Entity*>* auxIndex = auxList.start;

		for (ListItem<Entity*>* auxItem = auxList.start; auxItem != NULL; auxItem = auxItem->next) {
			if (auxItem->data->speed > auxIndex->data->speed || auxIndex == NULL) {
				auxIndex = auxItem;
			}
		}

		auxList.Del(auxIndex);
		turnList.Add(auxIndex->data);
		auxIndex = NULL;
	}
	
	return true;
}

bool BattleManager::SelectTargets() {


	ListItem<TileData*>* tileListItem;
	


	for (tileListItem = actionArea.start; tileListItem != NULL; tileListItem = tileListItem->next) {

		if (tileListItem->data->character != NULL && tileListItem->data->character->isEnemy==true && currentTurn->isEnemy==false) {

			targets.Add(tileListItem->data->character);
		}
		else if (tileListItem->data->character != NULL && tileListItem->data->character->isEnemy == false && currentTurn->isEnemy == true) {

			targets.Add(tileListItem->data->character);
		}

	}

	return true;
}

bool BattleManager::DisplayEnemys() {


	for (ListItem<Entity*>* entitylist = targets.start; entitylist != NULL; entitylist = entitylist->next) {
		iPoint pos = iPoint(entitylist->data->position.x, entitylist->data->position.y);

		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 250, 0, 0, 100);

	}


	return true;
}

bool BattleManager::UpdateTurnList() {

	ListItem<Entity*>* auxItem;
	auxItem = turnList.start;

	turnList.Del(auxItem);
	turnList.Add(auxItem->data);

	currentTurn = turnList.start->data;

	if (currentTurn != nullptr) {
		return true;
	}
	else{
		UpdateTurnList();
	}


	
}

bool BattleManager::DisplayTurnList() {
	int y = 1;
	int contador = 0;
	app->render->DrawTexture(turnList.At(1)->data->IconGtexture, 1775, 750);
	for (int i = 2; i <= turnList.Count()-1; i++) {

		if (turnList.At(i)->data->isAlive == true&& contador < 2) {

			app->render->DrawTexture(turnList.At(i)->data->Icontexture, 1800, 820 + (70 * y));
			contador++;
		}
		y++;
	}
	if (y >= turnList.Count()-1 && contador < 2 && turnList.At(0)->data->isAlive == true) {

		app->render->DrawTexture(turnList.At(0)->data->Icontexture, 1800, 820+(70*y) );
	}



	return true;
}

bool BattleManager::DisplayAtackArea(ActionType type) {

	bool ret = true;

	ListItem<TileData*>* tileListItem;
	tileListItem = actionArea.start;

	uint color[3];

	switch (type)
	{
	case ActionType::MOVE:
		color[0] = 255;
		color[1] = 0;
		color[2] = 0;
		break;
	case ActionType::ATTACK:
		color[0] = 0;
		color[1] = 255;
		color[2] = 0;
		break;
	case ActionType::ABILITY:
		color[0] = 0;
		color[1] = 0;
		color[2] = 255;
		break;
	default:
		break;
	}

	while (tileListItem != NULL) {

		iPoint pos = app->map->MapToWorld(tileListItem->data->x, tileListItem->data->y);
		app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, color[0], color[1], color[2], 100);

		tileListItem = tileListItem->next;
	}

	return ret;
}

bool BattleManager::DisplayMoveArea(ActionType type) {
	
		bool ret = true;
	
		ListItem<TileData*>*tileListItem;
		tileListItem = area.start;
	
		uint color[3];
	
	
		color[0] = 255;
		color[1] = 0;
		color[2] = 0;
		
	
		while (tileListItem != NULL) {
	
			
			iPoint pos = app->map->MapToWorld(tileListItem->data->x, tileListItem->data->y);
			app->render->DrawRectangle({ pos.x,pos.y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, 0, 0, 250, 100);
	
			tileListItem = tileListItem->next;
		}
	
		return ret;
	}

bool BattleManager::ApplyAction(Entity* character, ActionType type) {

	for (ListItem<Entity*>* targetItem = targets.start; targetItem != NULL; targetItem = targetItem->next) {
		switch (type) {
		case ActionType::ATTACK:
			targetItem->data->TakeDamage(character->attack);
			break;
		case ActionType::ABILITY:
			targetItem->data->TakeDamage(character->Ab1Power);
			break;
		default:
			break;
		}
	}

	return true;
}

void BattleManager::CheckWinCondition()
{
	LiveCondition();

	if (allies.Count() == 0 || app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) {

		finish = true;
		if (lose == false) {
			if (changeScreenTimer == -1) {
				changeScreenTimer = 100;
			
			}

			changeScreenTimer--;
			if (changeScreenTimer == 0) {
				app->audio->PlayFx(looseFx);
				battleState = BattleState::LOSE;
				lose = true;
			}
		}
	}
	
	if (enemies.Count() == 0 || app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		
		finish = true;
		if (lose == false) {
			if (changeScreenTimer == -1) {
				changeScreenTimer = 100;

			}

			changeScreenTimer--;
			if (changeScreenTimer == 0) {
				app->audio->PlayFx(victoryFx);
				battleState = BattleState::WIN;
				win = true;

			}
		}
	}
	
}

void BattleManager::LiveCondition() {



	for (ListItem<Entity*>* ally = allies.start; ally != NULL; ally = ally->next) {

		if (ally->data->health <= 0) {

			ally->data->isAlive = false;
		}
		if (ally->data->isAlive == false) {
			app->audio->PlayFx(deathFx);
			allies.Del(ally);
			turnList.Clear();
			MakeTurnList();
			while (currentTurn != turnList.start->data)
			{
				UpdateTurnList();
			}
			
		 }
	}
	for (ListItem<Entity*>* enemy = enemies.start; enemy != NULL; enemy = enemy->next) {


		if (enemy->data->health <= 0) {

			enemy->data->isAlive = false;
		}
		if (enemy->data->isAlive == false) {
			app->audio->PlayFx(deathFx);
		
			enemies.Del(enemy);
			turnList.Clear();
			MakeTurnList();
			while (currentTurn!=turnList.start->data)
			{
				UpdateTurnList();
			}
			
			
		
		}
	}
}

void BattleManager::PauseMenuAppear()
{
	if (app->uiModule->currentMenuType == COMBAT)
	{
		app->uiModule->pausecombat_menu_animation.Foward();
		app->uiModule->currentMenuType = COMBAT_PAUSE;
		// Call this function only when scene is changed
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
	}
	else
	{
		app->uiModule->pausecombat_menu_animation.Backward();
		app->uiModule->currentMenuType = COMBAT;
		// Call this function only when scene is changed
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

	}
}

void BattleManager::GodMode()
{

	for (ListItem<Entity*>* ally = allies.start; ally != NULL; ally = ally->next) {

		ally->data->health = ally->data->maxHealth;
		ally->data->stamina = ally->data->maxStamina;
		
	}
}

void BattleManager::TriggerAIAttack()
{
	ListItem<Entity*>* entitylist;
		entitylist = targets.start;
	
		while (entitylist != NULL) {
	
			if (entitylist->data->isAlive == true) {
				entitylist->data->health = entitylist->data->health - (currentTurn->attack - entitylist->data->defense);
				targets.Clear();
				currentTurn->UseStamina(5);
				app->sceneBattle->TakeDamageAnimation(entitylist->data->name.GetString());

			}
	
			entitylist = entitylist->next;
	
		}
}

bool BattleManager::IaEnemyAttack() {

	GetActionArea(currentTurn, ActionType::ATTACK);
	SelectTargets();

	if (currentTurn->stamina >= 5) {
		ListItem<Entity*>* entitylist;
		for (entitylist = targets.start; entitylist != NULL; entitylist = entitylist->next) {
			if (entitylist->data->isAlive == true) {
				return true;
			}

		}
	}
	return false;
}

//Mira si puedes desplazarte, hacia donde te mueves y crea el path.
bool BattleManager::IaEnemyMove() {

	GetActionArea(currentTurn, ActionType::MOVE);

	if (currentTurn->stamina >= 5) {
		ListItem<TileData*>* tiledata;
		for (tiledata = actionArea.start; tiledata != NULL; tiledata = tiledata->next) {

			if (tiledata->data->type == TILE_TYPE::FLOOR) {
				iPoint pos = iPoint(tiledata->data->x, tiledata->data->y);

				CreateArea(currentTurn->AttArea, 1, pos);
				for (int i = 0; i < area.Count(); i++) {

					if (area.At(i)->data->character!=NULL &&area.At(i)->data->character->isAlive==true) {

						length = app->pathfinding->CreatePath(origin, pos);
						destination.x = pos.x;
						destination.y = pos.y;
						currentTurn->UseStamina(3);
						i = area.Count();

						return true;

					}

				}

			}
		}


	}
	return false;

}

bool BattleManager::CreateArea(int range, int type, iPoint posTile) {


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
		if (combatMap[posTile.x][posTile.y - 1].type == TILE_TYPE::FLOOR) {
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
		for (i = 0; i <= range; i++) {
			for (j = 0; j <= range - i; j++) {
				if (combatMap[posTile.x + j][posTile.y + i].type == TILE_TYPE::FLOOR) {
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
				iPoint pos = iPoint(i, j);
				if (combatMap[j][i].type == TILE_TYPE::FLOOR) {

					area.Add(&combatMap[j][i]);

				}

			}
		}
		break;
	case 4:
		//LRRH attack area

		//for (int i = -1; i < 2; i++) {

		//	for (int j = 0; j < 8; i++) {

		//		if (posTile.x - j <2  && posTile.y + i < 9 && combatMap[posTile.x + j][posTile.y+i].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x + i][posTile.y+i]);
		//		}
		//		if (posTile.x - j < 2 && posTile.y + i > 9 && combatMap[posTile.x - j][posTile.y+i].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x - j][posTile.y+i]);
		//		}
		//		if (posTile.x+i >14 && posTile.y + j>6 && combatMap[posTile.x+i][posTile.y + j].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x+i][posTile.y + i]);
		//		}
		//		if (posTile.x + i > 14 && posTile.y - j < 2 && combatMap[posTile.x+i][posTile.y - j].type == TILE_TYPEE::FLOOR) {
		//			area.Add(&combatMap[posTile.x+i][posTile.y - j]);
		//		}
		//	}

		//}

		break;
	}


	return true;

}


