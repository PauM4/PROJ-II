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

	return ret;
}

void SceneBattle::CreateDialogue()
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


	//Tree
	auto angryVillagerTree = std::make_shared<DialogueTree>();
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
	firstOption1->SetText("What's going on? Why is everyone hiding in their houses?");
	firstOption1->AddChild(AVillagerToOptionPT);

	auto firstOption2PT = std::make_shared<DialogueNode>();
	firstOption2->SetText("Sounds like a tough situation. What can I do to assist?");
	firstOption2->AddChild(AVillagerToOptionPT);

	auto firstOption3PT = std::make_shared<DialogueNode>();
	firstOption3->SetText("Just tell me what I need to know. I'll take care of it myself since it seems like nobody here can handle it.");
	firstOption3->AddChild(AVillagerToOptionPT);

	auto firstOption4PT = std::make_shared<DialogueNode>();
	firstOption4->SetText("I don't really care about what's going on. But I guess I'll have to help, what a drag.");
	firstOption4->AddChild(AVillagerToOptionPT);

	//Root
	auto firstNodeAngryVillagerPT = std::make_shared<DialogueNode>();
	firstNodeAngryVillagerPT->SetText("I can't believe it. You beat me fair and square. I guess I underestimated you. Listen, I'm sorry for being so hostile before. It's just that we're all on edge here. The situation is dire.");
	firstNodeAngryVillagerPT->AddChild(firstOption1PT);
	firstNodeAngryVillagerPT->AddChild(firstOption2PT);
	firstNodeAngryVillagerPT->AddChild(firstOption3PT);
	firstNodeAngryVillagerPT->AddChild(firstOption4PT);

	//Tree
	auto angryVillagerTreePT = std::make_shared<DialogueTree>();
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


	//Tree
	auto talismanVillagerTree = std::make_shared<DialogueTree>();
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


	//Tree
	auto littleRedTree = std::make_shared <DialogueTree>();
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

	//Tree
	auto grandmaTree = std::make_shared<DialogueTree>();
	grandmaTree->SetRoot(fristNodeG);


}

// Called before the first frame
bool SceneBattle::Start()
{
	//Load map
	bool retLoad = app->map->Load(mapName, mapFolder);

	pathIndex = 1;
	turnstart = false;
	pathIndex = 0;
	
	//app->physics->Enable();
	//Load combat map
	/*MakeCombatMap();*/

	//Buttons
	uint w, h;
	app->win->GetWindowSize(w, h);
	button1_attack = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "Attack", { (int)w - 1820, (int)h - 300, 100, 20 }, this);
	button1_attack->state = GuiControlState::NORMAL;
	button2_skill = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "Skill", { (int)w - 1820, (int)h - 250, 100, 20 }, this);
	button2_skill->state = GuiControlState::NORMAL;
	button3_endTurn = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "End Turn", { (int)w - 1820, (int)h - 250, 100, 20 }, this);
	button3_endTurn->state = GuiControlState::NORMAL;

	if (retLoad) {
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);

	}


	mouseTileTex = app->tex->Load("Assets/Maps/Scenes/Path.png");

	originTex = app->tex->Load("Assets/Maps/Scenes/Cruz.png");

	MakeCombatMap();

	pbody = app->physics->CreateRectangle(670, 420, 50, 50, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = timmy;

	

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
	if (turnstart == true) {
		//if user selects attack
		CreateArea(characterTurn->AttArea, 0);
		Combat(characterTurn, targets, 1);
		turnstart = false;
		//if user selects ab1
		CreateArea(characterTurn->Ab1Area, characterTurn->Ab1RangeType);
		Combat(characterTurn, targets, 2);
		turnstart = false;

		//if user selects ab2
		CreateArea(characterTurn->Ab2Area, characterTurn->Ab2RangeType);
		Combat(characterTurn, targets, 3);
		turnstart = false;
	}

	bool ret = true;

	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->map->Draw();

	return true;
}

