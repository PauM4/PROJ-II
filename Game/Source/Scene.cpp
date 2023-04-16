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
#include "Item.h"


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
	
	
	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}
	
	//L02: DONE 3: Instantiate the player using the entity manager
	npc1 = (Npc*)app->entityManager->CreateEntity(EntityType::NPC);
	npc1->parameters = config.child("npc");

	item1 = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);

	app->entityManager->Awake(config);

	CreateDialogue(); //3MB

	return ret;
}


void Scene::CreateDialogue()
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
	angryVillagerTree = std::make_shared<DialogueTree>();
	angryVillagerTree->SetRoot(firstNodeAngryVillager);



	// - Angry Villager Post Tutorial
	//3rd level
	auto secondOption1PT = std::make_shared<DialogueNode>();
	secondOption1PT->SetText("Nod silently and look concerned. -Leave it to me.-");

	auto secondOption2PT = std::make_shared<DialogueNode>();
	secondOption2PT->SetText("Yawn and stretch, showing disinterest while saying, -Okay...sure.-");

	auto secondOption3PT = std::make_shared<DialogueNode>();
	secondOption3PT->SetText("What a bunch of cowards! You're all hiding in your homes while someone is out there killing innocent animals. You should be ashamed of yourselves. I'll take care of her myself.");

	auto secondOption4PT = std::make_shared<DialogueNode>();
	secondOption4PT->SetText("Tremble and stutter, - L - Little Red ? Killing animals ? That's terrifying... I-I'll try to talk to her.");

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
	firstOption3PT->SetText("Just tell me what I need to know. I'll take care of it myself since it seems like nobody here can handle it.");
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
	firstOption3TV->SetText("Why didn't you watch where you were going? You should be more careful with your belongings. I'll help you this time, but you need to take better care of your things.");
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
	firstNodeLR->SetText("-Who...are...you?- Little Red's voice is raspy and distorted, as if coming from a deep, dark place. -What...do...you...want?- Her eyes narrow and she grips her longbow tighter, a low growl emanating from her throat. -If...you're...here...to...kill...me...just...get...it...over...with-. Suddenly, a strange, otherworldly sound fills the room, like the creaking of a thousand dead branches. -I...won't...go...down...without...a...fight.-");
	firstNodeLR->AddChild(firstOption1LR);
	firstNodeLR->AddChild(firstOption2LR);
	firstNodeLR->AddChild(firstOption3LR);
	firstNodeLR->AddChild(firstOption4LR);
	firstNodeLR->ActivateNode();


	//Tree
	littleRedTree = std::make_shared <DialogueTree>();
	littleRedTree->SetRoot(firstNodeLR);


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
	gToOption1->SetText("Little Red Riding Hood is my granddaughter, and she's currently in her house. Please be gentle with her, though. She's lost her way and needs our help to find her path back home.");
	gToOption1->AddChild(secondOption1G);

	auto gToOption2 = std::make_shared<DialogueNode>();
	gToOption2->SetText("I will not tolerate that kind of rude behavior in my home. If you have business with Little Red Riding Hood, you will address me with respect. And yes, I know where she is. She is in her house, safe and sound. But I must warn you, if you have any ill intentions towards her, you will answer to me. So please be mindful of her state of mind. She's lost and needs our love and guidance to find her way back on track.");
	gToOption2->AddChild(secondOption2G);

	auto gToOption3 = std::make_shared<DialogueNode>();
	gToOption3->SetText("Little Red Riding Hood is my granddaughter, and she's currently staying in her house. Please be gentle with her, though. She's lost and needs our love and guidance to find her way back on track");
	gToOption3->AddChild(secondOption3G);

	auto gToOption4 = std::make_shared<DialogueNode>();
	gToOption4->SetText("Please don't be afraid, dear. Little Red Riding Hood is in her house. If you come across my granddaughter, show her kindness. She's lost her way and needs our help to find the path back to the light.");
	gToOption4->AddChild(secondOption4G);


	//1st Level
	auto firstOption1G = std::make_shared<DialogueNode>();
	firstOption1G->SetText("I apologize for barging in. I am looking for Little Red Riding Hood. Do you know where she is?");
	firstOption1G->AddChild(gToOption1);

	auto firstOption2G = std::make_shared<DialogueNode>();
	firstOption2G->SetText("Why do you care who I am? I have business with Little Red Riding Hood, and I need to find her now.");
	firstOption2G->AddChild(gToOption2);

	auto firstOption3G = std::make_shared<DialogueNode>();
	firstOption3G->SetText("Uh, sorry, I didn't mean to barge in or anything. I was just looking for someone to help LRRH, you know? She's in trouble and stuff. Needs our help or whatever. yawns");
	firstOption3G->AddChild(gToOption3);

	auto firstOption4G = std::make_shared<DialogueNode>();
	firstOption4G->SetText("Timmy nervously stammers and says: -I'm sorry, I didn't mean to bother you.I'm just a bit lost and scared. I heard that Little Red Riding Hood might be in trouble, and I was hoping you could help me find her. Please, can you tell me where she is?");
	firstOption4G->AddChild(gToOption4);

	//Root
	auto fristNodeG = std::make_shared<DialogueNode>();
	fristNodeG->SetText("Who are you? And how dare you barge into my home uninvited? I won't tolerate any nonsense or trouble.If you have something to say, say it quicklyand leave.");
	fristNodeG->ActivateNode();

	//Tree
	grandmaTree = std::make_shared<DialogueTree>();
	grandmaTree->SetRoot(fristNodeG);


}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	app->entityManager->Start();
	//Fonts initialize
	char lookUpTable[] = { " !�#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[�]^_�abcdefghijklmnopqrstuvwxyz{|}~" };

	font = app->fonts->Load("Assets/Fonts/GameFont.png", lookUpTable, 1);

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

	
	uint w, h;
	app->win->GetWindowSize(w, h);
	button1_continue = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Continue", { (int)w - 1820, (int)h - 300, 100, 20 }, this);
	button1_continue->state = GuiControlState::NONE;
	button2_exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", { (int)w - 1820, (int)h - 250, 100, 20 }, this);
	button2_exit->state = GuiControlState::NONE;

	pauseMenuActive = false;
	exitButtonBool = false;
	
	//dialogue = angryVillagerTreePT->Run(); //dialogo tipo 2
	dialogue = talismanVillagerTree->Run();

	godMode = false;


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
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	// L14: TODO 4: Make the camera movement independent of framerate
	float speed = 0.2 * dt;
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += ceil(speed);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= ceil(speed);

	
	// Menu appear
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		if (player->playerState == player->PlayerState::PAUSE)
		{
			player->playerState = player->playerPrevState;

			button1_continue->state = GuiControlState::NONE;
			button2_exit->state = GuiControlState::NONE;
		}
		else
		{
			// Save previous state to go back
			player->playerPrevState = player->playerState;
			player->playerState = player->PlayerState::PAUSE;
			button1_continue->state = GuiControlState::NORMAL;
			button2_exit->state = GuiControlState::NORMAL;			
		}
	}

	if(player->playerState == player->PAUSE) app->guiManager->Draw();

	GodMode();

	//std::cout << "Screen X: " << app->input->GetScreenMouseX() << std::endl;
	//std::cout << "Screen Y: " << app->input->GetScreenMouseY() << std::endl;

	//std::cout << "World X: " << app->input->GetWorldMouseXRelativeToPlayer(player->position.x) << std::endl;
	//std::cout << "World Y: " << app->input->GetWorldMouseYRelativeToPlayer(player->position.y) << std::endl;

		// Draw map
	app->map->Draw();
	//Font test
	
	return true;
}

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

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;



	//L15: Draw GUI
	app->guiManager->Draw();
	
	// When exit button click, close app
	if (exitButtonBool == true)
	{
		return false;
	}
	//app->fonts->DrawText("NPC1", -20, -90, 100, 100, { 255,255,255,255 }, app->fonts->gameFont);
	//app->fonts->DrawText("ITEM1", 100, -90, 100, 100, { 255,255,255,255 }, app->fonts->gameFont);


	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 Continue click");
		player->playerState = player->playerPrevState;
		break;
	case 2:
		LOG("Button 2 Exit click");
		exitButtonBool = true;
		break;
	}
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	app->fonts->UnLoad(font);
	app->map->CleanUp(); 
	app->entityManager->CleanUp(); 	
	

	return true;
}




