#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "W2_Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

W2_Scene::W2_Scene(bool isActive) : Module(isActive)
{
	name.Create("w2_scene");
}

// Destructor
W2_Scene::~W2_Scene()
{}

// Called before render is available
bool W2_Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading W2_Scene");
	bool ret = true;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();
	
	app->physics->Enable();

	npc = (NpcW2*)app->entityManager->CreateEntity(EntityType::NPCW2);
	npc->parameters = config.child("npcW2");

	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
		player->position.x = config.child("player").attribute("x").as_int();
		player->position.y = config.child("player").attribute("y").as_int();
	}

	app->entityManager->Awake(config);

	CreateDialogue(); //3MB

	npcPopUpTexture = app->tex->Load("Assets/Characters/Characters_popupsDialogueCut.png");
	uiSpriteTexture = app->tex->Load("Assets/UI/UI_SpriteSheet.png");

	return ret;
}

// Called before the first frame
bool W2_Scene::Start()
{
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
		player->ChangePosition(1868, 5608);
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
	numTimesPigsDialogueTriggered = 0;

	app->audio->PlayMusic("Assets/Sounds/Music/music_firstvillage_tension.wav", 0.2f);

	return true;
}

// Called each loop iteration
bool W2_Scene::PreUpdate()
{
	return true;
}



// Called each loop iteration
bool W2_Scene::Update(float dt)
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

	float speed = 0.2 * dt;
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);

	GodMode();

	// Menu appear
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		// If player is in pause, close it
		if (player->playerState == player->PlayerState::PAUSE)
		{
			player->playerState = player->playerPrevState;

			app->uiModule->currentMenuType = DISABLED;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
		}
		// If player is NOT in pause, open it
		else
		{
			// Save previous state to go back
			player->playerPrevState = player->playerState;
			player->playerState = player->PlayerState::PAUSE;

			app->uiModule->currentMenuType = PAUSE;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
		}
	}


	// Draw map
	app->map->Draw();

	return true;
}

void W2_Scene::AppearDialogue()
{
	if (player->playerState == player->PlayerState::NPC_INTERACT)
	{
		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = DIALOG;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

		for (auto& e : dialogue)
		{
			std::cout << e << std::endl;
		}

	}
}

// Called each loop iteration
bool W2_Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	// When exit button click, close app
	if (exitButtonBool == true)
	{
		return false;
	}

	

	return ret;
}

bool W2_Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
		// Option 1
	case 12:
		app->scene->player->buttonOption1 = true;
		break;
		// Option 2
	case 13:
		app->scene->player->buttonOption2 = true;
		break;
		// Option 3
	case 14:
		app->scene->player->buttonOption3 = true;
		break;
		// Option 4
	case 15:
		app->scene->player->buttonOption4 = true;
	default:
		break;
	}
	return true;
}

// Called before quitting
bool W2_Scene::CleanUp()
{
	LOG("Freeing scene");
	//app->fonts->Unload(font);
	app->map->CleanUp(); 
	app->entityManager->CleanUp(); 
	app->physics->Disable();

	app->tex->UnLoad(npcPopUpTexture);
	app->tex->UnLoad(uiSpriteTexture);
	

	return true;
}

//Toggles god mode on and off when the F10 key is pressed. When god mode is on, the camera follows the player's position without any boundaries.
void W2_Scene::GodMode()
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
void W2_Scene::Camera()
{
	if (godMode)
	{
		app->render->FollowObject(-(int)player->position.x, -(int)player->position.y - 35,
			app->render->camera.w / 2, app->render->camera.h / 2);
	}
	else
	{
		app->render->FollowObjectRespectBoundaries(-(int)player->position.x, -(int)player->position.y - 35,
			app->render->camera.w / 2, app->render->camera.h / 2);
	}

}


//Runs a dialogue tree for a specific NPC, identified using a ColliderType enum. This function delegates the NPC specific behavior to other functions based on the enum passed in.
void W2_Scene::RunDialogueTree(ColliderType NPC)
{
	switch (NPC)
	{
	case ColliderType::PIGS:
		dialogue = pigsTree->Run();

		for (auto& text : dialogue)
		{
			std::cout << text << std::endl;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			UpdateDialogueTree(3);
		}

		if (dialogue.empty() && !pigsDefeated)
		{
			dialogue.push_back("Little and Middle pig : We're pigs and we do as we please. What makes you think we care about what you think or try? Get ready.");
		}
		break;
	case ColliderType::WOLF:
		dialogue = wolfTree->Run();

		break;
	default:
		break;
	}
}

//Updates the dialogue tree based on the player's response to a dialogue prompt. The dialogue tree to update is selected based on the type of NPC the player last interacted
//with, as determined by the player's lastCollision variable. 
void W2_Scene::UpdateDialogueTree(int option)
{
	if (1 >= option <= 4)
	{
		switch (app->w2_scene->player->lastCollision)
		{
		case ColliderType::PIGS:
			pigsTree->Update(option);
			numTimesPigsDialogueTriggered++;
			break;

		case ColliderType::WOLF:
			wolfTree->Update(option);
			break;
		default:
			break;
		}
	}



}


