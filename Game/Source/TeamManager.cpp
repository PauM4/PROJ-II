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
		yoyo.ininventory = config.child("item").child("yoyo").attribute("ininventory").as_bool();
		yoyo.type = config.child("item").child("yoyo").attribute("type").as_int();
		yoyo.weaponuser = config.child("item").child("yoyo").attribute("weaponuser").as_int();
		yoyo.name = config.child("item").child("yoyo").attribute("name").as_string();
		yoyo.defense = config.child("item").child("yoyo").attribute("defense").as_int();
		yoyo.magic = config.child("item").child("yoyo").attribute("magic").as_int();
		yoyo.speed = config.child("item").child("yoyo").attribute("speed").as_int();
		yoyo.movement = config.child("item").child("yoyo").attribute("movement").as_int();
		yoyo.attack = config.child("item").child("yoyo").attribute("attack").as_int();
		yoyo.AttArea = config.child("item").child("yoyo").attribute("AttArea").as_int();
		yoyo.Ab1Power = config.child("item").child("yoyo").attribute("Ab1Power").as_int();
		yoyo.Ab2Power = config.child("item").child("yoyo").attribute("Ab2Power").as_int();
		yoyo.Ab1Area = config.child("item").child("yoyo").attribute("Ab1Area").as_int();
		yoyo.Ab2Area = config.child("item").child("yoyo").attribute("Ab2Area").as_int();
		yoyo.healingpower = config.child("item").child("yoyo").attribute("healingpower").as_int();
	}

	return true;
}

bool TeamManager::Start()
{


	return true;
}

bool TeamManager::Update(float dt)
{

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

	//Load inventory
	yoyo.ininventory = data.child("inventory").attribute("hasyoyo").as_bool();
	if (yoyo.ininventory == true) {
		inventory.Add(yoyo);
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

	pugi::xml_node inventory = data.append_child("inventory");
	inventory.append_attribute("hasyoyo") = yoyo.ininventory;
	//playerNode.append_attribute("x") = player->position.x;
	//playerNode.append_attribute("y") = player->position.y;
	app->LoadGameRequest();


	return true;
}