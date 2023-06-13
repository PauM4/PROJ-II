#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "W3_Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"
#include "ParticleSystem.h"
#include "ModuleParticles.h"

#include "Defs.h"
#include "Log.h"

W3_Scene::W3_Scene(bool isActive) : Module(isActive)
{
	name.Create("w3_scene");
}

// Destructor
W3_Scene::~W3_Scene()
{}

// Called before render is available
bool W3_Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading W3_Scene");
	bool ret = true;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();
	
	app->physics->Enable();

	npc = (NpcW3*)app->entityManager->CreateEntity(EntityType::NPCW3);
	npc->parameters = config.child("npcW3");


	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
		player->position.x = config.child("player").attribute("x").as_int();
		player->position.y = config.child("player").attribute("y").as_int();
	}

	portal = (Portal*)app->entityManager->CreateEntity(EntityType::PORTAL);
	portal->parameters = config.child("portal");

	app->entityManager->Awake(config);

	CreateDialogue(); //3MB

	npcPopUpTexture = app->tex->Load("Assets/Characters/Characters3_popupsDialogueCut.png");
	inventoryItemsTexture = app->tex->Load("Assets/UI/itemImage_petita.png");
	eKeyTexture = app->tex->Load("Assets/UI/eKey.png");
	questUiTexture = app->tex->Load("Assets/UI/questUI.png");
	inventoryScrollTexture = app->tex->Load("Assets/UI/.png");
	inventoryScrollTexture = app->tex->Load("Assets/UI/inventoryScroll.png");
	inventoryItemsTexture = app->tex->Load("Assets/UI/itemImage_petita.png");
	lvlupTexture = app->tex->Load("Assets/UI/blank.png");
	darkparticle_texture = app->tex->Load("Assets/UI/dark_particle.png");
	endComicTexture = app->tex->Load("Assets/UI/EndGame.png");


	currentQuestIndex = 0;

	Quest quest1;
	quest1.completed = false;
	quest1.description = "Stop the wolf!";
	questList.push_back(quest1);

	Quest quest2;
	quest2.completed = false;
	quest2.description = "Wake up...";
	questList.push_back(quest2);


	return ret;
}

// Called before the first frame
bool W3_Scene::Start()
{
	player->walkFx = app->audio->LoadFx("Assets/Sounds/FX/fx_grass_walk.wav");
	app->entityManager->Start();
	

	// L03: DONE: Load map
	bool retLoad = app->map->Load(mapName, mapFolder);

	// L12 Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		//bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		//if(retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}

	if (isNewGame)
	{
		player->ChangePosition(1026, 3000);
		isNewGame = false;
	}
	else
	{
		app->LoadGameRequest();
	}


	pauseMenuActive = false;
	exitButtonBool = false;

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
	
	dialogue.push_back("");

	godMode = false;

	app->audio->PlayMusic("Assets/Sounds/Music/music_wolf_world.ogg", 0.1f);

	particle_dark1.x = 300;
	particle_dark1.y = 2661;
	particle_dark1.velocity_x = 70;
	particle_dark1.velocity_y = -70;
	particle_dark1.spreadfactor = 360;
	particle_dark1.lifetime = 3;
	particle_dark1.beginscale = 50;
	particle_dark1.endscale = 0;
	particle_dark1.r = 255;
	particle_dark1.g = 0;
	particle_dark1.b = 0;
	particle_dark1.r2 = 0;
	particle_dark1.g2 = 0;
	particle_dark1.b2 = 255;
	particle_dark1.scaleVariation = 1;
	particle_dark1.particlepersecond = 20;
	particle_dark1.particletexture = darkparticle_texture;

	particle_dark2.x = 1800;
	particle_dark2.y = 2661;
	particle_dark2.velocity_x = -70;
	particle_dark2.velocity_y = -70;
	particle_dark2.spreadfactor = 360;
	particle_dark2.lifetime = 3;
	particle_dark2.beginscale = 50;
	particle_dark2.endscale = 0;
	particle_dark2.r = 255;
	particle_dark2.g = 0;
	particle_dark2.b = 0;
	particle_dark2.r2 = 0;
	particle_dark2.g2 = 0;
	particle_dark2.b2 = 255;
	particle_dark2.scaleVariation = 1;
	particle_dark2.particlepersecond = 20;
	particle_dark2.particletexture = darkparticle_texture;

	ParticleSystem* particlesystem_dark1 = new ParticleSystem(particle_dark1);
	ParticleSystem* particlesystem_dark2 = new ParticleSystem(particle_dark2);
	app->moduleParticles->emiters.push_back(particlesystem_dark1);
	app->moduleParticles->emiters.push_back(particlesystem_dark2);

	takePortal = false; 

	return true;
}

