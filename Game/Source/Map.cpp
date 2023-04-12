#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "SceneBattle.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map(bool isActive) : Module(isActive), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

//Create walkability map for pathfinding
bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;
    ListItem<TileLayer*>* item;
    item = mapData.tileLayers.start;

    for (item = mapData.tileLayers.start; item != NULL; item = item->next)
    {
        TileLayer* layer = item->data;

        if (layer->properties.GetProperty("Navigation") != NULL && !layer->properties.GetProperty("Navigation")->value)
            continue;

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);

        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;

                int tileId = layer->Get(x, y);
                TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

                if (tileset != NULL)
                {
                    //According to the mapType use the ID of the tile to set the walkability value
                    map[i] = 1;
                    
                }
                else {
                    //LOG("CreateWalkabilityMap: Invalid tileset found");
                    map[i] = 0;
                }
            }
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}

void Map::Draw()
{
    if (mapLoaded == false)
        return;

    ListItem<ImageLayer*>* imageLayerItem;
    imageLayerItem = mapData.imageLayers.start;
    
   
   while (imageLayerItem != NULL) {
        if (imageLayerItem->data->properties.GetProperty("Draw") != NULL && imageLayerItem->data->properties.GetProperty("Draw")->value) {

            app->render->DrawTexture(imageLayerItem->data->texture, imageLayerItem->data->offsetX, imageLayerItem->data->offsetY);
        }
        imageLayerItem = imageLayerItem->next;
    }
    
    ListItem<TileLayer*>* tileLayerItem;
    tileLayerItem = mapData.tileLayers.start;

    while (tileLayerItem != NULL) {
        if (tileLayerItem->data->properties.GetProperty("Draw") != NULL && tileLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < tileLayerItem->data->width; x++)
            {
                for (int y = 0; y < tileLayerItem->data->height; y++)
                {
                    //Complete the draw function
                    int gid = tileLayerItem->data->Get(x, y);

                    //Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture, pos.x, pos.y, &r);
                }
            }
        }

        tileLayerItem = tileLayerItem->next;
    }
}

// Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;
 
    return ret;
}

// Add method WorldToMap to obtain  map coordinates from screen coordinates
iPoint Map::WorldToMap(int x, int y) 
{
    iPoint ret(0, 0);

    ret.x = x / mapData.tileWidth;
    ret.y = y / mapData.tileHeight;
    
    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}


// Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L04: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // Remove all layers
    // Remove tileLayers
    ListItem<TileLayer*>* tileLayerItem;
    tileLayerItem = mapData.tileLayers.start;

    while (tileLayerItem != NULL)
    {
        RELEASE(tileLayerItem->data);
        mapData.tileLayers.Del(tileLayerItem); 
        tileLayerItem = tileLayerItem->next;
    }

    // Remove imageLayers
    ListItem<ImageLayer*>* imageLayerItem;
    imageLayerItem = mapData.imageLayers.start;

    while (imageLayerItem != NULL)
    {
        RELEASE(imageLayerItem->data);
        mapData.imageLayers.Del(imageLayerItem);
        imageLayerItem = imageLayerItem->next;
    }

   

    return true;
}

// Load new map
bool Map::Load(SString mapName, SString mapFolder)
{
    bool ret = true;

    mapFileName = mapName;
    this->mapFolder = mapFolder;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }

    if (ret == true)
    {
        ret = LoadColliders(mapFileXML.child("map"));
    }

    if(ret == true)
    {
        // LOG all the data loaded iterate all tilesets and LOG everything
       
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        // LOG the info for each loaded layer
        ListItem<TileLayer*>* tileLayer;
        tileLayer = mapData.tileLayers.start;

        while (tileLayer != NULL) {
            LOG("TileLayer----");
            LOG("id : %d name : %s", tileLayer->data->id, tileLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", tileLayer->data->width, tileLayer->data->height);
            tileLayer = tileLayer->next;
        }

        ListItem<ImageLayer*>* imageLayer;
        imageLayer = mapData.imageLayers.start;

        while (imageLayer != NULL) {
            LOG("ImageLayer----");
            LOG("id : %d name : %s", imageLayer->data->id, imageLayer->data->name.GetString());
            LOG("Layer offsetX : %d Layer offsetY : %d", imageLayer->data->offsetX, imageLayer->data->offsetY);
            LOG("TexturePath : %s", imageLayer->data->texturePath);
            imageLayer = imageLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();

        mapData.mapType = (MAP_TYPE)map.child("properties").child("property").attribute("value").as_int();
    }

    return ret;
}

// Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // L04: DONE 4: Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        mapData.tilesets.Add(set);
    }

    return ret;
}

