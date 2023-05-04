#include "NpcW3.h" 
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

NpcW3::NpcW3() : Entity(EntityType::NPC)
{
	name.Create("NpcW3");
}

NpcW3::~NpcW3() {

}

bool NpcW3::Awake() {

	NpcW3TexturePath = "Assets/Characters/Characters_popups.png";

	posDeadVillager.x = 400, posDeadVillager.y = 1500;
	deadVillagerAnimation.PushBack({ 284,55,127,189 });

	posWolf.x = 600, posWolf.y = 1500;
	wolfAnimation.PushBack({ 284,55,127,189 });

	posPedro.x = 800, posPedro.y = 1500;
	pedroAnimation.PushBack({ 284,55,127,189 });

	posSheepA.x = 1000, posSheepA.y = 1500;
	sheepAAnimation.PushBack({ 284,55,127,189 });

	posSheepB.x = 1200, posSheepB.y = 1500;
	sheepBAnimation.PushBack({ 284,55,127,189 });

	posSheepC.x = 1400, posSheepC.y = 1500;
	sheepCAnimation.PushBack({ 284,55,127,189 });

	posSheepD.x = 1600, posSheepD.y = 1500;
	sheepDAnimation.PushBack({ 284,55,127,189 });

	return true;
}

bool NpcW3::Start() {
	currentAnimation = nullptr;

	NpcW3Texture = app->tex->Load(NpcW3TexturePath);

	pbodyDeadVillager = app->physics->CreateRectangle(posDeadVillager.x + 40, posDeadVillager.y, 150, 150, bodyType::STATIC);
	pbodyDeadVillager->listener = this;
	pbodyDeadVillager->ctype = ColliderType::DEADVILLAGER;

	pbodyPedro = app->physics->CreateRectangle(posPedro.x + 40, posPedro.y, 150, 150, bodyType::STATIC);
	pbodyPedro->listener = this;
	pbodyPedro->ctype = ColliderType::PEDRO;

	pbodyWolf = app->physics->CreateRectangle(posWolf.x + 40, posWolf.y, 150, 150, bodyType::STATIC);
	pbodyWolf->listener = this;
	pbodyWolf->ctype = ColliderType::WOLF;

	pbodySheepA = app->physics->CreateRectangle(posSheepA.x + 40, posSheepA.y, 150, 150, bodyType::STATIC);
	pbodySheepA->listener = this;
	pbodySheepA->ctype = ColliderType::SHEEPA;

	pbodySheepB = app->physics->CreateRectangle(posSheepB.x + 40, posSheepB.y, 150, 150, bodyType::STATIC);
	pbodySheepB->listener = this;
	pbodySheepB->ctype = ColliderType::SHEEPB;

	pbodySheepC = app->physics->CreateRectangle(posSheepC.x + 40, posSheepC.y, 150, 150, bodyType::STATIC);
	pbodySheepC->listener = this;
	pbodySheepC->ctype = ColliderType::SHEEPC;

	pbodySheepD = app->physics->CreateRectangle(posSheepD.x + 40, posSheepD.y, 150, 150, bodyType::STATIC);
	pbodySheepD->listener = this;
	pbodySheepD->ctype = ColliderType::SHEEPD;


	return true;
}

bool NpcW3::Update(float dt)
{

	return true;
}

bool NpcW3::PostUpdate()
{
	currentAnimation = &deadVillagerAnimation;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posDeadVillager.x - 13, posDeadVillager.y - 86, &rect);

	currentAnimation = &wolfAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posWolf.x - 60, posWolf.y - 96, &rect);

	currentAnimation = &pedroAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posPedro.x - 60, posPedro.y - 96, &rect);


	currentAnimation = &sheepAAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posSheepA.x - 60, posSheepA.y - 96, &rect);

	currentAnimation = &sheepBAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posSheepB.x - 60, posSheepB.y - 96, &rect);

	currentAnimation = &sheepCAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posSheepC.x - 60, posSheepC.y - 96, &rect);

	currentAnimation = &sheepDAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posSheepD.x - 60, posSheepD.y - 96, &rect);


	return true;
}

bool NpcW3::CleanUp()
{
	app->tex->UnLoad(NpcW3Texture);

	if (pbodyDeadVillager != NULL)
	{
		pbodyDeadVillager->body->GetWorld()->DestroyBody(pbodyDeadVillager->body);
	}
	if (pbodyWolf != NULL)
	{
		pbodyWolf->body->GetWorld()->DestroyBody(pbodyWolf->body);
	}
	if (pbodyPedro != NULL)
	{
		pbodyPedro->body->GetWorld()->DestroyBody(pbodyPedro->body);
	}
	if (pbodySheepA != NULL)
	{
		pbodySheepA->body->GetWorld()->DestroyBody(pbodySheepA->body);
	}
	if (pbodySheepB != NULL)
	{
		pbodySheepB->body->GetWorld()->DestroyBody(pbodySheepB->body);
	}
	if (pbodySheepC != NULL)
	{
		pbodySheepC->body->GetWorld()->DestroyBody(pbodySheepC->body);
	}
	if (pbodySheepD != NULL)
	{
		pbodySheepD->body->GetWorld()->DestroyBody(pbodySheepD->body);
	}


	return true;
}

void NpcW3::OnCollision(PhysBody* physA, PhysBody* physB)
{

}
