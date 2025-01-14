#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "PathFinding.h"
#include "GuiManager.h"
#include "Fonts.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool isActive) : Module(isActive)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	mapName = config.attribute("name").as_string();
	mapFolder = config.attribute("path").as_string();
	
	app->physics->Enable();

	npc1 = (Npc*)app->entityManager->CreateEntity(EntityType::NPC);
	npc1->parameters = config.child("npc");



	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
		player->position.x = config.child("player").attribute("x").as_int();
		player->position.y = config.child("player").attribute("y").as_int();
	}


	for (pugi::xml_node doorNode = config.child("door"); doorNode; doorNode = doorNode.next_sibling("door")) {
		Door* door = (Door*)app->entityManager->CreateEntity(EntityType::DOOR);
		door->parameters = doorNode;

		doors.Add(door);
	}

	for (pugi::xml_node chestNode = config.child("chest"); chestNode; chestNode = chestNode.next_sibling("chest")) {
		Item* chest = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		chest->parameters = chestNode;

		chests.Add(chest);
	}

	pressKeyAnim.PushBack({ 0, 0, 485, 734 });
	pressKeyAnim.PushBack({ 485, 0, 485, 735 });
	pressKeyAnim.loop = true;
	pressKeyAnim.speed = 0.1f;

	app->entityManager->Awake(config);

	CreateDialogue(); //3MB

	npcPopUpTexture = app->tex->Load("Assets/Characters/Characters_popupsDialogueCut.png");
	uiSpriteTexture = app->tex->Load("Assets/UI/UI_SpriteSheet.png");
	ropeTexture = app->tex->Load("Assets/UI/ropeImage.png");
	pressKeyTexture = app->tex->Load("Assets/UI/pressEanimation.png");
	questUiTexture = app->tex->Load("Assets/UI/questUI.png");
	eKeyTexture = app->tex->Load("Assets/UI/eKey.png");
	moveTutorialTextutre = app->tex->Load("Assets/UI/Movement_Guide.png");
	interactTutorialTextutre = app->tex->Load("Assets/UI/Interact_Guide.png");

	currentQuestIndex = 0;

	return ret;

}

// Called before the first frame
bool Scene::Start()
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
		basicTutorialCounter = 0;
		isNewGame = false;
	}
	else
	{
		app->LoadGameRequest();
		basicTutorialCounter = 2;
	}

	pauseMenuActive = false;
	exitButtonBool = false;

	// Tell to UIModule which currentMenuType
	app->uiModule->currentMenuType = DISABLED;
	// Call this function only when buttons change
	app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
	
	dialogue.push_back("");

	godMode = false;
	numTimesAVDialogueTriggered = 0;
	numTimesLRRHDialogueTriggered = 0;

	app->audio->PlayMusic("Assets/Sounds/Music/music_firstvillage_tension.wav", 0.2f);

	// Rect for Rope texture
	ropeRect = { 0, 0, 177, 971};
	ropeSpeed = 0.01f;
	ropeJump = 20;
	ropeSpeedLimit = 550;

	ropeX = -app->render->camera.x + 1078;
	ropeY = -app->render->camera.y;

	ropeWin = false;
	minigameActive = false;

	minigameTVdialogueCounter = 0;

	eKeyAnim.Set();
	eKeyAnim.smoothness = 4;
	eKeyAnim.AddTween(100, 50, EXPONENTIAL_OUT);

	inventoryOpen = false;
	
	Quest quest1;
	quest1.completed = false;
	quest1.description = "Leave the forest and find the magic wand";
	questList.push_back(quest1);

	Quest quest2;
	quest2.completed = false;
	quest2.description = "Look for help in the village";
	questList.push_back(quest2);

	Quest quest3;
	quest3.completed = false;
	quest3.description = "Look for the Little Red Riding Hood grandma";
	questList.push_back(quest3);

	Quest quest4;
	quest4.completed = false;
	quest4.description = "Go save LRRH";
	questList.push_back(quest4);

	Quest quest5;
	quest5.completed = false;
	quest5.description = "Get through the portal";
	questList.push_back(quest5);

	secondQuestCollider = app->physics->CreateRectangleSensor(1756 + 443/2, 3968 + 101/2, 443, 101, bodyType::STATIC);
	secondQuestCollider->ctype = ColliderType::SECQUESTCOLLIDER;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}



