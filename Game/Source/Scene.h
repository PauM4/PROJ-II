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

	std::vector<std::string> GetDialogue() { return dialogue; }

	std::string LastTextNPC(ColliderType NPC);

	void AppearDialogue();

	ColliderType GetPlayerLastCollision() { return player->lastCollision; }

//Dialogos Mundo 2
public:
	void CreateDialogue2();

	void RunDialogueTree2(ColliderType NPC);

	void UpdateDialogueTree2(int opt);

	void AppearDialogue2();

	std::vector<std::string> GetDialogue2() { return dialogue2; }

private:
	std::shared_ptr<DialogueTree> pigsBeforeCombatTree, pigsAfterCombatTree, wolfTree;

	std::vector<std::string> dialogue2;





//Dialogos Mundo 3
public:
	void CreateDialogue3();

	void RunDialogueTree3(ColliderType NPC);

	void UpdateDialogueTree3(int opt);

	void AppearDialogue3();

	std::vector<std::string> GetDialogue3() { return dialogue3; }

private:
	std::shared_ptr<DialogueTree> deadVillagerTree, sheepATree, sheepBTree, sheepCTree, sheepDTree;

	std::vector<std::string> dialogue3;



	

//Dialogos Mundo 4
public:
	void CreateDialogue4();

	void RunDialogueTree4(ColliderType NPC);

	void UpdateDialogueTree4(int opt);

	void AppearDialogue4();

	std::vector<std::string> GetDialogue4() { return dialogue4; }

private:
	std::shared_ptr<DialogueTree> wolfBeforeCombatTree, wolfAfterCombatTree;

	std::vector<std::string> dialogue4;



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

	void Prueba();

	int pruebaj = 0;

	bool godMode;


};

#endif // __SCENE_H__