#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Door.h"
#include "Npc.h"

#include "UIModule.h"
#include "GuiButton.h"
#include "DialogueTree.h"
#include "UriBehaviorTree.h"
#include "Physics.h"
#include "Animation.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool isActive);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void CreateDialogue();

	void RunDialogueTree(ColliderType NPC);

	void UpdateDialogueTree(int opt);

	void UpdateRopeMinigame(float dt);

	std::vector<std::string> GetDialogue() { return dialogue; }

	std::string LastTextNPC(ColliderType NPC);
	std::string LastTextAngryVillager(std::string);
	std::string LastTextTalismanVillager(std::string);
	std::string LastTextGrandmaVillager(std::string);
	std::string LastTextLittleRedVillager(std::string);
	void AppearDialogue();

	ColliderType GetPlayerLastCollision() { return player->lastCollision; }

private:

	void Camera();
	void GodMode();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);


public:

	Player* player;
	Npc* npc1;
	List<Door*> doors;

	// Load things
	int loadPlayerPosX;
	int loadPlayerPosY;

	// New Game things
	bool isNewGame;

	SDL_Texture* npcPopUpTexture;
	SDL_Texture* uiSpriteTexture;

	SDL_Texture* ropeTexture;

	// Rope minigame
	Animation pressKeyAnim;
	SDL_Texture* pressKeyTexture;
	SDL_Rect ropeRect;
	SDL_Rect keyRect;

private:

	SString mapName;
	SString mapFolder;


	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	//Fonts
	int font;

	bool pauseMenuActive;
	bool exitButtonBool;

	std::shared_ptr<DialogueTree> angryVillagerTree, angryVillagerTreePT, 
		talismanVillagerTree, littleRedTree, grandmaTree;

	std::vector<std::string> dialogue;

	void Prueba();

	int pruebaj = 0;

	bool godMode;


	float ropeSpeed;
	int ropeJump;
	int ropeSpeedLimit;
	int ropeX;
	int ropeY;
	bool ropeWin;
};

#endif // __SCENE_H__