// Called each loop iteration
bool Scene::Update(float dt)
{

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
	MenuAppear();

	MoveToBattleFromDialogue();

	// Check if the current quest is completed
	if (questList[currentQuestIndex].completed || app->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		// If it is, move to the next quest
		nextQuest();
	}

	// Tutorial SCREENS when NewGame
	if (basicTutorialCounter >= 2)
	{
		// Draw map
		app->map->Draw();
		TweenyTestWithU();

	}
	else
	{
		if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			basicTutorialCounter++;
		}
		player->playerPrevState = player->playerState;
		player->playerState = player->TUTORIAL;

		if (basicTutorialCounter >= 2)
		{
			player->playerPrevState = player->playerState;
			player->playerState = player->MOVING;
		}
		// !!! Tutorial Images drawn in UIModule using basicTtorialCounter variable

	}	

	UpdateMinigameLogic(dt);

	//int mouseX, mouseY;
	//app->input->GetMousePosition(mouseX, mouseY);
	//std::cout << "MouseX: " << mouseX << "MouseY: " << mouseY << std::endl;

	return true;
}

void Scene::MenuAppear()
{
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		// If player is in pause, close it
		if (player->playerState == player->PlayerState::PAUSE)
		{
			player->playerState = player->playerPrevState;

			app->uiModule->currentMenuType = DISABLED;
			// Call this function only when scene is changed
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

			// Max volume
			app->audio->SetMusicVolume(128);

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

			// Mid-Low volume
			app->audio->SetMusicVolume(32);
		}
	}
}

void Scene::TweenyTestWithU()
{
	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		inventoryOpen = !inventoryOpen;
	}

	eKeyAnim.Step(1, false);

	if (inventoryOpen)
	{
		eKeyAnim.Foward();
	}
	else
	{
		eKeyAnim.Backward();
	}

	int offset = 1970;

	float point = eKeyAnim.GetPoint();
	app->render->DrawTexture(eKeyTexture, 1960, offset + point * (400 - 100));
}

//This function swaps after 2sec from Scene to SceneBattle/LRRHCombat If Player has triggered the dialogue with AngryVillager/LRRH and they haven't been defeated before
void Scene::MoveToBattleFromDialogue()
{
	if (numTimesAVDialogueTriggered == 1 && !angryVillagerDefeated)
	{
		timerToSceneBattle.Start(3.0f);
		numTimesAVDialogueTriggered = 0;
	}

	if (timerToSceneBattle.Test() == estadoTimerP::FIN)
	{
		doors.At(0)->data->TriggerDoor(GameScene::BATTLE);
	}

	if (numTimesLRRHDialogueTriggered == 1 && !LRRHDefeated)
	{
		timerToLRRHCombat.Start(3.0f);
		numTimesLRRHDialogueTriggered = 0;
	}

	if (timerToLRRHCombat.Test() == estadoTimerP::FIN)
	{
		doors.At(0)->data->TriggerDoor(GameScene::COMBATLHHR);
	}

}

void Scene::AppearDialogue()
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
bool Scene::PostUpdate()
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

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
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
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	//app->fonts->Unload(font);
	app->map->CleanUp(); 
	app->entityManager->CleanUp(); 
	app->physics->Disable();

	app->tex->UnLoad(npcPopUpTexture);
	app->tex->UnLoad(uiSpriteTexture);
	app->tex->UnLoad(ropeTexture);
	app->tex->UnLoad(pressKeyTexture);
	app->tex->UnLoad(questUiTexture);
	app->tex->UnLoad(moveTutorialTextutre);
	app->tex->UnLoad(interactTutorialTextutre);
	

	return true;
}

