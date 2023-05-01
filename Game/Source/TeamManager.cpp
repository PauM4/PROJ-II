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
	app->entityManager->Awake(config);

	return true;
}

bool TeamManager::Start()
{


	return true;
}

bool TeamManager::Update(float dt)
{

	timmy->attack;
	bunny->magic;
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

		if (equipment.At(i)->data.character == 1) {

			timmy->defense -= equipment.At(i)->data.defense;
			timmy->magic -= equipment.At(i)->data.magic;
			timmy->speed -= equipment.At(i)->data.speed;
			timmy->movement -= equipment.At(i)->data.movement;
			timmy->attack -= equipment.At(i)->data.attack;
			timmy->AttArea -= equipment.At(i)->data.AttArea;
			timmy->Ab1Power -= equipment.At(i)->data.Ab1Power;
			timmy->Ab2Power -= equipment.At(i)->data.Ab2Power;
			timmy->Ab1Area -= equipment.At(i)->data.Ab1Area;
			timmy->Ab2Area -= equipment.At(i)->data.Ab2Area;
			timmy->healingpower -= equipment.At(i)->data.healingpower;
		}

		if (equipment.At(i)->data.character == 2) {

			bunny->defense -= equipment.At(i)->data.defense;
			bunny->magic -= equipment.At(i)->data.magic;
			bunny->speed -= equipment.At(i)->data.speed;
			bunny->movement -= equipment.At(i)->data.movement;
			bunny->attack -= equipment.At(i)->data.attack;
			bunny->AttArea -= equipment.At(i)->data.AttArea;
			bunny->Ab1Power -= equipment.At(i)->data.Ab1Power;
			bunny->Ab2Power -= equipment.At(i)->data.Ab2Power;
			bunny->Ab1Area -= equipment.At(i)->data.Ab1Area;
			bunny->Ab2Area -= equipment.At(i)->data.Ab2Area;
			bunny->healingpower -= equipment.At(i)->data.healingpower;
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

	//Apply equipped item stats
	for (int i = 0; i < equipment.Count(); i++) {

		if (equipment.At(i)->data.character == 1) {

			timmy->defense += equipment.At(i)->data.defense;
			timmy->magic += equipment.At(i)->data.magic;
			timmy->speed += equipment.At(i)->data.speed;
			timmy->movement += equipment.At(i)->data.movement;
			timmy->attack += equipment.At(i)->data.attack;
			timmy->AttArea += equipment.At(i)->data.AttArea;
			timmy->Ab1Power += equipment.At(i)->data.Ab1Power;
			timmy->Ab2Power += equipment.At(i)->data.Ab2Power;
			timmy->Ab1Area += equipment.At(i)->data.Ab1Area;
			timmy->Ab2Area += equipment.At(i)->data.Ab2Area;
			timmy->healingpower += equipment.At(i)->data.healingpower;
		}

		if (equipment.At(i)->data.character == 2) {

			bunny->defense += equipment.At(i)->data.defense;
			bunny->magic += equipment.At(i)->data.magic;
			bunny->speed += equipment.At(i)->data.speed;
			bunny->movement += equipment.At(i)->data.movement;
			bunny->attack += equipment.At(i)->data.attack;
			bunny->AttArea += equipment.At(i)->data.AttArea;
			bunny->Ab1Power += equipment.At(i)->data.Ab1Power;
			bunny->Ab2Power += equipment.At(i)->data.Ab2Power;
			bunny->Ab1Area += equipment.At(i)->data.Ab1Area;
			bunny->Ab2Area += equipment.At(i)->data.Ab2Area;
			bunny->healingpower += equipment.At(i)->data.healingpower;
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

	app->LoadGameRequest();

	return true;
}