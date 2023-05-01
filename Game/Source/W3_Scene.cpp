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


	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	app->entityManager->Awake(config);

	CreateDialogue(); //3MB

	npcPopUpTexture = app->tex->Load("Assets/Characters/Characters_popupsDialogueCut.png");
	uiSpriteTexture = app->tex->Load("Assets/UI/UI_SpriteSheet.png");

	return ret;
}

// Called before the first frame
bool W3_Scene::Start()
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

	app->audio->PlayMusic("Assets/Sounds/Music/music_firstvillage_tension.wav", 0.2f);

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
	//if (pruebaj == 0)
	//{
	//	Prueba();
	//	std::cout <<" "<< std::endl;
	//	
	//}
	//else if (pruebaj == 1)
	//{
	//	//dialogo version tipo 2
	//	//angryVillagerTreePT->Update(1);
	//	//dialogue = angryVillagerTreePT->Run();
	//	//Prueba();

	//	//dialogo version tipo 1: funciona en las diferentes opciones
	//	//talismanVillagerTree->Update(2);
	//	//dialogue = talismanVillagerTree->Run();
	//	//Prueba();
	//}

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

void W3_Scene::AppearDialogue()
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
bool W3_Scene::PostUpdate()
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

bool W3_Scene::OnGuiMouseClickEvent(GuiControl* control)
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
bool W3_Scene::CleanUp()
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

//Return random number between 2 numbers
//int GenerateRandomNumber(int num1, int num2)
//{
//	auto eng = std::default_random_engine(std::time(0));
//	std::uniform_int_distribution<int> dist(num1, num2);
//
//	return dist(eng);
//
//
//}


void W3_Scene::Prueba()
{
	for (auto& e : dialogue)
	{
		std::cout << e << std::endl;
	}
	//	pruebaj++;

}