//Toggles god mode on and off when the F10 key is pressed. When god mode is on, the camera follows the player's position without any boundaries.
void Scene::GodMode()
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
void Scene::Camera()
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
int GenerateRandomNumber(int num1, int num2)
{
	auto eng = std::default_random_engine(std::time(0));
	std::uniform_int_distribution<int> dist(num1, num2);

	return dist(eng);


}

//Returns a string with the last line spoken by the specified NPC. Randomized.
std::string Scene::LastTextNPC(ColliderType NPC)
{
	std::string auxString;
	int index = GenerateRandomNumber(1, 2);

	switch (NPC)
	{
	case ColliderType::ANGRYVILLAGER:

		switch (index)
		{
		case 1:
			auxString = "What are you waiting?";
			break;
		case 2:
			auxString = "Hmm... I'm hungry";
			break;
		}
		break;
	case ColliderType::TALISMANVILLAGER:

		switch (index)
		{
		case 1:
			auxString = "May God bless you?";
			break;
		case 2:
			auxString = "Look at this beautiful talisman!";
			break;
		}
		break;

	case ColliderType::GRANDMA:
		
		switch (index)
		{
		case 1:
			auxString = "The early bird catches the worm.";
			break;
		case 2:
			auxString = "If you don't have anything nice to say, don't say anything at all.";
			break;
		}

		break;

	case ColliderType::LRRH:
		auxString = "I want to join your group to help you. I realize that I have been acting wrongly. The wolf corrupted me with his magic and made me do terrible things. I thought I was protecting the portal, but in reality, I was hurting others. I cannot justify my actions, but I want to make amends and restore peace to the world of dreams.I want to join your group to fight together against the wolf and stop his evil plan. Together we can do it.";

		break;
	default:

		break;
	}

	return auxString;
}

//Runs a dialogue tree for a specific NPC, identified using a ColliderType enum. This function delegates the NPC specific behavior to other functions based on the enum passed in.
void Scene::RunDialogueTree(ColliderType NPC)
{
	switch (NPC)
	{
	case ColliderType::ANGRYVILLAGER:
		dialogue = angryVillagerTreePT->Run();
		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}

		break;
	case ColliderType::TALISMANVILLAGER:
		
		if (ropeWin)
		{
			talismanVillagerTree->Update(1);
		}

		dialogue = talismanVillagerTree->Run();

		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}

		break;

	case ColliderType::GRANDMA:
		dialogue = grandmaTree->Run();

		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}

		break;

	case ColliderType::LRRH:
		dialogue = littleRedTree->Run();

		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}

		break;
	default:
		break;
	}
}

//Updates the dialogue tree based on the player's response to a dialogue prompt. The dialogue tree to update is selected based on the type of NPC the player last interacted
//with, as determined by the player's lastCollision variable. 
void Scene::UpdateDialogueTree(int option)
{
	if (1 >= option <= 4)
	{
		switch (app->scene->player->lastCollision)
		{
		case ColliderType::ANGRYVILLAGER:
			angryVillagerTreePT->Update(option);
			numTimesAVDialogueTriggered++;
			break;

		case ColliderType::TALISMANVILLAGER:
			talismanVillagerTree->Update(option);
			minigameTVdialogueCounter++;
			break;

		case ColliderType::GRANDMA:
			grandmaTree->Update(option);
			break;

		case ColliderType::LRRH:
			littleRedTree->Update(option);
			numTimesLRRHDialogueTriggered++;
			break;

		default:
			break;
		}
	}



}


