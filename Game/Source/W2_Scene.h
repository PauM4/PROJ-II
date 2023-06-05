#ifndef __W2_SCENE_H__
#define __W2_SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Door.h"
#include "NpcW2.h"
#include "Portal.h"
#include "Item.h"

#include "UIModule.h"
#include "GuiButton.h"
#include "DialogueTree.h"
#include "UriBehaviorTree.h"
#include "Physics.h"

struct SDL_Texture;

class W2_Scene : public Module
{
public:

	W2_Scene(bool isActive);

	// Destructor
	virtual ~W2_Scene();

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

	void AppearDialogue();

	void drawQuest(int posX, int posY);

	void nextQuest();

	bool CheckInsideBoundaries();

	std::vector<std::string> GetDialogue() { return dialogue; }


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
	NpcW2* npc;
	List<Door*> doors;
	List<Portal*> portals;

	Item* chest4;
	Item* chest5;
	Item* chest6;

	// A list of quests
	std::vector<Quest> questList;
	const char* questText;
	PhysBody* secondQuestCollider;

	// The index of the current quest
	int currentQuestIndex;

	// Load things
	int loadPlayerPosX;
	int loadPlayerPosY;

	// New Game things
	bool isNewGame;
	bool zorroDialogue;
	bool takePortal, firstKeyPicked, secondKeyPicked, enteredThirdHouse;

	SDL_Texture* npcPopUpTexture;
	SDL_Texture* questUiTexture;
	SDL_Texture* lvlupTexture;
	SDL_Texture* eKeyTexture;
	SDL_Texture* textDialogue;
	SDL_Texture* inventoryScrollTexture;
	SDL_Texture* keyTexture;
	SDL_Texture* brokenkeyTexture;
	SDL_Texture* assets;

	bool inventoryOpen;
	
	bool key1interact;
	bool key2interact;
	bool key1state;
	bool key2state;

	PhysBody* key1collider;
	PhysBody* key2collider;
	PhysBody* doorcollider;
	//chests
	SDL_Texture* chestTexture;
	SDL_Rect chestHRect;
	SDL_Rect chestVRect;
	SDL_Rect chestopenHRect;
	SDL_Rect chestopenVRect;

	// ITEMS UI
	SDL_Texture* inventoryItemsTexture;


private:

	SString mapName;
	SString mapFolder;

	bool pigsDefeated;
	int numTimesPigsDialogueTriggered;
	SDL_Timer timerToPigsCombat;

	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;
	int chestFX = -1;
	//Fonts
	int font;

	bool pauseMenuActive;
	bool exitButtonBool;

	std::shared_ptr<DialogueTree> pigsTree, wolfTree, zorroTree;

	std::vector<std::string> dialogue;

	bool godMode;



};

#endif // __W2_SCENE_H__