//Runs a dialogue tree for a specific NPC, identified using a ColliderType enum. This function delegates the NPC specific behavior to other functions based on the enum passed in.
void W3_Scene::RunDialogueTree(ColliderType NPC)
{
	switch (NPC)
	{
	case ColliderType::DEADVILLAGER:
		dialogue = deadVillagerTree->Run();

		if (dialogue.empty())
		{
			//Esto no hará falta porque cuando se muera ya no tendrá texto y tendriamos que hacer algo para que ya no pueda
			//interaccionar con el dead villager
		}
		break;
	case ColliderType::SHEEPA:
		dialogue = sheepATree->Run();

		if (dialogue.empty())
		{
			//Esto no deberia tener que estar empty pq no se actualiza y siempre tiene el mismo nodo
		}
		break;

	case ColliderType::SHEEPB:
		dialogue = sheepBTree->Run();

		if (dialogue.empty())
		{
			//Esto no deberia tener que estar empty pq no se actualiza y siempre tiene el mismo nodo
		}
		break;

	case ColliderType::SHEEPC:
		dialogue = sheepCTree->Run();

		if (dialogue.empty())
		{
			//Esto no deberia tener que estar empty pq no se actualiza y siempre tiene el mismo nodo
		}
		break;

	case ColliderType::SHEEPD:
		dialogue = sheepDTree->Run();

		if (dialogue.empty())
		{
			//Esto no deberia tener que estar empty pq no se actualiza y siempre tiene el mismo nodo
		}
		break;

	case ColliderType::WOLFBEFORECOMBAT:
		dialogue = wolfBeforeCombatTree->Run();

		if (dialogue.empty())
		{
			//Esto no hará falta porque cuando se muera ya no tendrá texto y tendriamos que hacer algo para que ya no pueda
			//interaccionar con el dead villager
		}
		break;

	case ColliderType::WOLFAFTERCOMBAT:
		dialogue = wolfAfterCombatTree->Run();

		if (dialogue.empty())
		{
			//Esto no deberia tener que estar empty pq no se actualiza y siempre tiene el mismo nodo
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
		switch (app->scene->player->lastCollision)
		{
		case ColliderType::DEADVILLAGER:
			deadVillagerTree->Update(option);
			break;

		case ColliderType::WOLFBEFORECOMBAT:
			wolfBeforeCombatTree->Update(option);
			break;

		case ColliderType::WOLFAFTERCOMBAT:
			wolfAfterCombatTree->Update(option);
			break;

		default:
			break;
		}
	}



}


//Create Tree Dialogues
void W3_Scene::CreateDialogue()
{
	//deadVillager
	auto firstOption1 = std::make_shared<DialogueNode>();
	firstOption1->SetText("Don't worry, we'll do it. - He says as he approaches the already dead villager and closes his eyes.");

	auto firstOption2 = std::make_shared<DialogueNode>();
	firstOption2->SetText("We'll crush everything that crosses our path! We'll make the devil himself tremble if necessary!");

	auto firstOption3 = std::make_shared<DialogueNode>();
	firstOption3->SetText("Save Pedro? What if he hurts us? I don't want to go there! - He says while looking at the rest of the group with terrified eyes.");

	auto firstOption4 = std::make_shared<DialogueNode>();
	firstOption4->SetText("Meh, we'll save Pedro, I guess. But don't expect us to take it too seriously.");

	auto firstDeadVillagerNode = std::make_shared<DialogueNode>();
	firstDeadVillagerNode->SetText("Pedro became furious because no one believed his story about the wolf, and when corruption affected him, he became even more dangerous. He became obsessed with the idea that no one believed him and that his honor was tarnished. The corruption in his heart consumed him, and he became a destructive force that threatened the entire town. He wanted to kill everyone in the town, but some managed to escape. The person coughs and pauses to catch their breath before continuing. Pedro's farm is beyond the forest - he says urgently. I don't know what has happened there, but I can feel that something bad has happened. Pedro is under its influence, he is not himself. I beg you to save him, he does not deserve to die like this. The man coughs again and becomes weak. Timmy and his group look at each other, knowing that they have a new important mission: to save Pedro from corruption.");
	firstDeadVillagerNode->AddChild(firstOption1);
	firstDeadVillagerNode->AddChild(firstOption2);
	firstDeadVillagerNode->AddChild(firstOption3);
	firstDeadVillagerNode->AddChild(firstOption4);

	deadVillagerTree = std::make_shared<DialogueTree>();
	deadVillagerTree->SetRoot(firstDeadVillagerNode);


	//sheeps
	auto sheepANode = std::make_shared<DialogueNode>();
	sheepANode->SetText("Path 1 is the right one.");
	sheepATree = std::make_shared<DialogueTree>();
	sheepATree->SetRoot(sheepANode);


	auto sheepBNode = std::make_shared<DialogueNode>();
	sheepBNode->SetText("Path 3 is the right one.");
	sheepBTree = std::make_shared<DialogueTree>();
	sheepBTree->SetRoot(sheepBNode);


	auto sheepCNode = std::make_shared<DialogueNode>();
	sheepCNode->SetText("D says path 2 is the right one.");
	sheepCTree = std::make_shared<DialogueTree>();
	sheepCTree->SetRoot(sheepCNode);


	auto sheepDNode = std::make_shared<DialogueNode>();
	sheepDNode->SetText("B and C tell lies.");
	sheepDTree = std::make_shared<DialogueTree>();
	sheepDTree->SetRoot(sheepDNode);


	auto wolfBeforeCombatNode = std::make_shared<DialogueNode>();
	wolfBeforeCombatNode->SetText("Well... it seems you've got me, I know you're probably itching to defeat me right now, but let me tell you something, Timmy, I wasn't always like this. There was a time when I was a happy and friendly wolf, living in the forest with my family. But then, something changed in me. I started feeling frustrated because I was always cast as the evil antagonist in stories. I wanted to be the hero, the main character, but I was never allowed to. So, one day I started doing bad things, without really knowing why or having control over myself, things I had never done before. I started stealing and intimidating other animals in the forest out of a need to prove my strength. I think I believed that this way, I would finally get the respect I so desperately wanted. But I was wrong. The more evil I became, the more alone I felt and the less control I had over myself. Nobody wanted to be around me, and my family distanced themselves from me. I had become a lonely and sad wolf. Sometimes I feel like I'm not myself anymore, like something takes over me and makes me do things I don't want to... (suddenly his eyes turn red and he looks at Timmy with an aggressive face).");
	wolfBeforeCombatTree = std::make_shared<DialogueTree>();
	wolfBeforeCombatTree->SetRoot(wolfBeforeCombatNode);


	auto wolfAfterCombatNode = std::make_shared<DialogueNode>();
	wolfAfterCombatNode->SetText("Timmy, thank you for showing me that there is still hope for me. Your bravery has inspired me to do what is right, even if it means sacrificing myself. The evil I have caused is too great to be ignored, and my existence will only perpetuate the darkness. But with my death, I can release my soul from this curse and protect others from its influence. -The wolf kneels down and impales his heart with the sword, falling to the ground in a pool of blood. - Finally, I was able to find redemption in your bravery.");
	wolfAfterCombatTree = std::make_shared<DialogueTree>();
	wolfAfterCombatTree->SetRoot(wolfAfterCombatNode);

}

bool W3_Scene::LoadState(pugi::xml_node& data)
{
	//loadPlayerPosX = data.child("player").attribute("x").as_int();
	//loadPlayerPosY = data.child("player").attribute("y").as_int();

	//player->ChangePosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());

	return true;
}

bool W3_Scene::SaveState(pugi::xml_node& data)
{
	//pugi::xml_node playerNode = data.append_child("player");

	//// If door, save mes lluny
	//if (app->uiModule->doorPlayerPosition)
	//{
	//	playerNode.append_attribute("x") = player->position.x;
	//	playerNode.append_attribute("y") = player->position.y + 75;
	//	app->uiModule->doorPlayerPosition = false;
	//}

	//playerNode.append_attribute("x") = player->position.x;
	//playerNode.append_attribute("y") = player->position.y;

	//

	return true;
}