//Create Tree Dialogues
void Scene::CreateDialogue()
{
	if (!angryVillagerDefeated)
	{
		// - Angry Villager Pre Tutorial
	//2nd level
		auto AVillagerToOption1 = std::make_shared<DialogueNode>();
		AVillagerToOption1->SetText("Just passing through? You think you can just walk away from this mess? You're as much a part of this as anyone else. I'll make sure you regret your indifference.");

		auto AVillagerToOption2 = std::make_shared<DialogueNode>();
		AVillagerToOption2->SetText("A wand? You think a wand will solve our problems? You're even more clueless than I thought. I'll tell you what you need, a good beating.");

		auto AVillagerToOption3 = std::make_shared<DialogueNode>();
		AVillagerToOption3->SetText("You don't have time for this? You're the one who's wasting our time with your useless questions. I'll teach you a lesson you won't forget.");

		auto AVillagerToOption4 = std::make_shared<DialogueNode>();
		AVillagerToOption4->SetText("Help? You don't need help, you need a good thrashing. You think you can just come here and demand our help? I'll show you what we do with strangers who cause trouble.");


		//1rst level
		auto firstOption1 = std::make_shared<DialogueNode>();
		firstOption1->SetText("I'm just passing through.  What's going on?");
		firstOption1->AddChild(AVillagerToOption1);

		auto firstOption2 = std::make_shared<DialogueNode>();
		firstOption2->SetText("I'm looking for something. Have you seen a wand around here?");
		firstOption2->AddChild(AVillagerToOption2);

		auto firstOption3 = std::make_shared<DialogueNode>();
		firstOption3->SetText("I don't have time for this. Let me in.");
		firstOption3->AddChild(AVillagerToOption3);

		auto firstOption4 = std::make_shared<DialogueNode>();
		firstOption4->SetText("Please, I need your help. I'm lost and scared.");
		firstOption4->AddChild(AVillagerToOption4);


		//Root
		auto firstNodeAngryVillager = std::make_shared<DialogueNode>();
		firstNodeAngryVillager->SetText("What do you want? Can't you see we're in the middle of a crisis here?");
		firstNodeAngryVillager->AddChild(firstOption1);
		firstNodeAngryVillager->AddChild(firstOption2);
		firstNodeAngryVillager->AddChild(firstOption3);
		firstNodeAngryVillager->AddChild(firstOption4);
		firstNodeAngryVillager->ActivateNode();

		//Tree
		angryVillagerTreePT = std::make_shared<DialogueTree>();
		angryVillagerTreePT->SetRoot(firstNodeAngryVillager);

	}
	else
	{
		// - Angry Villager Post Tutorial
		//3rd level
		auto secondOption1PT = std::make_shared<DialogueNode>();
		secondOption1PT->SetText("Nod silently and look concerned. -Leave it to me.-");

		auto secondOption2PT = std::make_shared<DialogueNode>();
		secondOption2PT->SetText("Yawn and stretch, showing disinterest while saying, -Okay...sure.-");

		auto secondOption3PT = std::make_shared<DialogueNode>();
		secondOption3PT->SetText("What a bunch of cowards! You're all hiding in your homes! . I'll take care of her myself.");

		auto secondOption4PT = std::make_shared<DialogueNode>();
		secondOption4PT->SetText("L - Little Red ? Killing animals ? That's terrifying... I-I'll try to talk to her.");

		//2nd level
		auto AVillagerToOptionPT = std::make_shared<DialogueNode>();
		AVillagerToOptionPT->SetText("It all started with Little Red. She's gone mad and started killing the animals in the forest. We've tried talking to her grandmother, but she won't do anything to stop her. Now the villagers are angry and scared, looking for someone to blame. Little Red is dangerous and unpredictable, and we fear for our safety. That's why we're all holed up in our homes");
		AVillagerToOptionPT->AddChild(secondOption1PT);
		AVillagerToOptionPT->AddChild(secondOption2PT);
		AVillagerToOptionPT->AddChild(secondOption3PT);
		AVillagerToOptionPT->AddChild(secondOption4PT);

		//1rst level
		auto firstOption1PT = std::make_shared<DialogueNode>();
		firstOption1PT->SetText("What's going on? Why is everyone hiding in their houses?");
		firstOption1PT->AddChild(AVillagerToOptionPT);

		auto firstOption2PT = std::make_shared<DialogueNode>();
		firstOption2PT->SetText("Sounds like a tough situation. What can I do to assist?");
		firstOption2PT->AddChild(AVillagerToOptionPT);

		auto firstOption3PT = std::make_shared<DialogueNode>();
		firstOption3PT->SetText("Just tell me what I need to know. I'll take care of it myself!");
		firstOption3PT->AddChild(AVillagerToOptionPT);

		auto firstOption4PT = std::make_shared<DialogueNode>();
		firstOption4PT->SetText("I don't really care about what's going on. But I guess I'll have to help, what a drag.");
		firstOption4PT->AddChild(AVillagerToOptionPT);

		//Root
		auto firstNodeAngryVillagerPT = std::make_shared<DialogueNode>();
		firstNodeAngryVillagerPT->SetText("I can't believe it. You beat me fair and square. I guess I underestimated you. Listen, I'm sorry for being so hostile before. It's just that we're all on edge here. The situation is dire.");
		firstNodeAngryVillagerPT->AddChild(firstOption1PT);
		firstNodeAngryVillagerPT->AddChild(firstOption2PT);
		firstNodeAngryVillagerPT->AddChild(firstOption3PT);
		firstNodeAngryVillagerPT->AddChild(firstOption4PT);
		firstNodeAngryVillagerPT->ActivateNode();

		//Tree
		angryVillagerTreePT = std::make_shared<DialogueTree>();
		angryVillagerTreePT->SetRoot(firstNodeAngryVillagerPT);
	}
	


	



	// - Talisman Villager
	//2nd level
	auto tVillagerToOption1 = std::make_shared<DialogueNode>();
	tVillagerToOption1->SetText("Thank you so much for getting my medallion back! You're a lifesaver.");

	auto tVillagerToOption2 = std::make_shared<DialogueNode>();
	tVillagerToOption2->SetText("It's about time. You should have been able to do that faster.");

	auto tVillagerToOption3 = std::make_shared<DialogueNode>();
	tVillagerToOption3->SetText("Well, I suppose you did the bare minimum. But don't expect me to thank you for doing something you should have done in the first place.");

	auto tVillagerToOption4 = std::make_shared<DialogueNode>();
	tVillagerToOption4->SetText("Oh, you actually did it! I didn't think you had it in you. Well done! See? There was nothing to be afraid of. You've got some skills, kid.");


	//1rst level
	auto firstOption1TV = std::make_shared<DialogueNode>();
	firstOption1TV->SetText("Sure thing, I'll get it for you. Don't worry about it.");
	firstOption1TV->AddChild(tVillagerToOption1);

	auto firstOption2TV = std::make_shared<DialogueNode>();
	firstOption2TV->SetText("rolls eyes -Ugh, fine.Just give me a minute.");
	firstOption2TV->AddChild(tVillagerToOption2);

	auto firstOption3TV = std::make_shared<DialogueNode>();
	firstOption3TV->SetText("Don't you watch where you go? I'll help you this time. Take better care of your things.");
	//firstOption3TV->SetText("Why didn't you watch where you were going? You should be more careful with your belongings. I'll help you this time, but you need to take better care of your things.");
	firstOption3TV->AddChild(tVillagerToOption3);

	auto firstOption4TV = std::make_shared<DialogueNode>();
	firstOption4TV->SetText("starts sweating profusely. -I - I don't know if I can do it. What if I mess up?");
	firstOption4TV->AddChild(tVillagerToOption4);


	//Root
	auto firstNodeTalismanVillager = std::make_shared<DialogueNode>();
	firstNodeTalismanVillager->SetText("Oh, hello there. You look like an adventurer. Can you help me out? You see, my family's medallion fell into the well and I can't get it out. It's been passed down for generations and it's the only thing we have left of our family history...");
	firstNodeTalismanVillager->AddChild(firstOption1TV);
	firstNodeTalismanVillager->AddChild(firstOption2TV);
	firstNodeTalismanVillager->AddChild(firstOption3TV);
	firstNodeTalismanVillager->AddChild(firstOption4TV);
	firstNodeTalismanVillager->ActivateNode();


	//Tree
	talismanVillagerTree = std::make_shared<DialogueTree>();
	talismanVillagerTree->SetRoot(firstNodeTalismanVillager);


	if (!LRRHDefeated)
	{
		// - Little Red Riding Hood
		//2nd Level
		auto lRToOption1 = std::make_shared<DialogueNode>();
		lRToOption1->SetText("I don't believe you. Everyone wants to kill me.");

		auto lRToOption2 = std::make_shared<DialogueNode>();
		lRToOption1->SetText("I've faced tougher foes than you. Bring it on");

		auto lRToOption3 = std::make_shared<DialogueNode>();
		lRToOption1->SetText("Wake up, lazybone. This fight won't be a naptime");

		auto lRToOption4 = std::make_shared<DialogueNode>();
		lRToOption1->SetText("Hmph. Don't think your tears will work on me.");


		//1st Level
		auto firstOption1LR = std::make_shared<DialogueNode>();
		firstOption1LR->SetText("I'm not here to hurt you.");
		firstOption1LR->AddChild(lRToOption1);

		auto firstOption2LR = std::make_shared<DialogueNode>();
		firstOption2LR->SetText("Watch your mouth. You think you can take me down?");
		firstOption2LR->AddChild(lRToOption2);

		auto firstOption3LR = std::make_shared<DialogueNode>();
		firstOption3LR->SetText("yawns -Can't someone else deal with this?");
		firstOption3LR->AddChild(lRToOption3);

		auto firstOption4LR = std::make_shared<DialogueNode>();
		firstOption4LR->SetText("P-please don't hurt me.");
		firstOption4LR->AddChild(lRToOption4);


		//Root
		auto firstNodeLR = std::make_shared<DialogueNode>();
		firstNodeLR->SetText("Who...are...you? What...do...you...want? If...you're...here...to...kill...me...just...get...it...over...with. I...won't...go...down...without...a...fight.-");
		firstNodeLR->AddChild(firstOption1LR);
		firstNodeLR->AddChild(firstOption2LR);
		firstNodeLR->AddChild(firstOption3LR);
		firstNodeLR->AddChild(firstOption4LR);
		firstNodeLR->ActivateNode();


		//Tree
		littleRedTree = std::make_shared <DialogueTree>();
		littleRedTree->SetRoot(firstNodeLR);
	}
	else
	{
		//LittleRedAfterCombat
		auto fristNodeLRAC = std::make_shared<DialogueNode>();
		fristNodeLRAC->SetText("I want to join your group to help you. I realize that I have been acting wrongly. The wolf corrupted me with his magic and made me do terrible things. I thought I was protecting the portal, but in reality, I was hurting others. I cannot justify my actions, but I want to make amends and restore peace to the world of dreams.I want to join your group to fight together against the wolf and stop his evil plan. Together we can do it.");
		fristNodeLRAC->ActivateNode();

		littleRedTree = std::make_shared<DialogueTree>();
		littleRedTree->SetRoot(fristNodeLRAC);
	}

	


	// - Grandma

	//3rd Level
	auto secondOption1G = std::make_shared<DialogueNode>();
	secondOption1G->SetText("Thank you for your help. I will head over to your house now.");

	auto secondOption2G = std::make_shared<DialogueNode>();
	secondOption2G->SetText("Whatever, I don’t need your help anymore.");

	auto secondOption3G = std::make_shared<DialogueNode>();
	secondOption3G->SetText("Yeah, sure. Whatever, old lady. Thanks for the info, I guess. I'm outta here. Shrugs and leaves");

	auto secondOption4G = std::make_shared<DialogueNode>();
	secondOption4G->SetText("Thank you so much. I'll make sure to be careful on the way.");

	//2nd Level
	auto gToOption1 = std::make_shared<DialogueNode>();
	gToOption1->SetText("Little Red Riding Hood is my granddaughter, and she's currently in my house. Please be gentle with her, though. She's lost her way and needs our help to find her path back home.");
	gToOption1->AddChild(secondOption1G);

	auto gToOption2 = std::make_shared<DialogueNode>();
	gToOption2->SetText("I will not tolerate that kind of rude behavior. If you have business with Little Red Riding Hood, you will address me with respect. And yes, I know where she is. She is in my house, safe and sound. But I must warn you, if you have any ill intentions towards her, you will answer to me. So please be mindful of her state of mind. She's lost and needs our love and guidance to find her way back on track.");
	gToOption2->AddChild(secondOption2G);

	auto gToOption3 = std::make_shared<DialogueNode>();
	gToOption3->SetText("Little Red Riding Hood is my granddaughter, and she's currently staying in my house. Please be gentle with her, though. She's lost and needs our love and guidance to find her way back on track");
	gToOption3->AddChild(secondOption3G);

	auto gToOption4 = std::make_shared<DialogueNode>();
	gToOption4->SetText("Please don't be afraid, dear. Little Red Riding Hood is in my house. If you come across my granddaughter, show her kindness. She's lost her way and needs our help to find the path back to the light.");
	gToOption4->AddChild(secondOption4G);


	//1st Level
	auto firstOption1G = std::make_shared<DialogueNode>();
	firstOption1G->SetText("I apologize for barging in. I am looking for Little Red Riding Hood. Do you know where she is?");
	firstOption1G->AddChild(gToOption1);

	auto firstOption2G = std::make_shared<DialogueNode>();
	firstOption2G->SetText("Why do you care who I am? I have business with Little Red Riding Hood, I need to find her.");
	firstOption2G->AddChild(gToOption2);

	auto firstOption3G = std::make_shared<DialogueNode>();
	firstOption3G->SetText("Do you know where Little Red Hood is? She's in trouble and stuff. Needs our help or whatever.");
	firstOption3G->AddChild(gToOption3);

	auto firstOption4G = std::make_shared<DialogueNode>();
	firstOption4G->SetText("I'm sorry, I didn't mean to bother you. I heard that Little Red Riding Hood might be in trouble.");
	firstOption4G->AddChild(gToOption4);

	//Root
	auto fristNodeG = std::make_shared<DialogueNode>();
	fristNodeG->SetText("Who are you? And how dare you barge into my home uninvited? I won't tolerate any nonsense or trouble.If you have something to say, say it quicklyand leave.");
	fristNodeG->AddChild(firstOption1G);
	fristNodeG->AddChild(firstOption2G);
	fristNodeG->AddChild(firstOption3G);
	fristNodeG->AddChild(firstOption4G);
	fristNodeG->ActivateNode();

	//Tree
	grandmaTree = std::make_shared<DialogueTree>();
	grandmaTree->SetRoot(fristNodeG);




}