// L05: DONE 3: Implement a function that loads a single layer layer
bool Map::LoadTileLayer(pugi::xml_node& node, TileLayer* layer)
{
    bool ret = true;

    // Load the attributes
    layer->name = node.attribute("name").as_string();
    layer->id = node.attribute("id").as_int();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    // Load properties
    LoadProperties(node, layer->properties);

    // Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    // Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();

        //Fills a 2D array with every gid value from metadata tile layer
        if (mapData.mapType == MAP_TYPE::COMBAT && layer->name == "metadata") {

            metadataLayer[i % COMBAT_MAP_WIDTH][i / COMBAT_MAP_WIDTH] = tile.attribute("gid").as_int() - 1;
        }

        i++;
    }

    return ret;
}

bool Map::LoadImageLayer(pugi::xml_node& node, ImageLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->name = node.attribute("name").as_string();
    layer->id = node.attribute("id").as_int();
    layer->offsetX = node.attribute("offsetx").as_int();
    layer->offsetY = node.attribute("offsety").as_int();
    layer->texturePath = (const char*)node.child("image").attribute("source").as_string();

    layer->texture = app->tex->Load(layer->texturePath);

    // Load properties
    LoadProperties(node, layer->properties);

    LOG("LAYER PATH: %s", layer->texturePath);

    return ret;
}

// Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    // Iterate TileLayers
    for (pugi::xml_node tileLayerNode = mapNode.child("layer"); tileLayerNode && ret; tileLayerNode = tileLayerNode.next_sibling("layer"))
    {
        //Load the layer
        TileLayer* tileLayer = new TileLayer();
        ret = LoadTileLayer(tileLayerNode, tileLayer);

        //add the layer to the map
        mapData.tileLayers.Add(tileLayer);
    }

    // Iterate imageLayers
    for (pugi::xml_node imageLayerNode = mapNode.child("imagelayer"); imageLayerNode && ret; imageLayerNode = imageLayerNode.next_sibling("imagelayer"))
    {
        //Load the layer
        ImageLayer* imageLayer = new ImageLayer();
        ret = LoadImageLayer(imageLayerNode, imageLayer);

        //add the layer to the map
        mapData.imageLayers.Add(imageLayer);
    }

    return ret;
}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertyNode = node.child("properties").child("property"); propertyNode; propertyNode = propertyNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertyNode.attribute("name").as_string();
        p->value = propertyNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}

bool Map::LoadColliders(pugi::xml_node& node) {

    bool ret = true;

    for (pugi::xml_node colLayerNode = node.child("objectgroup"); colLayerNode; colLayerNode = colLayerNode.next_sibling("objectgroup")) {
        int colType = colLayerNode.child("properties").child("property").attribute("value").as_int();
        for (pugi::xml_node colNode = colLayerNode.child("object"); colNode; colNode = colNode.next_sibling("object")) {

            ColData col;

            col.id = colNode.attribute("id").as_int();
            col.x = colNode.attribute("x").as_float();
            col.y = colNode.attribute("y").as_float();
            col.width = colNode.attribute("width").as_float();
            col.height = colNode.attribute("height").as_float();
            col.type = colType;

            CreateColliders(col);

        }
    }

    return ret;
}

void Map::CreateColliders(ColData col) {

    PhysBody* collider;

    collider = app->physics->CreateRectangle(col.x + col.width / 2, col.y + col.height / 2, col.width, col.height, bodyType::STATIC);
    collider->ctype = (ColliderType)col.type;

}

Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}

