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

	
	statslist.Add(&timmystats);
	statslist.Add(&bunnystats);
	statslist.Add(&lrrhstats);
	
	for (int i = 0; i < statslist.Count(); i++) {

		statslist.At(i)->data->character = i+1;
		statslist.At(i)->data->defense = 0;
		statslist.At(i)->data->magic = 0;
		statslist.At(i)->data->speed = 0;
		statslist.At(i)->data->movement = 0;
		statslist.At(i)->data->attack = 0;
		statslist.At(i)->data->AttArea = 0;
		statslist.At(i)->data->Ab1Power = 0;
		statslist.At(i)->data->Ab2Power = 0;
		statslist.At(i)->data->Ab1Area = 0;
		statslist.At(i)->data->Ab2Area = 0;
		statslist.At(i)->data->healingpower = 0;

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

		timmystats.attack = 3;
		bunnystats.magic = 30;
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
	characters.Clear();
	team.Clear();
	inventory.Clear();
	equipment.Clear();
	statslist.Clear();
	return true;
}
bool TeamManager::LoadState(pugi::xml_node& data)
{
	
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
	//Load Stats
	pugi::xml_node statsnode = data.child("stats");
	timmystats.defense = statsnode.child("timmy").attribute("defense").as_int();
	timmystats.magic = statsnode.child("timmy").attribute("magic").as_int();
	timmystats.speed = statsnode.child("timmy").attribute("speed").as_int();
	timmystats.movement = statsnode.child("timmy").attribute("movement").as_int();
	timmystats.attack = statsnode.child("timmy").attribute("attack").as_int();
	timmystats.AttArea = statsnode.child("timmy").attribute("AttArea").as_int();
	timmystats.Ab1Power = statsnode.child("timmy").attribute("Ab1Power").as_int();
	timmystats.Ab2Power = statsnode.child("timmy").attribute("Ab2Power").as_int();
	timmystats.Ab1Area = statsnode.child("timmy").attribute("Ab1Area").as_int();
	timmystats.Ab2Area = statsnode.child("timmy").attribute("Ab2Area").as_int();
	timmystats.healingpower = statsnode.child("timmy").attribute("healingpower").as_int();

	bunnystats.defense = statsnode.child("bunny").attribute("defense").as_int();
	bunnystats.magic = statsnode.child("bunny").attribute("magic").as_int();
	bunnystats.speed = statsnode.child("bunny").attribute("speed").as_int();
	bunnystats.movement = statsnode.child("bunny").attribute("movement").as_int();
	bunnystats.attack = statsnode.child("bunny").attribute("attack").as_int();
	bunnystats.AttArea = statsnode.child("bunny").attribute("AttArea").as_int();
	bunnystats.Ab1Power = statsnode.child("bunny").attribute("Ab1Power").as_int();
	bunnystats.Ab2Power = statsnode.child("bunny").attribute("Ab2Power").as_int();
	bunnystats.Ab1Area = statsnode.child("bunny").attribute("Ab1Area").as_int();
	bunnystats.Ab2Area = statsnode.child("bunny").attribute("Ab2Area").as_int();
	bunnystats.healingpower = statsnode.child("bunny").attribute("healingpower").as_int();

	lrrhstats.defense = statsnode.child("lrrh").attribute("defense").as_int();
	lrrhstats.magic = statsnode.child("lrrh").attribute("magic").as_int();
	lrrhstats.speed = statsnode.child("lrrh").attribute("speed").as_int();
	lrrhstats.movement = statsnode.child("lrrh").attribute("movement").as_int();
	lrrhstats.attack = statsnode.child("lrrh").attribute("attack").as_int();
	lrrhstats.AttArea = statsnode.child("lrrh").attribute("AttArea").as_int();
	lrrhstats.Ab1Power = statsnode.child("lrrh").attribute("Ab1Power").as_int();
	lrrhstats.Ab2Power = statsnode.child("lrrh").attribute("Ab2Power").as_int();
	lrrhstats.Ab1Area = statsnode.child("lrrh").attribute("Ab1Area").as_int();
	lrrhstats.Ab2Area = statsnode.child("lrrh").attribute("Ab2Area").as_int();
	lrrhstats.healingpower = statsnode.child("lrrh").attribute("healingpower").as_int();

	addallstats();

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

	//Save item
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

	pugi::xml_node statsnode = data.append_child("stats");
	statsnode.append_child("timmy");
	statsnode.child("timmy").append_attribute("defense") = timmystats.defense;
	statsnode.child("timmy").append_attribute("magic") = timmystats.magic;
	statsnode.child("timmy").append_attribute("speed") = timmystats.speed;
	statsnode.child("timmy").append_attribute("movement") = timmystats.movement;
	statsnode.child("timmy").append_attribute("attack") = timmystats.attack;
	statsnode.child("timmy").append_attribute("AttArea") = timmystats.AttArea;
	statsnode.child("timmy").append_attribute("Ab1Power") = timmystats.Ab1Power;
	statsnode.child("timmy").append_attribute("Ab2Power") = timmystats.Ab2Power;
	statsnode.child("timmy").append_attribute("Ab1Area") = timmystats.Ab1Area;
	statsnode.child("timmy").append_attribute("Ab2Area") = timmystats.Ab2Area;
	statsnode.child("timmy").append_attribute("healingpower") = timmystats.healingpower;

	statsnode.append_child("bunny");
	statsnode.child("bunny").append_attribute("defense") = bunnystats.defense;
	statsnode.child("bunny").append_attribute("magic") = bunnystats.magic;
	statsnode.child("bunny").append_attribute("speed") = bunnystats.speed;
	statsnode.child("bunny").append_attribute("movement") = bunnystats.movement;
	statsnode.child("bunny").append_attribute("attack") = bunnystats.attack;
	statsnode.child("bunny").append_attribute("AttArea") = bunnystats.AttArea;
	statsnode.child("bunny").append_attribute("Ab1Power") = bunnystats.Ab1Power;
	statsnode.child("bunny").append_attribute("Ab2Power") = bunnystats.Ab2Power;
	statsnode.child("bunny").append_attribute("Ab1Area") = bunnystats.Ab1Area;
	statsnode.child("bunny").append_attribute("Ab2Area") = bunnystats.Ab2Area;
	statsnode.child("bunny").append_attribute("healingpower") = bunnystats.healingpower;

	statsnode.append_child("lrrh");
	statsnode.child("lrrh").append_attribute("defense") = lrrhstats.defense;
	statsnode.child("lrrh").append_attribute("magic") = lrrhstats.magic;
	statsnode.child("lrrh").append_attribute("speed") = lrrhstats.speed;
	statsnode.child("lrrh").append_attribute("movement") = lrrhstats.movement;
	statsnode.child("lrrh").append_attribute("attack") = lrrhstats.attack;
	statsnode.child("lrrh").append_attribute("AttArea") = lrrhstats.AttArea;
	statsnode.child("lrrh").append_attribute("Ab1Power") = lrrhstats.Ab1Power;
	statsnode.child("lrrh").append_attribute("Ab2Power") = lrrhstats.Ab2Power;
	statsnode.child("lrrh").append_attribute("Ab1Area") = lrrhstats.Ab1Area;
	statsnode.child("lrrh").append_attribute("Ab2Area") = lrrhstats.Ab2Area;
	statsnode.child("lrrh").append_attribute("healingpower") = lrrhstats.healingpower;


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

bool TeamManager::addallstats()
{
	
	for (int i = 0; i < characters.Count(); i++) {
		statsdone = false;
		characters.At(i)->data->Awake();
	}
	for (int j = 0; j < statslist.Count(); j++) {

		if (statslist.At(j)->data->character == 1) {
			timmy->defense += statslist.At(j)->data->defense;
			timmy->magic += statslist.At(j)->data->magic;
			timmy->speed += statslist.At(j)->data->speed;
			timmy->movement += statslist.At(j)->data->movement;
			timmy->attack += statslist.At(j)->data->attack;
			timmy->AttArea += statslist.At(j)->data->AttArea;
			timmy->Ab1Power += statslist.At(j)->data->Ab1Power;
			timmy->Ab2Power += statslist.At(j)->data->Ab2Power;
			timmy->Ab1Area += statslist.At(j)->data->Ab1Area;
			timmy->Ab2Area += statslist.At(j)->data->Ab2Area;
			timmy->healingpower += statslist.At(j)->data->healingpower;
		}
		if (statslist.At(j)->data->character == 2) {
			bunny->defense += statslist.At(j)->data->defense;
			bunny->magic += statslist.At(j)->data->magic;
			bunny->speed += statslist.At(j)->data->speed;
			bunny->movement += statslist.At(j)->data->movement;
			bunny->attack += statslist.At(j)->data->attack;
			bunny->AttArea += statslist.At(j)->data->AttArea;
			bunny->Ab1Power += statslist.At(j)->data->Ab1Power;
			bunny->Ab2Power += statslist.At(j)->data->Ab2Power;
			bunny->Ab1Area += statslist.At(j)->data->Ab1Area;
			bunny->Ab2Area += statslist.At(j)->data->Ab2Area;
			bunny->healingpower += statslist.At(j)->data->healingpower;
		}
		if (statslist.At(j)->data->character == 3) {
			lrrh->defense += statslist.At(j)->data->defense;
			lrrh->magic += statslist.At(j)->data->magic;
			lrrh->speed += statslist.At(j)->data->speed;
			lrrh->movement += statslist.At(j)->data->movement;
			lrrh->attack += statslist.At(j)->data->attack;
			lrrh->AttArea += statslist.At(j)->data->AttArea;
			lrrh->Ab1Power += statslist.At(j)->data->Ab1Power;
			lrrh->Ab2Power += statslist.At(j)->data->Ab2Power;
			lrrh->Ab1Area += statslist.At(j)->data->Ab1Area;
			lrrh->Ab2Area += statslist.At(j)->data->Ab2Area;
			lrrh->healingpower += statslist.At(j)->data->healingpower;
		}

	}

	

	return false;
}