bool Scene::LoadState(pugi::xml_node& data)
{
	loadPlayerPosX = data.child("player").attribute("x").as_int();
	loadPlayerPosY = data.child("player").attribute("y").as_int();

	if (active)
	{
		player->ChangePosition(loadPlayerPosX, loadPlayerPosY);
	}

	pugi::xml_node battleInfo = data.parent().child("BattleInfo");
	angryVillagerDefeated = battleInfo.attribute("isAngryVillagerDefeated").as_bool();
	LRRHDefeated = battleInfo.attribute("isLRRHDefeated").as_bool();

	ropeWin = data.child("rope_minigame").attribute("rope_minigame_state").as_bool();

	LoadChests(data);

	currentQuestIndex = data.child("questIndex").attribute("questIndex").as_int();
	if (currentQuestIndex == 0) secondQuestCollider->body->SetActive(true);

	return true;
}

bool Scene::SaveState(pugi::xml_node& data)
{
	// PLAYER
	pugi::xml_node playerNode = data.append_child("player");

	// If door, save mes lluny
	if (app->uiModule->doorPlayerPosition)
	{
		playerNode.append_attribute("x") = player->position.x;
		playerNode.append_attribute("y") = player->position.y + 75;
		app->uiModule->doorPlayerPosition = false;
	}
	
	if (active)
	{
		playerNode.append_attribute("x") = player->position.x + 16;
		playerNode.append_attribute("y") = player->position.y + 16;
	}
	
	
	// Save Minigame has been Completed
	pugi::xml_node ropeMinigameNode = data.append_child("rope_minigame");
	ropeMinigameNode.append_attribute("rope_minigame_state") = ropeWin;

	// CHESTS
	pugi::xml_node chestGameSave = data.append_child("chests");

	for (int i = 0; i < chests.Count(); i++) {
		Item* chest = chests[i];
		pugi::xml_node chestNode = chestGameSave.append_child("chest");


		chestNode.append_attribute("isPicked").set_value(chest->isPicked);
		// Add code to save other variables of the chest here
	}

	pugi::xml_node questIndexSave = data.append_child("questIndex");

	questIndexSave.append_attribute("questIndex").set_value(currentQuestIndex);
	

	return true;
}

