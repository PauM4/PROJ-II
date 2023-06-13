#include "Enemy_FinalWolf.h"

#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "SceneBattle.h"
#include "BattleManager.h"


Enemy_FinalWolf::Enemy_FinalWolf() : Entity(EntityType::FINALWOLF) {

	name.Create("enemy_finalwolf");
	namechar.Create("FinalWolf");
	isAlive = true;
	battleState = IDLE;
	//Behavior Tree
	//Action Nodes
	auto getCloser = std::make_shared<GetCloser>();
	auto shoot = std::make_shared<Shoot>();
	auto moveAway = std::make_shared<MoveAway>();

	//Control Flow Nodes
	inRangeChecker = std::make_shared<SwitchConditionNode>(shoot, getCloser);
	inRangeChecker->SetNodeName("AngryVillager: inRangeChecker");

	isEnemyTooClose = std::make_shared<SwitchConditionNode>(moveAway, inRangeChecker);
	isEnemyTooClose->SetNodeName("AngryVillager: isEnemyTooClose");

	//Behavior Tree Node
	behaviorTree = std::make_unique<BehaviorTree>(isEnemyTooClose);
	behaviorTree->SetDebugEnabled(true);

}
Enemy_FinalWolf::~Enemy_FinalWolf() {

}
bool Enemy_FinalWolf::Awake()
{
	id = 3;
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	 
	health = stats.attribute("health").as_int();
	maxHealth = stats.attribute("maxHealth").as_int();
	defense = stats.attribute("defense").as_int();
	magic = stats.attribute("magic").as_int();
	stamina = stats.attribute("stamina").as_int();
	maxStamina = stats.attribute("maxStamina").as_int();
	speed = stats.attribute("speed").as_int();
	attack = stats.attribute("attack").as_int();
	AttArea = stats.attribute("AttArea").as_int();
	Ab1Type = stats.attribute("Ab1Type").as_int();
	Ab1Area = stats.attribute("Ab1Area").as_int();
	Ab1RangeType = stats.attribute("Ab1RangeType").as_int();
	Ab1Power = stats.attribute("Ab1Power").as_int();
	Ab2Type = stats.attribute("Ab2Type").as_int();
	Ab2Area = stats.attribute("Ab2Area").as_int();
	Ab2RangeType = stats.attribute("Ab2RangeType").as_int();
	Ab2Power = stats.attribute("Ab2Power").as_int();
	healingpower = stats.attribute("healingpower").as_int();
	movement = stats.attribute("movement").as_int();
	isEnemy = true;

	idleAnim.PushBack({ 0, 0, 225, 225 });
	idleAnim.loop = true;

	takedmgAnim.PushBack({ 1575, 0, 225, 225 });
	takedmgAnim.PushBack({ 1575, 0, 225, 225 });
	takedmgAnim.PushBack({ 1575, 0, 225, 225 });
	takedmgAnim.PushBack({ 1575, 0, 225, 225 });
	takedmgAnim.loop = false;
	takedmgAnim.speed = 0.20f;

	for (int i = 0; i < 5; i++) {
		abilityAnim.PushBack({ (i * 225) + 225, 0, 225, 225 });
	}
	abilityAnim.loop = false;
	abilityAnim.speed = 0.25;
	

	for (int i = 0; i < 6; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i * 225), 225, 225, 225 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.15f;

	for (int i = 0; i < 6; i++)
	{
		walkUpAnim.PushBack({ ( 225), 900, 225, 225 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.15f;

	for (int i = 0; i < 6; i++)
	{
		walkRightAnim.PushBack({ (i * 225), 450, 225, 225 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.15f;

	for (int i = 0; i < 6; i++)
	{
		walkLeftAnim.PushBack({ (i * 225), 675, 225, 225 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.15f;


	texture = app->tex->Load("Assets/Characters/Lobo_Corrompido.png");

	IconGtexture = app->tex->Load("Assets/UI/WolfIconG.png");
	Icontexture = app->tex->Load("Assets/UI/WolfIcon.png");	

	currentAnimation = &idleAnim;



	return true;
}

bool Enemy_FinalWolf::Start()
{
	abilityFx = app->audio->LoadFx("Assets/Sounds/FX/fx_knife.wav");
	return true;
}

bool Enemy_FinalWolf::Update(float dt)
{
	currentAnimation->Update();


	return true;
}

bool Enemy_FinalWolf::PostUpdate()
{
	//Render
	if (health > 0) {
		SDL_Rect rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x - 55, position.y - 90, &rect);
	}

	if (currentAnimation == &abilityAnim)
	{
		if (abilityAnim.HasFinished())
		{
			finishAnimBool = true;
			abilityAnim.Reset();
			currentAnimation = &idleAnim;
		
		}
	}
	return true;
}

bool Enemy_FinalWolf::CleanUp()
{
	return true;
}

bool Enemy_FinalWolf::LoadState(pugi::xml_node&)
{
	return true;
}

bool Enemy_FinalWolf::SaveState(pugi::xml_node&)
{
	return true;
}

void Enemy_FinalWolf::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

int Enemy_FinalWolf::GetState()
{
	return 0;
}

int Enemy_FinalWolf::Attack()
{
	return 0;
}

int Enemy_FinalWolf::Ability(int id)
{
	return 0;
}

void Enemy_FinalWolf::Movement()
{



}


NodeStatus Enemy_FinalWolf::GetCloser::Run() {
	//Hacemos que actionType sea Move para que se mueva cuando entre al estado Inactive
	app->battleManager->actionType = ActionType::MOVE;

	return NodeStatus::Success;
}

NodeStatus Enemy_FinalWolf::Shoot::Run() {

	//Ataca al player
	app->battleManager->TriggerAIAttack();

	//--------
	app->battleManager->battleState = BattleState::THINKING;
	app->battleManager->UpdateTurnList();


	return NodeStatus::Success;
}

NodeStatus Enemy_FinalWolf::MoveAway::Run() {
	// implementacion detallada del nodo
	return NodeStatus::Success;
}

