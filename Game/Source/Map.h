#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"

#include "PugiXml\src\pugixml.hpp"

#define COST_MAP_SIZE 25

#define COMBAT_MAP_WIDTH 16
#define COMBAT_MAP_HEIGHT 9

enum class MAP_TYPE {
	UNKNOWN = -1,
	NORMAL,
	COMBAT
};

struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	//Method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int gid) const;
};

struct Properties
{
	struct Property
	{
		SString name;
		bool value;
	};

	~Properties()
	{
		//...
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.Clear();
	}

	// L06: DONE 7: Method to ask for the value of a custom property
	Property* GetProperty(const char* name);

	List<Property*> list;
};

struct TileLayer {

	SString	name;
	int id;
	int width;
	int height;
	uint* data;

	Properties properties;

	TileLayer() : data(NULL)
	{}

	~TileLayer()
	{
		RELEASE(data);
	}

	//Gets the gid value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct ImageLayer {

	SString name;
	int id;
	int width;
	int height;
	int x;
	int y;

	Properties properties;

	const char* texturePath;
	SDL_Texture* texture;
};

struct MapData
{
	int width;
	int	height;
	int tileWidth;
	int tileHeight;
	MAP_TYPE mapType;
	List<TileSet*> tilesets;

	//List of layers of the map
	List<TileLayer*> tileLayers;
	List<ImageLayer*> imageLayers;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load();

	// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	// L08: DONE 3: Add method WorldToMap to obtain  
	iPoint Map::WorldToMap(int x, int y);

private:

	bool LoadMap(pugi::xml_node mapFile);

	//Loads the tileset
	bool LoadTileSet(pugi::xml_node mapFile);

	
	bool LoadTileLayer(pugi::xml_node& node, TileLayer* layer);
	bool LoadImageLayer(pugi::xml_node& node, ImageLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	TileSet* GetTilesetFromTileId(int gid) const;

	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public: 

	MapData mapData;

	int metadataLayer[COMBAT_MAP_WIDTH][COMBAT_MAP_HEIGHT];

private:

    SString mapFileName;
	SString mapFolder;

    bool mapLoaded;
};

#endif // __MAP_H__