// Code to Load Chests variables, encapsulated. It is called in LoadState() 
void Scene::LoadChests(pugi::xml_node& data)
{
	pugi::xml_node chestsNode = data.child("chests");

	if (!chestsNode)
		return;

	for (pugi::xml_node chestNode : chestsNode.children("chest"))
	{
		int chestIndex = chestNode.attribute("index").as_int();
		bool isPicked = chestNode.attribute("isPicked").as_bool();

		chests[chestIndex]->isPicked = isPicked;

		// Add code to load other variables of the chest here
	}
}


// Every time player presses T, rope goes up.
// Every frame the rope goes down
// The player has to press the button faster proportionally with how much he has left to reach the goal
void Scene::UpdateRopeMinigame(float dt)
{
	if (!ropeWin)
	{
		if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			ropeSpeed += ropeJump;
		}

		// To increase difficulty, change divider to smaller num
		ropeSpeed -= ropeSpeed / 6 * dt;
		if (ropeSpeed <= 0) ropeSpeed = 0.01f;

		// update rope position
		ropeX = -app->render->camera.x + 1078;
		ropeY = -app->render->camera.y - ropeSpeed;

		// If player reaches ropeSpeedLimt, stop the rope and trigger win consequence
		if (ropeSpeed >= ropeSpeedLimit || app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
		{
			ropeSpeed = 0;

			// Win...
			ropeWin = true;
		}
	}
	else
	{
		// update rope position
		ropeX = -app->render->camera.x + 1078;
		// ropeY remains the same
		ropeY = -app->render->camera.y - ropeSpeedLimit;

		// Timer to wait for particles to celebrate win?
		//...

		// Then disable minigame
		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = DIALOG;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
		
	}

	//std::cout << "Rope Speed " << ropeSpeed << std::endl;
}

