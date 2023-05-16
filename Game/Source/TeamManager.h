	#ifndef __TEAMMANAGER_H__
#define __TEAMMANAGER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "List.h"
#include "Animation.h"
#include "Timmy.h"
#include "Bunny.h"
#include "Lrrh.h"
#include "LittlePig.h"
#include "MiddlePig.h"
#include <iostream>
using namespace std;

struct item
{
	//true if player has obtained this item
	bool ininventory;
	//type = 1 if weapon, 2 if armor, 3 if accessory
	int type;
	//weaponuser = 0 if nonweapon, 1 if timmy, 2 if bunny, 3 if lrrh, 4 if LilPig, 5 if MiddlePig, 6 if Peter
	int weaponuser;
	//character that has equipped this item: 0 if not equipped, 1 if timmy, 2 if bunny, 3 if lrrh, 4 if LilPig, 5 if MiddlePig, 6 if Peter
	int character;
	//Name and Stats
	string name;
	int defense;
	int magic;
	int speed;
	int movement;
	int attack;
	int AttArea;
	int Ab1Power;
	int Ab2Power;
	int Ab1Area;
	int Ab2Area;
	int healingpower;

};
struct stats
{
	//1 if timmy, 2 if bunny, 3 if lrrh, 4 if LilPig, 5 if MiddlePig, 6 if Peter
	int character;
	int defense;
	int magic;
	int speed;
	int movement;
	int attack;
	int AttArea;
	int Ab1Power;
	int Ab2Power;
	int Ab1Area;
	int Ab2Area;
	int healingpower;

};
class TeamManager : public Module {
public:

	TeamManager(bool isActive);
	
	virtual ~TeamManager();

	bool Awake(pugi::xml_node& config);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	void PrintLvlUpText();

	bool CleanUp();

	bool additemstats(Entity* ent, int i);

	bool addallstats();

	bool printLVLup();

	void loadinventory();

	void ApplyEquipedItemStats();

	void UpdateParty();

private:

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);


public:
	//List of acquaried playable characters
	List<Entity*> characters;
	//List of the selected playable characters in the team
	List<Entity*> team;
	List<stats*> statslist;
	List<stats> statslist2;
	Timmy* timmy;
	Bunny* bunny;
	Lrrh* lrrh;
	LittlePig* littlepig;
	MiddlePig* middlepig;
	item yoyo;
	item handsxd;
	item bow;
	item club;
	item knife;
	item shotgun;
	item ironchestplate;
	item reversehat;
	item talisman;
	item susjar;
	item dentures;

	List<item*> inventory;
	List<item*> equipment;
	List<item*> itemstoshow;
	stats timmystats;
	stats bunnystats;
	stats lrrhstats;
	stats lilpigstats;
	stats midpigstats;
	stats peterstats;
	//True if is a playable character
	bool istimmyplayable;
	//True if is a playable character
	bool isbunnyplayable;
	//True if is a playable character
	bool islrrhplayable;
	//True if is a playable character
	bool islilpigplayable;
	//True if is a playable character
	bool ismidpigplayable;
	//True if is a playable character
	bool ispeterplayable;
	//True if it's on the team
	bool IsTimmyOnTeam;
	//True if it's on the team
	bool IsBunnyOnTeam;
	//True if it's on the team
	bool IsLrrhOnTeam;
	//True if it's on the team
	bool IsLilPigOnTeam;
	//True if it's on the team
	bool IsMidPigOnTeam;
	//True if it's on the team
	bool IsPeterOnTeam;

	bool statsdone = false;

	bool startstatsup = false;
	
	bool defenseup;
	bool magicup;
	bool speedup;
	bool movementup;
	bool attackup;
	bool Ab1Powerup;
	bool healingpowerup;

	bool lvlupbool;
	bool lvlupplayerstate;
	int LvlUpPoints;
	int cont;

private:

};

#endif // __TEAMMANAGER_H__