// Called each loop iteration
bool W3_Scene::PreUpdate()
{
	return true;
}



// Called each loop iteration
bool W3_Scene::Update(float dt)
{
	std::cout << "X: " << player->position.x << std::endl;
	std::cout << "Y: " << player->position.y << std::endl;
	Camera();

	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();



	GodMode();

	// Menu appear
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN)
	{
		app->uiModule->pause_menu_animation_bool = true;
		// If player is in pause, close it
		if (player->playerState == player->PlayerState::PAUSE)
		{
			app->uiModule->pause_menu_animation.Backward();
			player->playerState = player->playerPrevState;

			app->uiModule->pause_menu_animation_bool = false;
			app->uiModule->menu_pause = false;

			app->uiModule->currentMenuType = DISABLED;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
			app->uiModule->menu_pause = true;
		}
		// If player is NOT in pause, open it
		else
		{
			app->uiModule->pause_menu_animation.Foward();
			// Save previous state to go back
			player->playerPrevState = player->playerState;
			player->playerState = player->PlayerState::PAUSE;

			app->uiModule->currentMenuType = PAUSE;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
			app->uiModule->menu_pause = true;
		}
	}
	MoveToBattleFromDialogue();

	// Draw map
	app->map->Draw();

	return true;
}

void W3_Scene::AppearDialogue()
{
	if (player->playerState == player->PlayerState::NPC_INTERACT)
	{
		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = DIALOG3;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		for (auto& e : dialogue)
		{
			std::cout << e << std::endl;
		}

	}
}

// Called each loop iteration
bool W3_Scene::PostUpdate()
{
	bool ret = true;

	if ((app->uiModule->offset + app->uiModule->point * (-app->render->camera.y - app->uiModule->offset)) >= -app->render->camera.y - 20 && app->uiModule->menu_pause) {
		if (player->playerState == player->PlayerState::PAUSE) {
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
			app->uiModule->menu_pause = false;
		}

	}
	
	if (!godMode) app->map->PostDraw(player->position.y + 40);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	// When exit button click, close app
	if (exitButtonBool == true)
	{
		return false;
	}

	

	return ret;
}

bool W3_Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
		// Option 1
	case 34:
		app->w3_scene->player->buttonOption1 = true;
		break;
		// Option 2
	case 35:
		app->w3_scene->player->buttonOption2 = true;
		break;
		// Option 3
	case 36:
		app->w3_scene->player->buttonOption3 = true;
		break;
		// Option 4
	case 37:
		app->w3_scene->player->buttonOption4 = true;
	default:
		break;
	}
	return true;
}

// Called before quitting
bool W3_Scene::CleanUp()
{
	LOG("Freeing scene");
	//app->fonts->Unload(font);
	app->map->CleanUp(); 
	app->entityManager->CleanUp(); 
	app->physics->Disable();

	app->tex->UnLoad(npcPopUpTexture);
	app->tex->UnLoad(inventoryItemsTexture);
	app->tex->UnLoad(eKeyTexture);
	app->tex->UnLoad(questUiTexture);
	app->tex->UnLoad(inventoryScrollTexture);
	app->tex->UnLoad(inventoryItemsTexture);
	app->tex->UnLoad(lvlupTexture);
	app->tex->UnLoad(darkparticle_texture);
	app->tex->UnLoad(endComicTexture); 

	app->moduleParticles->CleanUp();

	return true;
}

//Toggles god mode on and off when the F10 key is pressed. When god mode is on, the camera follows the player's position without any boundaries.
void W3_Scene::GodMode()
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godMode)
		{
			godMode = false;
		}
		else
		{
			godMode = true;
		}
	}
}

