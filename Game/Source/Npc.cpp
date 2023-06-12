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

	npcTexturePathAngryVillager = "Assets/Characters/F_sprites_angry_Villager.png";
	npcTexturePathTalismanVillager = "Assets/Characters/F_idle_pozo.png";
	npcTexturePathLRRH = "Assets/Characters/F_sprites_lrrh.png";
	npcTexturePathGrandma = "Assets/Characters/F_idle_abuela.png";

	posAngryVillager.x = 7382, posAngryVillager.y = 4934;

	posTalismanVillager.x = 1730, posTalismanVillager.y = 1408;

	posGrandma.x = 7069, posGrandma.y = 2960;

	posLRRH.x = 7224, posLRRH.y = 457;
	
	

	angryVillagerAnimation.PushBack({ 0,0,150,150 });

	talismanVillagerAnimation.PushBack({ 0,0,170,170 });

	grandmaAnimation.PushBack({ 0,0,170,170 });

	lrrhAnimation.PushBack({ 0,0,150,150 });

	return true;
}

bool Npc::Start() {
	currentAnimation = nullptr;

	npcTextureAngryVillager = app->tex->Load(npcTexturePathAngryVillager);

	npcTextureGrandma = app->tex->Load(npcTexturePathGrandma);

	npcTextureTalismanVillager = app->tex->Load(npcTexturePathTalismanVillager);

	npcTextureLRRH = app->tex->Load(npcTexturePathLRRH);

	pbodyAVillager = app->physics->CreateRectangle(posAngryVillager.x + 40, posAngryVillager.y, 150, 150, bodyType::STATIC);
	pbodyAVillager->listener = this;				
	pbodyAVillager->ctype = ColliderType::ANGRYVILLAGER;

	pbodyTLVillager = app->physics->CreateRectangle(posTalismanVillager.x + 30, posTalismanVillager.y, 110, 150, bodyType::STATIC);
	pbodyTLVillager->listener = this;				
	pbodyTLVillager->ctype = ColliderType::TALISMANVILLAGER;

	pbodyGrandma = app->physics->CreateRectangle(posGrandma.x+(134/2) + 10, posGrandma.y + (191/2), 150, 150, bodyType::STATIC);
	pbodyGrandma->listener = this;				
	pbodyGrandma->ctype = ColliderType::GRANDMA;

	pbodyLRRH = app->physics->CreateRectangle(posLRRH.x+(122/2), posLRRH.y - 10, 110, 110, bodyType::STATIC);
	pbodyLRRH->listener = this;
	pbodyLRRH->ctype = ColliderType::LRRH;

	return true;
}

bool Npc::Update(float dt)
{
	currentAnimation = &angryVillagerAnimation;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTextureAngryVillager, posAngryVillager.x - 13, posAngryVillager.y - 86, &rect);

	currentAnimation = &talismanVillagerAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTextureTalismanVillager, posTalismanVillager.x - 60, posTalismanVillager.y - 96, &rect);

	currentAnimation = &grandmaAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTextureGrandma, posGrandma.x - 13, posGrandma.y - 26, &rect);

	currentAnimation = &lrrhAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTextureLRRH, posLRRH.x - 23, posLRRH.y - 90, &rect);

	return true;
}

bool Npc::PostUpdate()
{
	

	

	return true;
}

bool Npc::CleanUp()
{
	app->tex->UnLoad(npcTextureAngryVillager);
	app->tex->UnLoad(npcTextureGrandma);
	app->tex->UnLoad(npcTextureLRRH);
	app->tex->UnLoad(npcTextureAngryVillager);

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
