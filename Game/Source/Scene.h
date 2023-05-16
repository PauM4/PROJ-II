#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Door.h"
#include "Npc.h"
#include "Item.h"
#include "Portal.h"

#include "UIModule.h"
#include "GuiButton.h"
#include "DialogueTree.h"
#include "UriBehaviorTree.h"
#include "Physics.h"
#include "SDL_Timer.h"
#include "Animation.h"
#include "Tweening.h"

#include <vector>
#include <string>

struct SDL_Texture;

// A struct to represent a quest
struct Quest {
	const char* description; // The quest description
	bool completed; // Whether the quest is completed or not
};


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

	void MenuAppear();

	void TweenyTestWithU();

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

	void UpdateMinigameLogic(float dt);

	void drawQuest(int posX, int posY);

	void nextQuest();

	bool CheckInsideBoundaries();

	std::vector<std::string> GetDialogue() { return dialogue; }

	std::string LastTextNPC(ColliderType NPC);

	void AppearDialogue();

	ColliderType GetPlayerLastCollision() { return player->lastCollision; }

private:

	void Camera();
	void GodMode();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);
	void MoveToBattleFromDialogue();
	void LoadChests(pugi::xml_node& data);


public:

	Player* player;
	Npc* npc1;
	List<Door*> doors;
	Portal* portal;
	Item* chest1;
	Item* chest2;
	Item* chest3;

	// A list of quests
	std::vector<Quest> questList;
	const char* questText;
	PhysBody* secondQuestCollider;

	// The index of the current quest
	int currentQuestIndex;

	// Counter that triggers the tutorial screens when new Game
	// Number 2 means --> don't appear the tutorial
	int basicTutorialCounter;

	// Load things
	int loadPlayerPosX;
	int loadPlayerPosY;

	// New Game things
	bool isNewGame;

	SDL_Texture* npcPopUpTexture;
	SDL_Texture* uiSpriteTexture;
	SDL_Texture* questUiTexture;

	// Basic Tutorials
	SDL_Texture* moveTutorialTextutre;
	SDL_Texture* interactTutorialTextutre;
	SDL_Texture* lvlupTexture;

	// Rope minigame
	SDL_Texture* ropeTexture;
	Animation pressKeyAnim;
	SDL_Texture* pressKeyTexture;
	SDL_Rect ropeRect;
	SDL_Rect keyRect;

	//Chests
	SDL_Texture* chestTexture;
	SDL_Rect chestHRect;
	SDL_Rect chestVRect;

	bool angryVillagerDefeated, LRRHDefeated;

	int numTimesAVDialogueTriggered, numTimesLRRHDialogueTriggered;
	SDL_Timer timerToSceneBattle, timerToLRRHCombat;

	//Load FX rope
	int ropeFX = -1;

	

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
		talismanVillagerTree, littleRedTree, grandmaTree, littleRedACTree;

	std::vector<std::string> dialogue;

	bool godMode;


	// Minigame variables:
	float ropeSpeed;
	int ropeJump;
	int ropeSpeedLimit;
	bool ropeWin;

public:
	int ropeX;
	int ropeY;
	// Debug bool to activate minigame when desired
	bool minigameActive;
	int minigameTVdialogueCounter;

	//----------------

	SDL_Texture* eKeyTexture;
	Tween eKeyAnim;
	bool inventoryOpen;
};

#endif // __SCENE_H__


