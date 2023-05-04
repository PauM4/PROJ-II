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

	npcW2TexturePath = "Assets/Characters/Characters_popups.png";

	posPigs.x = 5054, posPigs.y = 2958;

	pigsAnimation.PushBack({ 284,55,127,189 });

	return true;
}

bool NpcW2::Start() {
	currentAnimation = nullptr;

	npcW2Texture = app->tex->Load(npcW2TexturePath);

	pbodyPigs = app->physics->CreateRectangle(posPigs.x + 40, posPigs.y, 150, 150, bodyType::STATIC);
	pbodyPigs->listener = this;
	pbodyPigs->ctype = ColliderType::PIGS;



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

	return true;
}

bool NpcW2::CleanUp()
{
	app->tex->UnLoad(npcW2Texture);

	if (pbodyPigs != NULL)
	{
		pbodyPigs->body->GetWorld()->DestroyBody(pbodyPigs->body);
	}


	return true;
}

void NpcW2::OnCollision(PhysBody* physA, PhysBody* physB)
{

}