std::string Scene::LastTextNPC(ColliderType NPC)
{
	std::string auxString;
	switch (NPC)
	{
	case ColliderType::ANGRYVILLAGER:
		auxString = LastTextAngryVillager(auxString);
		break;
	case ColliderType::TALISMANVILLAGER:
		auxString = LastTextTalismanVillager(auxString);
		break;

	case ColliderType::GRANDMA:
		auxString = LastTextGrandmaVillager(auxString);
		break;

	case ColliderType::LRRH:
		auxString = LastTextLittleRedVillager(auxString);
		break;
	default:

		break;
	}

	return auxString;
}

//Return random number between 2 numbers
int GenerateRandomNumber(int num1, int num2)
{
	auto eng = std::default_random_engine(std::time(0));
	std::uniform_int_distribution<int> dist(num1, num2);

	return dist(eng);


}

std::string Scene::LastTextAngryVillager(std::string lastText)
{
	int index = GenerateRandomNumber(1, 2);

	switch (index)
	{
	case 1:
		lastText = "What are you waiting?";
		break;
	case 2:
		lastText = "Hmm... I'm hungry";
		break;
	}

	return lastText;
}

std::string Scene::LastTextTalismanVillager(std::string lastText)
{
	int index = GenerateRandomNumber(1, 2);

	switch (index)
	{
	case 1:
		lastText = "May God bless you?";
		break;
	case 2:
		lastText = "Look at this beautiful talisman!";
		break;
	}
	return lastText;
}

