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

	NpcW3TexturePath = "Assets/Characters/W3_wolf.png";//provisional de momento hasta que tengamos de gameplay


	posWolf.x = 600, posWolf.y = 1500;
	wolfAnimation.PushBack({ 0,0,170,300 });


	return true;
}

bool NpcW3::Start() {
	currentAnimation = nullptr;

	NpcW3Texture = app->tex->Load(NpcW3TexturePath);

	
	pbodyWolf = app->physics->CreateRectangle(posWolf.x + 40, posWolf.y, 150, 150, bodyType::STATIC);
	pbodyWolf->listener = this;
	pbodyWolf->ctype = ColliderType::WOLF;


	return true;
}

bool NpcW3::Update(float dt)
{


	currentAnimation = &wolfAnimation;
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(NpcW3Texture, posWolf.x - 60, posWolf.y - 96, &rect);

	return true;
}

bool NpcW3::PostUpdate()
{
	


	return true;
}

bool NpcW3::CleanUp()
{
	app->tex->UnLoad(NpcW3Texture);

	
	if (pbodyWolf != NULL)
	{
		pbodyWolf->body->GetWorld()->DestroyBody(pbodyWolf->body);
	}
	

	return true;
}

void NpcW3::OnCollision(PhysBody* physA, PhysBody* physB)
{

}
