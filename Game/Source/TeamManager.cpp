#include "TeamManager.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Log.h"
#include "Point.h"
#include "EntityManager.h"
#include "Timmy.h"
#include "Bunny.h"
TeamManager::TeamManager(bool isActive) : Module (isActive)
{
	name.Create("teamManager");

}

TeamManager::~TeamManager() {

}

bool TeamManager::Awake(pugi::xml_node& config)
{
	
	//Load from xml
	if (config.parent().child("timmy")) {
		timmy = (Timmy*)app->entityManager->CreateEntity(EntityType::TIMMY);
		timmy->stats = config.parent().child("timmy");
	}
	if (config.parent().child("bunny")) {
		bunny = (Bunny*)app->entityManager->CreateEntity(EntityType::BUNNY);
		bunny->stats = config.parent().child("bunny");
	}
	if (config.parent().child("lrrh")) {
		lrrh = (Lrrh*)app->entityManager->CreateEntity(EntityType::LRRH);
		lrrh->stats = config.parent().child("lrrh");
	}
	if (config.child("playable").attribute("istimmyplayable").as_bool() == true) {
		istimmyplayable = true;
	}
	if (config.child("playable").attribute("isbunnyplayable").as_bool() == true) {
		isbunnyplayable = true;
	}
	if (config.child("playable").attribute("islrrhplayable").as_bool() == true) {
		islrrhplayable = true;
	}
	if (config.child("onteam").attribute("IsTimmyOnTeam").as_bool() == true) {
		IsTimmyOnTeam = true;
	}
	if (config.child("onteam").attribute("IsBunnyOnTeam").as_bool() == true) {
		IsBunnyOnTeam = true;
	}
	if (config.child("onteam").attribute("IsLrrhOnTeam").as_bool() == true) {
		IsLrrhOnTeam = true;
	}

	if (config.child("item").child("yoyo")) {
		pugi::xml_node newnode = config.child("item").child("yoyo");
		yoyo.ininventory = newnode.attribute("ininventory").as_bool();
		yoyo.type = newnode.attribute("type").as_int();
		yoyo.weaponuser = newnode.attribute("weaponuser").as_int();
		yoyo.character = newnode.attribute("character").as_int();
		yoyo.name = newnode.attribute("name").as_string();
		yoyo.defense = newnode.attribute("defense").as_int();
		yoyo.magic = newnode.attribute("magic").as_int();
		yoyo.speed = newnode.attribute("speed").as_int();
		yoyo.movement = newnode.attribute("movement").as_int();
		yoyo.attack = newnode.attribute("attack").as_int();
		yoyo.AttArea = newnode.attribute("AttArea").as_int();
		yoyo.Ab1Power = newnode.attribute("Ab1Power").as_int();
		yoyo.Ab2Power = newnode.attribute("Ab2Power").as_int();
		yoyo.Ab1Area = newnode.attribute("Ab1Area").as_int();
		yoyo.Ab2Area = newnode.attribute("Ab2Area").as_int();
		yoyo.healingpower = newnode.attribute("healingpower").as_int();
	}
	if (config.child("item").child("handsxd")) {
		pugi::xml_node newnode = config.child("item").child("handsxd");
		handsxd.ininventory = newnode.attribute("ininventory").as_bool();
		handsxd.type = newnode.attribute("type").as_int();
		handsxd.weaponuser = newnode.attribute("weaponuser").as_int();
		handsxd.character = newnode.attribute("character").as_int();
		handsxd.name = newnode.attribute("name").as_string();
		handsxd.defense = newnode.attribute("defense").as_int();
		handsxd.magic = newnode.attribute("magic").as_int();
		handsxd.speed = newnode.attribute("speed").as_int();
		handsxd.movement = newnode.attribute("movement").as_int();
		handsxd.attack = newnode.attribute("attack").as_int();
		handsxd.AttArea = newnode.attribute("AttArea").as_int();
		handsxd.Ab1Power = newnode.attribute("Ab1Power").as_int();
		handsxd.Ab2Power = newnode.attribute("Ab2Power").as_int();
		handsxd.Ab1Area = newnode.attribute("Ab1Area").as_int();
		handsxd.Ab2Area = newnode.attribute("Ab2Area").as_int();
		handsxd.healingpower = newnode.attribute("healingpower").as_int();
	}
	if (config.child("item").child("bow")) {
		pugi::xml_node newnode = config.child("item").child("bow");
		bow.ininventory = newnode.attribute("ininventory").as_bool();
		bow.type = newnode.attribute("type").as_int();
		bow.weaponuser = newnode.attribute("weaponuser").as_int();
		bow.character = newnode.attribute("character").as_int();
		bow.name = newnode.attribute("name").as_string();
		bow.defense = newnode.attribute("defense").as_int();
		bow.magic = newnode.attribute("magic").as_int();
		bow.speed = newnode.attribute("speed").as_int();
		bow.movement = newnode.attribute("movement").as_int();
		bow.attack = newnode.attribute("attack").as_int();
		bow.AttArea = newnode.attribute("AttArea").as_int();
		bow.Ab1Power = newnode.attribute("Ab1Power").as_int();
		bow.Ab2Power = newnode.attribute("Ab2Power").as_int();
		bow.Ab1Area = newnode.attribute("Ab1Area").as_int();
		bow.Ab2Area = newnode.attribute("Ab2Area").as_int();
		bow.healingpower = newnode.attribute("healingpower").as_int();
	}
	if (config.child("item").child("club")) {
		pugi::xml_node newnode = config.child("item").child("club");
		club.ininventory = newnode.attribute("ininventory").as_bool();
		club.type = newnode.attribute("type").as_int();
		club.weaponuser = newnode.attribute("weaponuser").as_int();
		club.character = newnode.attribute("character").as_int();
		club.name = newnode.attribute("name").as_string();
		club.defense = newnode.attribute("defense").as_int();
		club.magic = newnode.attribute("magic").as_int();
		club.speed = newnode.attribute("speed").as_int();
		club.movement = newnode.attribute("movement").as_int();
		club.attack = newnode.attribute("attack").as_int();
		club.AttArea = newnode.attribute("AttArea").as_int();
		club.Ab1Power = newnode.attribute("Ab1Power").as_int();
		club.Ab2Power = newnode.attribute("Ab2Power").as_int();
		club.Ab1Area = newnode.attribute("Ab1Area").as_int();
		club.Ab2Area = newnode.attribute("Ab2Area").as_int();
		club.healingpower = newnode.attribute("healingpower").as_int();
	}
	app->entityManager->Awake(config);

	return true;
}