//Create Tree Dialogues
void W2_Scene::CreateDialogue()
{//Wolfs


	if (!pigsDefeated)
	{
		// - Angry Villager Post Tutorial
	//3rd level
		auto secondOption1PT = std::make_shared<DialogueNode>();
		secondOption1PT->SetText("What happened here? The houses are destroyed.");

		auto secondOption2PT = std::make_shared<DialogueNode>();
		secondOption2PT->SetText("How could you do that? You're monsters and you'll be the next ones to become bacon.");

		auto secondOption3PT = std::make_shared<DialogueNode>();
		secondOption3PT->SetText("What guarantees that you won't do the same to us? I'll end you (with a trembling voice).");

		auto secondOption4PT = std::make_shared<DialogueNode>();
		secondOption4PT->SetText("Whatever, let's just move on and get this over with.");

		//2nd level
		auto PigsToOptionPT = std::make_shared<DialogueNode>();
		PigsToOptionPT->SetText("Little  pig: Oh, don't worry about him. He was just an obstacle in our way to freedom. Middle pig: Yeah, after all, we were very hungry.");
		PigsToOptionPT->AddChild(secondOption1PT);
		PigsToOptionPT->AddChild(secondOption2PT);
		PigsToOptionPT->AddChild(secondOption3PT);
		PigsToOptionPT->AddChild(secondOption4PT);

		//1rst level
		auto firstOption1PT = std::make_shared<DialogueNode>();
		firstOption1PT->SetText("What happened here? The houses are destroyed.");
		firstOption1PT->AddChild(PigsToOptionPT);

		auto firstOption2PT = std::make_shared<DialogueNode>();
		firstOption2PT->SetText("What the hell did you do to your older brother, you despicable pigs?");
		firstOption2PT->AddChild(PigsToOptionPT);

		auto firstOption3PT = std::make_shared<DialogueNode>();
		firstOption3PT->SetText("W-where is your older brother? Will we find him in the portal?");
		firstOption3PT->AddChild(PigsToOptionPT);

		auto firstOption4PT = std::make_shared<DialogueNode>();
		firstOption4PT->SetText("Fine, fine, I get it, more trouble. Because I can't have a peaceful life.");
		firstOption4PT->AddChild(PigsToOptionPT);

		//Root
		auto firstNodePigs = std::make_shared<DialogueNode>();
		firstNodePigs->SetText("Little  pig: (with a sinister smile) Hi, friend. What brings you here?. Middle pig: (laughing) Looks like you arrived at the wrong time.");
		firstNodePigs->AddChild(firstOption1PT);
		firstNodePigs->AddChild(firstOption2PT);
		firstNodePigs->AddChild(firstOption3PT);
		firstNodePigs->AddChild(firstOption4PT);
		firstNodePigs->ActivateNode();

		//Tree
		pigsTree = std::make_shared<DialogueTree>();
		pigsTree->SetRoot(firstNodePigs);
	}
	else
	{
		auto firstNodeWolf = std::make_shared<DialogueNode>();
		firstNodeWolf->SetText("You will pay for this, Timmy. And you, you pathetic pigs, couldn't even defeat him. You're all worthless.");
		firstNodeWolf->ActivateNode();

		wolfTree = std::make_shared<DialogueTree>();
		wolfTree->SetRoot(firstNodeWolf);

		//PigsAfterCombat:
		//1rst level
		auto firstOption1AC = std::make_shared<DialogueNode>();
		firstOption1AC->SetText("Thank you for your apology. We can work together to stop the wolf and avenge your brother.");

		auto firstOption2AC = std::make_shared<DialogueNode>();
		firstOption2AC->SetText("You better not betray us again or you'll regret it. But for now, welcome to the team.");

		auto firstOption3AC = std::make_shared<DialogueNode>();
		firstOption3AC->SetText("I don't know if I can trust you, but I need all the help I can get. Welcome to the group.");

		auto firstOption4AC = std::make_shared<DialogueNode>();
		firstOption4AC->SetText("Whatever, I don't really care. As long as you don't get in my way, you can join us.");

		auto firstNodePigsAC = std::make_shared<DialogueNode>();
		firstNodePigsAC->SetText("SmallerMiddle pig: I'm sorry, Timmy. I don't know how we could do such a thing. We were carried away by the darkness. Little pig: (nodding) Yes, I'm so sorry. But now we want to help you. We know we can't bring our brother back to life, but we can help you avenge him.");
		firstNodePigsAC->AddChild(firstOption1AC);
		firstNodePigsAC->AddChild(firstOption2AC);
		firstNodePigsAC->AddChild(firstOption3AC);
		firstNodePigsAC->AddChild(firstOption4AC);
		firstNodePigsAC->ActivateNode();

		pigsTree = std::make_shared<DialogueTree>();
		pigsTree->SetRoot(firstNodePigsAC);
	}



	

}

bool W2_Scene::LoadState(pugi::xml_node& data)
{
	loadPlayerPosX = data.child("player").attribute("x").as_int();
	loadPlayerPosY = data.child("player").attribute("y").as_int();

	player->ChangePosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());

	pugi::xml_node battleInfo = data.parent().child("BattleInfo");
	pigsDefeated = battleInfo.attribute("isPigDefeated").as_bool();

	return true;
}

bool W2_Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playerNode = data.append_child("player");

	//// If door, save mes lluny
	//if (app->uiModule->doorPlayerPosition)
	//{
	//	playerNode.append_attribute("x") = player->position.x;
	//	playerNode.append_attribute("y") = player->position.y + 75;
	//	app->uiModule->doorPlayerPosition = false;
	//}

	//playerNode.append_attribute("x") = player->position.x;
	//playerNode.append_attribute("y") = player->position.y;

	

	return true;
}


//This function swaps after 2sec from Scene to SceneBattle/LRRHCombat If Player has triggered the dialogue with AngryVillager/LRRH and they haven't been defeated before
void W2_Scene::MoveToBattleFromDialogue()
{
	if (numTimesPigsDialogueTriggered == 1 && !pigsDefeated)
	{
		timerToPigsCombat.Start(2.0f);
		numTimesPigsDialogueTriggered = 0;
	}

	if (timerToPigsCombat.Test() == estadoTimerP::FIN)
	{
		//Teleportar a GameScene::Pigcombat
		//doors.At(0)->data->TriggerDoor(GameScene::BATTLE);
	}



}