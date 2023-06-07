#ifndef __UIMODULE_H__
#define __UIMODULE_H__

#include "Module.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "SceneManager.h"
#include <vector>
#include <string>
#include "SDL_Timer.h"
#include <cassert>

struct SDL_Texture;

enum CurrentMenuType
{
	MAIN,
	PAUSE,
	COMBAT_PAUSE,
	DIALOG,
	DIALOG2,
	DIALOG3,
	COMBAT,
	ROPE_MINIGAME,
	DISABLED,
	LEVEL_UP,
	INVENTORY,
	PARTY,
	OPTIONS_MAIN,
	OPTIONS_GAME,
	OPTIONS_COMBAT
};

enum WhatInventoryImIn
{
	NO,
	TIMMY,
	BUNNY,
	LRRH,
	LPIG,
	MPIG
};

class UIModule : public Module
{
public:

	UIModule(bool isActive);

	// Destructor
	virtual ~UIModule();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	void DisableButtonsToNone();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	void PrintItemImages(int i);

	void PrintItemName();

	void PrintStats(int id);

	void PrintPositiveItemStats(int id);
	void PrintNegativeItemStats(int id);
	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void EquipUnequipItem(int numOfItem);

	// Change buttons state depending on the scene
	bool ChangeButtonState(int& currentMenuType);

	void PrintDialogue(std::vector<std::string> dialogue);
	void PrintDialogue2(std::vector<std::string> dialogue);
	void PrintDialogue3(std::vector<std::string> dialogue);

	void CleaningDialogeOverTime();

	void CheckPartyTextOnStart();

	void AddButtonsToList();


private:

	std::string DialogueOverTime(std::string dialogue);


public:

	int currentMenuType;
	int whatInventoryImIn;

	// Buttons list

	GuiButton* mainmenu_play_button;
	GuiButton* mainmenu_options_button;
	GuiButton* mainmenu_credits_button;
	GuiButton* mainmenu_quit_button;
	GuiButton* mainmenu_newGame_button;
	GuiButton* mainmenu_continueGame_button;
	GuiButton* mainmenu_return_button;

	GuiButton* pausemenu_resume_button;
	GuiButton* pausemenu_inventory_button;
	GuiButton* pausemenu_party_button;
	GuiButton* pausemenu_save_button;
	GuiButton* pausemenu_options_button;
	GuiButton* pausemenu_return_button;
	GuiButton* pausemenu_load_button;
	GuiButton* pausemenu_backtomain_button;
	GuiButton* pausemenu_quit_button;

	GuiButton* pausemenuCombat_resume_button;
	GuiButton* pausemenuCombat_options_button;
	GuiButton* pausemenuCombat_backtomain_button;
	GuiButton* pausemenuCombat_return_button;
	GuiButton* pausemenuCombat_quit_button;

	GuiButton* combat_attack_button;
	GuiButton* combat_ability_button;
	GuiButton* combat_move_button;
	GuiButton* combat_endTurn_button;

	GuiButton* dialog_option1_button;
	GuiButton* dialog_option2_button;
	GuiButton* dialog_option3_button;
	GuiButton* dialog_option4_button;

	GuiButton* dialog2_option1_button;
	GuiButton* dialog2_option2_button;
	GuiButton* dialog2_option3_button;
	GuiButton* dialog2_option4_button;

	GuiButton* dialog3_option1_button;
	GuiButton* dialog3_option2_button;
	GuiButton* dialog3_option3_button;
	GuiButton* dialog3_option4_button;

	GuiButton* levelup_defenseUp_button;
	GuiButton* levelup_magicUp_button;
	GuiButton* levelup_speedUp_button;
	GuiButton* levelup_movementUp_button;
	GuiButton* levelup_attackUp_button;
	GuiButton* levelup_AB1PowerUp_button;
	GuiButton* levelup_healingpowerUp_button;

	GuiButton* party_timmy_button;
	GuiButton* party_bunny_button;
	GuiButton* party_lrrh_button;
	GuiButton* party_Lpig_button;
	GuiButton* party_Mpig_button;
	// World 3 player
	GuiButton* party_peter_button;

	// Inventory People
	GuiButton* inventory_timmy_button;
	GuiButton* inventory_bunny_button;
	GuiButton* inventory_lrrh_button;
	GuiButton* inventory_Lpig_button;
	GuiButton* inventory_Mpig_button;
	// World 3 player
	/*GuiButton* inventory_peter_button;*/

	// Inventory items
	GuiButton* item_1_button;
	GuiButton* item_2_button;
	GuiButton* item_3_button;
	GuiButton* item_4_button;
	GuiButton* item_5_button;
	GuiButton* item_6_button;
	GuiButton* item_7_button;
	GuiButton* item_8_button;
	GuiButton* item_9_button;
	GuiButton* item_10_button;
	GuiButton* item_11_button;
	GuiButton* item_12_button;

	// Options Menu
	GuiSlider* musicSlider;
	GuiSlider* fxSlider;
	GuiCheckBox* fullScreenCheckBox;
	GuiCheckBox* vsyncCheckBox;
	SDL_Texture* sliderTexture;
	SDL_Texture* optionsBgTexture;
	SDL_Texture* checkboxTexture;

	SDL_Texture* textureA;
	SDL_Texture* playButtonTexture;
	SDL_Texture* optionsButtonTexture;
	SDL_Texture* creditsButtonTexture;
	SDL_Texture* quitButtonTexture;
	SDL_Texture* newgameButtonsTexture;
	SDL_Texture* continueButtonsTexture;
	SDL_Texture* returnButtonTexture;
	SDL_Texture* eKeyTexture;
	SDL_Texture* attackButtonTexture;
	SDL_Texture* abilityButtonTexture;
	SDL_Texture* moveButtonTexture;
	SDL_Texture* skipButtonTexture;
	SDL_Texture* currentTurnTexture;
	SDL_Texture* combatStatsTexture;
	SDL_Texture* combatEnemyStatsTexture;
	SDL_Texture* plusButton;

	SDL_Texture* pauseButtonsTexture;

	SDL_Texture* uiSpriteTexture;


	bool timmyItem;
	bool bunnyItem;
	bool lrrhItem;
	bool lpigItem;
	bool mpigItem;


	List<GuiButton*> buttonsList;
	List<GuiButton*> inventoryButtonsList;

	bool quitButtonBool;

	bool continueBool;

	// UI Things
	bool doorPlayerPosition;

	SDL_Timer textDialogueTimer;
	std::string previousTextDialogue, dialogueOverTime;

	bool dialogueHasChanged;
	int indexDialogueOverTime;
	float timeToRefreshDialogue;

	int levelUpFX = -1;
	int errorFX = -1;
	int equipitemFx = -1;
	int unequipitemFx = -1;

	bool errorWhenParty;

	SDL_Rect yoyoRect;
	SDL_Rect bunnyHandRect;
	SDL_Rect bowRect;
	SDL_Rect knifeRect;
	SDL_Rect clubRect;
	SDL_Rect shotgunRect;
	SDL_Rect ironchestRect;
	SDL_Rect revhatRect;
	SDL_Rect talismanRect;
	SDL_Rect susjarRect;
	SDL_Rect denturesRect;
private:

};

#endif // __UIMODULE_H__