//Updates the camera position based on the player's position. If god mode is on, the camera follows the player's position without any boundaries. If god mode is off, the camera follows the player's position while respecting the game's boundaries.
void W3_Scene::Camera()
{
	if (godMode || !CheckInsideBoundaries())
	{
		app->render->FollowObject(-(int)player->position.x, -(int)player->position.y - 35,
			app->render->camera.w / 2, app->render->camera.h / 2);
	}
	else
	{
		app->render->FollowObjectRespectBoundaries(-(int)player->position.x, -(int)player->position.y - 35,
			app->render->camera.w / 2, app->render->camera.h / 2, -448, -3, -2079, -24);
	}

}

bool W3_Scene::CheckInsideBoundaries()
{
	bool insideX = (player->position.x == clamp(player->position.x, 3, 448 + (1920)));
	bool insideY = (player->position.y == clamp(player->position.y, 24, 2079 + 1080));

	if (insideX && insideY)
	{
		return true;
	}

	return false;
}


//Runs a dialogue tree for a specific NPC, identified using a ColliderType enum. This function delegates the NPC specific behavior to other functions based on the enum passed in.
void W3_Scene::RunDialogueTree(ColliderType NPC)
{

	switch (NPC)
	{
	case ColliderType::WOLF:
		dialogue = wolfTree->Run();


		if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			UpdateDialogueTree(3);
		}

		for (auto& text : dialogue)
		{
			std::cout << text << std::endl;
		}

		if (dialogue.empty() && wolfDefeated)
		{
			dialogue.push_back("Timmy, thank you for showing me that there is still hope for me. Your bravery has inspired me to do what is right, even if it means sacrificing myself. The evil I have caused is too great to be ignored, and my existence will only perpetuate the darkness. But with my death, I can release my soul from this curse and protect others from its influence. -The wolf kneels down and impales his heart with the sword, falling to the ground in a pool of blood. - Finally, I was able to find redemption in your bravery.");
		}
		break;

	default:
		break;
	}



}

//Updates the dialogue tree based on the player's response to a dialogue prompt. The dialogue tree to update is selected based on the type of NPC the player last interacted
//with, as determined by the player's lastCollision variable. 
void W3_Scene::UpdateDialogueTree(int option)
{
	if (1 >= option <= 4)
	{
		switch (app->w3_scene->player->lastCollision)
		{
		case ColliderType::WOLF:
			wolfTree->Update(option);
			numTimesWolfDialogueTriggered++;
			break;

		default:
			break;
		}
	}



}


//Create Tree Dialogues
void W3_Scene::CreateDialogue()
{
	if (!wolfDefeated)
	{
		// - Wolf
		//2nd Level
		auto PToOption1 = std::make_shared<DialogueNode>();
		PToOption1->SetText("There is no escape from my wrath. Prepare to face my power.");

		auto PToOption2 = std::make_shared<DialogueNode>();
		PToOption2->SetText("Your threats are futile. Come and face my wrath if you dare.");

		auto PToOption3 = std::make_shared<DialogueNode>();
		PToOption3->SetText("Your fear only fuels my power. Prepare to face me, for you will have no other choice.");

		auto PToOption4 = std::make_shared<DialogueNode>();
		PToOption4->SetText("Your indifference only shows your weakness. Come and prove me wrong.");


		//1st Level
		auto firstOption1P = std::make_shared<DialogueNode>();
		firstOption1P->SetText("I'm ready to fight.");
		firstOption1P->AddChild(PToOption1);

		auto firstOption2P = std::make_shared<DialogueNode>();
		firstOption2P->SetText("I'm gonna crush your balls.");
		firstOption2P->AddChild(PToOption2);

		auto firstOption3P = std::make_shared<DialogueNode>();
		firstOption3P->SetText("I want to cry - sobs.");
		firstOption3P->AddChild(PToOption3);

		auto firstOption4P = std::make_shared<DialogueNode>();
		firstOption4P->SetText("yawns - Can't we settle this without violence? I'm too tired for a fight.");
		firstOption4P->AddChild(PToOption4);


		//Root
		auto firstNodeP = std::make_shared<DialogueNode>();
		firstNodeP->SetText("You dare enter my domain, fools? You think you can defeat me? You have no idea what kind of power you're up against. The darkness within me grows stronger every day, and I will not hesitate to destroy you.");
		firstNodeP->AddChild(firstOption1P);
		firstNodeP->AddChild(firstOption2P);
		firstNodeP->AddChild(firstOption3P);
		firstNodeP->AddChild(firstOption4P);
		firstNodeP->ActivateNode();

		//Tree
		wolfTree = std::make_shared <DialogueTree>();
		wolfTree->SetRoot(firstNodeP);
	}
	else
	{
		//LittleRedAfterCombat
		auto fristNodePAC = std::make_shared<DialogueNode>();
		fristNodePAC->SetText("Timmy, thank you for showing me that there is still hope for me. Your bravery has inspired me to do what is right, even if it means sacrificing myself. The evil I have caused is too great to be ignored, and my existence will only perpetuate the darkness. But with my death, I can release my soul from this curse and protect others from its influence. -The wolf kneels down and impales his heart with the sword, falling to the ground in a pool of blood. - Finally, I was able to find redemption in your bravery.");
		fristNodePAC->ActivateNode();

		wolfTree = std::make_shared<DialogueTree>();
		wolfTree->SetRoot(fristNodePAC);
	}




}

