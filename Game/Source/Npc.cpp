#include "Npc.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Npc::Npc() : Entity(EntityType::NPC)
{
	name.Create("Npc");
}

Npc::~Npc() {

}

bool Npc::Awake() {

	npcTexturePath = parameters.attribute("npcTexturePath").as_string();

	posAngryVillager.x = 1107, posAngryVillager.y = 1843;

	posTalismanVillager.x = 1584, posTalismanVillager.y = -380;

	posGrandma.x = 1667, posGrandma.y = -978;

	posLRRH.x = 1595, posLRRH.y = -162;

	angryVillagerAnimation.PushBack({ 586,44,105,174 });

	talismanVillagerAnimation.PushBack({ 586,243, 112, 203 });

	grandmaAnimation.PushBack({ 13,471,123,185 });

	lrrhAnimation.PushBack({ 17,276,122,179 });


	return true;
}

bool Npc::Start() {
	currentAnimation = nullptr;

	npcTexture = app->tex->Load(npcTexturePath);

	pbodyAVillager = app->physics->CreateRectangleSensor(posAngryVillager.x, posAngryVillager.y, 130, 150, bodyType::STATIC);
	pbodyAVillager->listener = this;				
	pbodyAVillager->ctype = ColliderType::ANGRYVILLAGER;

	pbodyTLVillager = app->physics->CreateRectangleSensor(posTalismanVillager.x, posTalismanVillager.y, 130, 150, bodyType::STATIC);
	pbodyTLVillager->listener = this;				
	pbodyTLVillager->ctype = ColliderType::TALISMANVILLAGER;

	pbodyGrandma = app->physics->CreateRectangleSensor(posGrandma.x, posGrandma.y, 130, 150, bodyType::STATIC);
	pbodyGrandma->listener = this;				
	pbodyGrandma->ctype = ColliderType::GRANDMA;

	pbodyLRRH = app->physics->CreateRectangleSensor(posLRRH.x, posLRRH.y, 130, 150, bodyType::STATIC);
	pbodyLRRH->listener = this;
	pbodyLRRH->ctype = ColliderType::LRRH;

	return true;
}

bool Npc::Update()
{

	return true;
}

bool Npc::PostUpdate()
{
	currentAnimation = &angryVillagerAnimation;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posAngryVillager.x - 13, posAngryVillager.y - 26, &rect);

	currentAnimation = &talismanVillagerAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posTalismanVillager.x - 13, posTalismanVillager.y - 26, &rect);

	currentAnimation = &grandmaAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posGrandma.x - 13, posGrandma.y - 26, &rect);

	currentAnimation = &lrrhAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posLRRH.x - 13, posLRRH.y - 26, &rect);

	

	return true;
}

bool Npc::CleanUp()
{
	app->tex->UnLoad(npcTexture);

	if (pbodyAVillager != NULL)
	{
		pbodyAVillager->body->GetWorld()->DestroyBody(pbodyAVillager->body);
	}
	if (pbodyTLVillager != NULL)
	{
		pbodyTLVillager->body->GetWorld()->DestroyBody(pbodyTLVillager->body);
	}
	if (pbodyGrandma != NULL)
	{
		pbodyGrandma->body->GetWorld()->DestroyBody(pbodyGrandma->body);
	}
	if (pbodyLRRH != NULL)
	{
		pbodyLRRH->body->GetWorld()->DestroyBody(pbodyLRRH->body);
	}

	return true;
}

void Npc::OnCollision(PhysBody* physA, PhysBody* physB)
{

}