// Called each loop iteration
bool SceneBattle::PostUpdate()
{
	bool ret = true;

	b2Vec2 vel = b2Vec2(0, 0);

	app->guiManager->Draw();

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	//	ret = false;

	////if (!DisplayArea()) ret = false;

	

	//
	iPoint posTile = iPoint(0, 0);
	posTile = app->map->WorldToMap(pos.x - app->render->camera.x , pos.y - app->render->camera.y);
	
	
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x,mouseY - app->render->camera.y);
	/*LOG("%d %d", mouseTile.x-4, mouseTile.y-1);*/
	iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
	if (app->pathfinding->IsWalkable(mouseTile) && combatMap[mouseTile.x-4][mouseTile.y].character!=false) {
		app->render->DrawRectangle({ highlightedTileWorld.x, highlightedTileWorld.y, 120, 120 }, 0, 143, 57, 100, true);
		/*app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);*/
	}

	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		
			if (originSelected == true)
			{
				if (app->pathfinding->IsWalkable(origin)) {
					length=app->pathfinding->CreatePath(origin, mouseTile);
					destination.x = mouseTile.x;
					destination.y = mouseTile.y;
					originSelected = false;
				}
				else {
					app->pathfinding->ClearLastPath();
				}
			}
			else if(combatMap[mouseTile.x - 4][ mouseTile.y ].character != false)
			{
					origin = mouseTile;
					if (app->pathfinding->IsWalkable(origin)) {
						originSelected = true;
						
					}
					app->pathfinding->ClearLastPath();
				
			}
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
	if (app->pathfinding->IsWalkable(origin)) {

	  app->render->DrawRectangle({ originScreen.x, originScreen.y, 120, 120 }, 250, 0, 0, 100, true);
	  app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	}


	//int j = 0;
	//for (int i = 0; i < path.Count(); i++)
	//{
	//	if (path[i] != (*lastpath)[j])
	//		
	//	j++;
	//	
	//}
	//path.Clear();
	//for (int i = 0; i < lastpath->Count(); i++)
	//{
	//	path.PushBack((*lastpath)[i]);
	//}
	//if (j < path.Count())
	//	pathIndex = j;
	//else
	//	pathIndex = 0;


	/*fPoint pixelPosition;
	fPoint finalPosition;
	float distance;

	
		
	
	pixelPosition.x = lastpath->At(pathIndex)->x * app->map->mapData.tileWidth;
	pixelPosition.y = lastpath->At(pathIndex)->y * app->map->mapData.tileHeight;

	finalPosition.x= lastpath->At(length-1)->x * app->map->mapData.tileWidth;
	finalPosition.x = lastpath->At(length - 1)->x * app->map->mapData.tileWidth;
	if (finalPosition.DistanceTo(pos) == 0) {
		pathIndex = 0;
	}

	distance = pixelPosition.DistanceTo(pos);



	if (distance == 0)
	{
		pathIndex++;
		vel.y = 0;
		vel.x = 0;
	}
	else
	{
		float xDiff = pixelPosition.x - pos.x;
		float yDiff = pixelPosition.y - pos.y;

	


		if (abs(xDiff) > abs(yDiff))
		{
			int xDir = (xDiff > 0) ? 1 : -1;
			if (abs(xDiff) < abs(xDir * 10))
			{
				vel.x += xDiff;
				vel.y = 0;
			}
			else
			{
				vel.x = 10 * xDir;
				vel.y = 0;
			}
		}
		else
		{
			int yDir = (yDiff > 0) ? 1 : -1;
			if (abs(yDiff) < abs(yDir *  10))
			{
				vel.y += yDiff;
				vel.x = 0;
			}
			else
			{
				vel.y += yDir * 10;
				vel.x = 0;
			}

		}
	}*/

	


	if (pathIndex != length) {
		length = app->pathfinding->CreatePath(origin, destination);
	}
	else {
		length = 1;
		pathIndex = 1;
		app->pathfinding->ClearLastPath();
		
	}
	iPoint dist;
	iPoint finaldist;
	LOG("length= %d", length);
	LOG(" posTileX: %d", posTile.x);
	LOG(" posTiley: %d", posTile.y);
	if (length > 1) {
		
		lastpath = app->pathfinding->GetLastPath();
		nextpos.x = lastpath->At(pathIndex)->x;
		nextpos.y = lastpath->At(pathIndex)->y;
		LOG(" NextposX: %d", nextpos.x);
		LOG(" NextposY: %d", nextpos.y);
		
		dist.x = nextpos.x - posTile.x;
		LOG(" disX: %d", dist.x);
		dist.y = nextpos.y - posTile.y;
		LOG(" disY: %d", dist.y);


	
		int xDir = (dist.x > 0) ? 1 : -1;
		int yDir = (dist.y > 0) ? 1 : -1;
		
		if (dist.x==0 && dist.y ==0) {
			pathIndex++;
			
		}
		else if (abs(dist.x) > 0) {
			vel.x = 10*xDir;
			
		}
	/*	else if (dist.x < 0) {
			vel.x = -10;
			
		}*/
		else if (abs(dist.y) > 0) {
			vel.y = 10*yDir;
			
		}
		else {


		}
	/*	else if (dist.y < 0) {
			vel.y = -10;
			
		}*/

		/*if(pathIndex == length){ 
			pathIndex = 0; app->pathfinding->ClearLastPath();
			length = 0;
		}*/
	}





	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 9; j++) {

			combatMap[i][j].x = i;
			combatMap[i][j].y = j;
			combatMap[i][j].character = false;
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	pbody->body->SetLinearVelocity(vel);

	pos.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	pos.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	

	/*LOG("%d %d", posTile.x-4, posTile.y);*/
	combatMap[posTile.x-4][ posTile.y ].character = true;
	
	app->render->DrawRectangle({ int(pos.x)-25, int(pos.y)-25, 50, 50 }, 250, 0, 0, 250, true);

	

	return ret;
}

