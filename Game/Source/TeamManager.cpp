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
	characters.Add(timmy);
	characters.Add(bunny);
	team.Add(timmy);
	team.Add(bunny);
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
	istimmy = data.child("teamMembers").attribute("istimmy");
	isbunny = data.child("teamMembers").attribute("isbunny");
	islrrh = data.child("teamMembers").attribute("islrrh");

	return true;
}

bool TeamManager::SaveState(pugi::xml_node& data)
{
	pugi::xml_node teamMembers = data.append_child("teamMembers");
	// If door, save mes lluny
	teamMembers.append_attribute("istimmy") = istimmy;
	teamMembers.append_attribute("isbunny") = isbunny;
	teamMembers.append_attribute("islrrh") = islrrh;

	//playerNode.append_attribute("x") = player->position.x;
	//playerNode.append_attribute("y") = player->position.y;



	return true;
}