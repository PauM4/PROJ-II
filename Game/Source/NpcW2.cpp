#include "NpcW2.h" 
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

NpcW2::NpcW2() : Entity(EntityType::NPC)
{
	name.Create("NpcW2");
}

NpcW2::~NpcW2() {

}

bool NpcW2::Awake() {

	npcW2TexturePath = "Assets/Characters/W2Sprites_ingame.png";
	//x=5054
	posPigs.x = 4854, posPigs.y = 2958;
	pigsAnimation.PushBack({ 307,25,218,252 });

	posZorro.x = 5374, posZorro.y = 3218;
	zorroAnimation.PushBack({ 44,60,179,174 });

	return true;
}

bool NpcW2::Start() {
	currentAnimation = nullptr;

	npcW2Texture = app->tex->Load(npcW2TexturePath);

	pbodyPigs = app->physics->CreateRectangle(posPigs.x+99, posPigs.y+40, 218, 252, bodyType::STATIC);
	pbodyPigs->listener = this;
	pbodyPigs->ctype = ColliderType::PIGS;

	pbodyZorro = app->physics->CreateRectangle(posZorro.x + 79, posZorro.y + 30, 179, 174, bodyType::STATIC);
	pbodyZorro->listener = this;
	pbodyZorro->ctype = ColliderType::ZORRO;

	return true;
}

bool NpcW2::Update(float dt)
{

	return true;
}

bool NpcW2::PostUpdate()
{
	currentAnimation = &pigsAnimation;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcW2Texture, posPigs.x - 13, posPigs.y - 86, &rect);

	currentAnimation = &zorroAnimation;
	rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(npcW2Texture, posZorro.x - 13, posZorro.y - 86, &rect);

	return true;
}

bool NpcW2::CleanUp()
{
	app->tex->UnLoad(npcW2Texture);

	if (pbodyPigs != NULL)
	{
		pbodyPigs->body->GetWorld()->DestroyBody(pbodyPigs->body);
	}

	if (pbodyZorro != NULL)
	{
		pbodyZorro->body->GetWorld()->DestroyBody(pbodyZorro->body);
	}

	return true;
}

void NpcW2::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

