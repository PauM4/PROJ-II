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

	// Initialize Npc parameters
	position.x = 0;
	position.y = 0;

	return true;
}

bool Npc::Start() { //128 // 128
	pbodyAVillager = app->physics->CreateRectangleSensor(1107, 1843, 130, 150, bodyType::STATIC);
	pbodyAVillager->listener = this;				
	pbodyAVillager->ctype = ColliderType::ANGRYVILLAGER;

	pbodyTLVillager = app->physics->CreateRectangleSensor(1584, -380, 130, 150, bodyType::STATIC);
	pbodyTLVillager->listener = this;				
	pbodyTLVillager->ctype = ColliderType::TALISMANVILLAGER;

	pbodyGrandma = app->physics->CreateRectangleSensor(1667, -978, 130, 150, bodyType::STATIC);
	pbodyGrandma->listener = this;				
	pbodyGrandma->ctype = ColliderType::GRANDMA;

	pbodyLRRH = app->physics->CreateRectangleSensor(1595, -162, 130, 150, bodyType::STATIC);
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
	return true;
}

bool Npc::CleanUp()
{
	app->tex->UnLoad(texture);

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
