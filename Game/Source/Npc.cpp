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
	switch (app->sceneManager->scene) {
	case SCENE:
		posAngryVillager.x = 7382, posAngryVillager.y = 4934;

		posTalismanVillager.x = 1730, posTalismanVillager.y = 1408;

		posGrandma.x = 7069, posGrandma.y = 2960;

		posLRRH.x = 7224, posLRRH.y = 457;
		break; 
	}
	

	angryVillagerAnimation.PushBack({ 284,55,127,189 });

	talismanVillagerAnimation.PushBack({ 280,511, 122, 187 });

	grandmaAnimation.PushBack({ 12,494,134,191 });

	lrrhAnimation.PushBack({ 25,276,182,190 });

	return true;
}

bool Npc::Start() {
	currentAnimation = nullptr;

	npcTexture = app->tex->Load(npcTexturePath);

	pbodyAVillager = app->physics->CreateRectangle(posAngryVillager.x + 40, posAngryVillager.y, 150, 150, bodyType::STATIC);
	pbodyAVillager->listener = this;				
	pbodyAVillager->ctype = ColliderType::ANGRYVILLAGER;

	pbodyTLVillager = app->physics->CreateRectangle(posTalismanVillager.x, posTalismanVillager.y, 110, 150, bodyType::STATIC);
	pbodyTLVillager->listener = this;				
	pbodyTLVillager->ctype = ColliderType::TALISMANVILLAGER;

	pbodyGrandma = app->physics->CreateRectangle(posGrandma.x+(134/2)-10, posGrandma.y + (191/2), 150, 150, bodyType::STATIC);
	pbodyGrandma->listener = this;				
	pbodyGrandma->ctype = ColliderType::GRANDMA;

	pbodyLRRH = app->physics->CreateRectangle(posLRRH.x+(122/2)-10, posLRRH.y+15, 150, 180, bodyType::STATIC);
	pbodyLRRH->listener = this;
	pbodyLRRH->ctype = ColliderType::LRRH;

	return true;
}

bool Npc::Update(float dt)
{
	currentAnimation = &angryVillagerAnimation;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posAngryVillager.x - 13, posAngryVillager.y - 86, &rect);

	currentAnimation = &talismanVillagerAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posTalismanVillager.x - 60, posTalismanVillager.y - 96, &rect);

	currentAnimation = &grandmaAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posGrandma.x - 13, posGrandma.y - 26, &rect);

	currentAnimation = &lrrhAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcTexture, posLRRH.x - 23, posLRRH.y - 90, &rect);

	return true;
}

bool Npc::PostUpdate()
{
	

	

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