void Scene::UpdateMinigameLogic(float dt)
{
	if (minigameTVdialogueCounter == 1 && !minigameActive)
	{
		minigameActive = true;
		// Tell to UIModule which currentMenuType
		app->uiModule->currentMenuType = ROPE_MINIGAME;
		// Call this function only when buttons change
		app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);
	}

	if (minigameActive)
	{
		player->movementRestringed = true;
		// While playing the minigame, appear animation of press Key
		// Draw is made on UIModule
		UpdateRopeMinigame(dt);
		pressKeyAnim.Update();
		keyRect = pressKeyAnim.GetCurrentFrame();

		if (ropeWin)
		{
			minigameActive = false;
			// Then disable minigame
			// Tell to UIModule which currentMenuType
			app->uiModule->currentMenuType = DIALOG;
			// Call this function only when buttons change
			app->uiModule->ChangeButtonState(app->uiModule->currentMenuType);

			minigameTVdialogueCounter++;

			app->SaveGameRequest();
		}
	}
}

// A function to draw the current quest on the screen
void Scene::drawQuest(int posX, int posY) {

	questText = questList[currentQuestIndex].description;

	SDL_Rect rect = { 0, 0, 280, 20 };

	SDL_Texture* textDialogue = app->fonts->LoadRenderedParagraph(rect, app->fonts->gameFont, questText, { 0,0,0 }, 280);

	app->render->DrawTexture(textDialogue, posX, posY, &rect);
}

// A function to move to the next quest
void Scene::nextQuest() {
	currentQuestIndex++;
	if (currentQuestIndex >= questList.size()) {
		// If we've reached the end of the quest list, wrap around to the beginning
		currentQuestIndex = 0;
	}
}