bool SceneBattle::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 Attack click");
		
		break;
	case 2:
		LOG("Button 2 Skill click");
		
		break;
	case 3:
		LOG("Button 3 End Turn click");

		break;
	}
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
			combatMap[i][j].type = (TILE_TYPE)app->map->metadataLayer[i][j];
		}
	}

	return ret;
}

List<TileData*> SceneBattle::CreateArea(int range, int type) {

	List<TileData*> area;
	
	switch (type) {

	case 0:
		//attack
	case 1:
		//lineal
	case 2:
		//circular
	case 3:
		//global
		for (int i = 0; i < COMBAT_MAP_HEIGHT; i++) {
			for (int j = 0; j < COMBAT_MAP_WIDTH; j++) {
				if (combatMap[j][i].type != TILE_TYPE::BARRIER && combatMap[j][i].type != TILE_TYPE::HALF_BARRIER) {
				
					area.Add(&combatMap[j][i]);

				}
			}
		}
	}

	return area;

}

bool SceneBattle::DisplayArea(List<TileData*> area, int type) {

	bool ret = true;

	ListItem<TileData*>* tileListItem;
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
		app->render->DrawRectangle({ tileListItem->data->x,tileListItem->data->y,app->map->mapData.tileWidth,app->map->mapData.tileHeight }, color[0], color[1], color[2], 100);

		tileListItem = tileListItem->next;
	}

	return ret;
}

// Starts combat, id=1 --> attack, id=2 --> ability 1, id=3 --> ability 2
bool SceneBattle::Combat(Entity* inturn, List<Entity*> target, int id) {
	
	bool ret = true;

	//id = 1 --> attack
	if (id == 1) {
		for (int i = 0; i++; i > target.Count()) {
			target.At(i)->data->TakeDamage(inturn->Attack());
		}
	}
	//id = 2 --> ability 1
	if (id == 2) {
		for (int i = 0; i++; i > target.Count()) {
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
		for (int i = 0; i++; i > target.Count()) {
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

//Called before quitting
bool SceneBattle::CleanUp()
{
	LOG("Freeing sceneBattle");
	app->map->CleanUp(); 
	//app->physics->Disable();
	return true;
}