std::string Scene::LastTextGrandmaVillager(std::string lastText)
{
	int index = GenerateRandomNumber(1, 2);

	switch (index)
	{
	case 1:
		lastText = "The early bird catches the worm.";
		break;
	case 2:
		lastText = "If you don't have anything nice to say, don't say anything at all.";
		break;
	}
	return lastText;
}

std::string Scene::LastTextLittleRedVillager(std::string lastText)
{
	
	int index = GenerateRandomNumber(1, 2);

	switch (index)
	{
	case 1:
		break;
	case 2:
		break;
	}
	return lastText;
}

void Scene::Prueba()
{
	for (auto& e : dialogue)
	{
		std::cout << e << std::endl;
	}
	//	pruebaj++;

}

void Scene::RunDialogueTree(ColliderType NPC)
{
	switch (NPC)
	{
	case ColliderType::ANGRYVILLAGER:
		dialogue = angryVillagerTreePT->Run();
		//Prueba();
		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}
		Prueba();
		break;
	case ColliderType::TALISMANVILLAGER:
		dialogue = talismanVillagerTree->Run();
		//Prueba();
		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}
		Prueba();
		break;

	case ColliderType::GRANDMA:
		dialogue = grandmaTree->Run();
		//Prueba();
		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}
		Prueba();
		break;

	case ColliderType::LRRH:
		dialogue = littleRedTree->Run();
		//Prueba();
		if (dialogue.empty())
		{
			dialogue.push_back(LastTextNPC(NPC));
		}
		Prueba();
		break;
	default:
		break;
	}
}


void Scene::UpdateDialogueTree(int option)
{
	if (1 >= option <= 4)
	{
		switch (app->scene->player->lastCollision)
		{
		case ColliderType::ANGRYVILLAGER:
			angryVillagerTreePT->Update(option);
			break;

		case ColliderType::TALISMANVILLAGER:
			talismanVillagerTree->Update(option);
			break;

		case ColliderType::GRANDMA:
			grandmaTree->Update(option);
			break;

		case ColliderType::LRRH:
			littleRedTree->Update(option);
			break;

		default:
			break;
		}
	}



}