bool W3_Scene::LoadState(pugi::xml_node& data)
{
	loadPlayerPosX = data.child("player").attribute("x").as_int();
	loadPlayerPosY = data.child("player").attribute("y").as_int();

	if (active) {
		player->ChangePosition(loadPlayerPosX, loadPlayerPosY);
	}


	pugi::xml_node battleInfo = data.parent().child("BattleInfo");
	wolfDefeated = battleInfo.attribute("isWolfDefeated").as_bool();

	if (wolfDefeated)
	{
		wolfTree->~DialogueTree();

		//WolfAfterCombat
		auto fristNodeW = std::make_shared<DialogueNode>();
		fristNodeW->SetText("Timmy, thank you for showing me that there is still hope for me. Your bravery has inspired me to do what is right, even if it means sacrificing myself. The evil I have caused is too great to be ignored, and my existence will only perpetuate the darkness. But with my death, I can release my soul from this curse and protect others from its influence. -The wolf kneels down and impales his heart with the sword, falling to the ground in a pool of blood. - Finally, I was able to find redemption in your bravery.");
		fristNodeW->ActivateNode();

		wolfTree = std::make_shared<DialogueTree>();
		wolfTree->SetRoot(fristNodeW);
	}

	return true;
}

bool W3_Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerNode = data.append_child("player");


	// If door, save mes lluny
	if (takePortal)
	{
		if (player != nullptr) {
			playerNode.append_attribute("x") = player->position.x + 100;
			playerNode.append_attribute("y") = player->position.y;
		}
	}
	else {
		if (player != nullptr) {
			playerNode.append_attribute("x") = player->position.x + 16;
			playerNode.append_attribute("y") = player->position.y + 16;
		}

	}

	return true;
}


void W3_Scene::MoveToBattleFromDialogue()
{

	if (numTimesWolfDialogueTriggered == 1 && !wolfDefeated)
	{
		timerToWolfCombat.Start(2.0f);
		numTimesWolfDialogueTriggered = 0;
	}

	if (timerToWolfCombat.Test() == estadoTimerP::FIN)
	{
		//Teleportar a GameScene::Pigcombat
		doors.At(0)->data->TriggerDoor(GameScene::COMBATWOLF);
	}

}

// A function to draw the current quest on the screen
void W3_Scene::drawQuest(int posX, int posY) {

	questText = questList[currentQuestIndex].description;

	SDL_Rect rect = { 0, 0, 280, 20 };

	textDialogue = app->fonts->LoadRenderedParagraph(rect, app->fonts->gameFont, questText, { 0,0,0 }, 280);

	app->render->DrawTexture(textDialogue, posX, posY, &rect);
}

// A function to move to the next quest
void W3_Scene::nextQuest() {
	currentQuestIndex++;
	if (currentQuestIndex >= questList.size()) {
		// If we've reached the end of the quest list, wrap around to the beginning
		currentQuestIndex = 0;
	}
}