bool TeamManager::Start()
{


	return true;
}

bool TeamManager::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		yoyo.character = 1;
		app->SaveGameRequest();
	}
	timmy->attack;
	return true;

}

bool TeamManager::PostUpdate()
{

	return true;
}

bool TeamManager::CleanUp()
{
	return true;
}
bool TeamManager::LoadState(pugi::xml_node& data)
{
	/*loadPlayerPosX = data.child("player").attribute("x").as_int();
	loadPlayerPosY = data.child("player").attribute("y").as_int();

	player->ChangePosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());*/
	//Load unlocked characters
	istimmyplayable = data.child("playable").attribute("istimmyplayable").as_bool();
	isbunnyplayable = data.child("playable").attribute("isbunnyplayable").as_bool();
	islrrhplayable = data.child("playable").attribute("islrrhplayable").as_bool();
	//Load characters the team
	IsTimmyOnTeam = data.child("onteam").attribute("IsTimmyOnTeam").as_bool();
	IsBunnyOnTeam = data.child("onteam").attribute("IsBunnyOnTeam").as_bool();
	IsLrrhOnTeam = data.child("onteam").attribute("IsLrrhOnTeam").as_bool();
	//Add characters and team to the lists
	characters.Clear();
	team.Clear();
	characters.Add(timmy);
	characters.Add(bunny);
	if (IsTimmyOnTeam == true) {
		team.Add(timmy);
	}
	if (IsBunnyOnTeam == true) {
		team.Add(bunny);
	}
	if (islrrhplayable == true) {
		characters.Add(lrrh);
		if (IsLrrhOnTeam == true) {
			team.Add(lrrh);
		}
	}
	//Delete stats from items
	for (int i = 0; i < equipment.Count(); i++) {

		switch (equipment.At(i)->data.character)
		{
		case 1:
			substractitemstats(timmy, i);
			break;
		case 2:
			substractitemstats(bunny, i);
			break;
		case 3:
			substractitemstats(lrrh, i);
			break;
		case 4:
			//Indev
			break;
		case 5:
			//Indev
			break;
		case 6:
			//Indev
			break;
		}
	}
	//Load inventory
	inventory.Clear();
	equipment.Clear();
	yoyo.ininventory = data.child("inventory").child("yoyo").attribute("isobtained").as_bool();
	yoyo.character = data.child("inventory").child("yoyo").attribute("character").as_int();
	if (yoyo.ininventory == true) {
		inventory.Add(yoyo);
		if (yoyo.character != 0) {
			equipment.Add(yoyo);
		}
	}
	handsxd.ininventory = data.child("inventory").child("handsxd").attribute("isobtained").as_bool();
	handsxd.character = data.child("inventory").child("handsxd").attribute("character").as_int();
	if (handsxd.ininventory == true) {
		inventory.Add(handsxd);
		if (handsxd.character != 0) {
			equipment.Add(handsxd);
		}
	}
	bow.ininventory = data.child("inventory").child("bow").attribute("isobtained").as_bool();
	bow.character = data.child("inventory").child("bow").attribute("character").as_int();
	if (bow.ininventory == true) {
		inventory.Add(bow);
		if (bow.character != 0) {
			equipment.Add(bow);
		}
	}
	club.ininventory = data.child("inventory").child("club").attribute("isobtained").as_bool();
	club.character = data.child("inventory").child("club").attribute("character").as_int();
	if (club.ininventory == true) {
		inventory.Add(club);
		if (club.character != 0) {
			equipment.Add(club);
		}
	}
	//Apply equipped item stats
	for (int i = 0; i < equipment.Count(); i++) {

		switch (equipment.At(i)->data.character)
		{
		case 1:
			additemstats(timmy, i);
			break;
		case 2:
			additemstats(bunny, i);
			break;
		case 3:
			additemstats(lrrh, i);
			break;
		case 4:
			//Indev
			break;
		case 5:
			//Indev
			break;
		case 6:
			//Indev
			break;
		}
		
	}
	statsdone = true;
	return true;
}

