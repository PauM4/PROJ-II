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
	yoyo.name = "yoyo";

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
	istimmyplayable = data.child("playable").attribute("istimmyplayable");
	isbunnyplayable = data.child("playable").attribute("isbunnyplayable");
	islrrhplayable = data.child("playable").attribute("islrrhplayable");

	IsTimmyOnTeam = data.child("onteam").attribute("IsTimmyOnTeam");
	IsBunnyOnTeam = data.child("onteam").attribute("IsBunnyOnTeam");
	IsLrrhOnTeam = data.child("onteam").attribute("IsLrrhOnTeam");
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
	return true;
}

bool TeamManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node playable = data.append_child("playable");
	// If door, save mes lluny
	playable.append_attribute("istimmyplayable") = istimmyplayable;
	playable.append_attribute("isbunnyplayable") = isbunnyplayable;
	playable.append_attribute("islrrhplayable") = islrrhplayable;
	
	pugi::xml_node onteam = data.append_child("onteam");
	onteam.append_attribute("IsTimmyOnTeam") = IsTimmyOnTeam;
	onteam.append_attribute("IsBunnyOnTeam") = IsBunnyOnTeam;
	onteam.append_attribute("IsLrrhOnTeam") = IsLrrhOnTeam;
	//playerNode.append_attribute("x") = player->position.x;
	//playerNode.append_attribute("y") = player->position.y;



	return true;
}