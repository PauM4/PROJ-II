#ifndef __W3_SCENE_H__
#define __W3_SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Door.h"
#include "NpcW3.h"

#include "UIModule.h"
#include "GuiButton.h"
#include "DialogueTree.h"
#include "UriBehaviorTree.h"
#include "Physics.h"

struct SDL_Texture;

class W3_Scene : public Module
{
public:

	W3_Scene(bool isActive);

	// Destructor
	virtual ~W3_Scene();

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

	std::vector<std::string> GetDialogue() { return dialogue; }

	void AppearDialogue();

	ColliderType GetPlayerLastCollision() { return player->lastCollision; }

	void drawQuest(int posX, int posY);
	void nextQuest();


private:

	void Camera();
	bool CheckInsideBoundaries();
	void GodMode();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);
	void MoveToBattleFromDialogue();


public:

	Player* player;
	NpcW3* npc;
	List<Door*> doors;

	// A list of quests
	std::vector<Quest> questList;
	const char* questText;

	// The index of the current quest
	int currentQuestIndex;

	// Load things
	int loadPlayerPosX;
	int loadPlayerPosY;

	// New Game things
	bool isNewGame;

	SDL_Texture* npcPopUpTexture;
	SDL_Texture* inventoryItemsTexture;
	SDL_Texture* eKeyTexture;
	SDL_Texture* questUiTexture;
	SDL_Texture* textDialogue;
	SDL_Texture* inventoryScrollTexture;
	SDL_Texture* lvlupTexture;

	bool pedroDefeated, wolfDefeated;
	int numTimesPedroDialogueTriggered, numTimesWolfDialogueTriggered;
	SDL_Timer timerToPedroCombat, timerToWolfCombat;

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

	std::shared_ptr<DialogueTree> wolfTree;
	std::vector<std::string> dialogue;

	bool godMode;


};

#endif // __W3_SCENE_H__