bool TeamManager::SaveState(pugi::xml_node& data)
{
	//Save unlocked characters
	pugi::xml_node playable = data.append_child("playable");
	playable.append_attribute("istimmyplayable") = istimmyplayable;
	playable.append_attribute("isbunnyplayable") = isbunnyplayable;
	playable.append_attribute("islrrhplayable") = islrrhplayable;
	
	//Save characters in team
	pugi::xml_node onteam = data.append_child("onteam");
	onteam.append_attribute("IsTimmyOnTeam") = IsTimmyOnTeam;
	onteam.append_attribute("IsBunnyOnTeam") = IsBunnyOnTeam;
	onteam.append_attribute("IsLrrhOnTeam") = IsLrrhOnTeam;

	pugi::xml_node inventory = data.append_child("inventory");
	inventory.append_child("yoyo");
	inventory.child("yoyo").append_attribute("isobtained") = yoyo.ininventory;
	inventory.child("yoyo").append_attribute("character") = yoyo.character;

	inventory.append_child("handsxd");
	inventory.child("handsxd").append_attribute("isobtained") = handsxd.ininventory;
	inventory.child("handsxd").append_attribute("character") = handsxd.character;

	inventory.append_child("bow");
	inventory.child("bow").append_attribute("isobtained") = bow.ininventory;
	inventory.child("bow").append_attribute("character") = bow.character;

	inventory.append_child("club");
	inventory.child("club").append_attribute("isobtained") = club.ininventory;
	inventory.child("club").append_attribute("character") = club.character;

	app->LoadGameRequest();

	return true;
}

bool TeamManager::additemstats(Entity* ent, int i)
{

	ent->defense += equipment.At(i)->data.defense;
	ent->magic += equipment.At(i)->data.magic;
	ent->speed += equipment.At(i)->data.speed;
	ent->movement += equipment.At(i)->data.movement;
	ent->attack += equipment.At(i)->data.attack;
	ent->AttArea += equipment.At(i)->data.AttArea;
	ent->Ab1Power += equipment.At(i)->data.Ab1Power;
	ent->Ab2Power += equipment.At(i)->data.Ab2Power;
	ent->Ab1Area += equipment.At(i)->data.Ab1Area;
	ent->Ab2Area += equipment.At(i)->data.Ab2Area;
	ent->healingpower += equipment.At(i)->data.healingpower;

	return false;
}

bool TeamManager::substractitemstats(Entity* ent, int i)
{
	ent->defense -= equipment.At(i)->data.defense;
	ent->magic -= equipment.At(i)->data.magic;
	ent->speed -= equipment.At(i)->data.speed;
	ent->movement -= equipment.At(i)->data.movement;
	ent->attack -= equipment.At(i)->data.attack;
	ent->AttArea -= equipment.At(i)->data.AttArea;
	ent->Ab1Power -= equipment.At(i)->data.Ab1Power;
	ent->Ab2Power -= equipment.At(i)->data.Ab2Power;
	ent->Ab1Area -= equipment.At(i)->data.Ab1Area;
	ent->Ab2Area -= equipment.At(i)->data.Ab2Area;
	ent->healingpower -= equipment.At(i)->data.healingpower;
	return false;
}
