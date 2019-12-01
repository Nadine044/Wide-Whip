#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2Defs.h"
#include "j1App.h"
#include "j1Textures.h"
#include "SDL/include/SDL_rect.h"
#include "j1Player.h"
#include "Enemy.h"
// ----------------------------------------------------
struct MapLayer
{

	~MapLayer()
	{
		RELEASE_ARRAY(data_gid);
	}
	p2SString		name;
	uint			width_in_tiles;
	uint			height_in_tiles;
	uint*			data_gid;
	float			parallax_vel;


inline uint GetID(int x, int y)
{
	return data_gid[x + (y * width_in_tiles)];
}
};

struct ImageLayers
{
	SDL_Texture*		texture;
	int					position_x;
	int					position_y;
	int					image_width;
	int					image_height;
	float				parallax_image;
	int					background_new_pos1 = 0;
	int					background_new_pos2 = 0;
	bool				on_first = true;
	~ImageLayers() 
	{
		App->tex->UnLoad(texture);
	};
};


// ----------------------------------------------------
struct TileSet
{
	~TileSet();
	SDL_Rect GetRectFromID(const int id)
	{
		SDL_Rect ret;

		int new_id = id - firstgid;

		int width_with_margin = tile_width + spacing;
		int height_with_margin = tile_height + spacing;

		ret.x = (new_id % num_tiles_width) * width_with_margin + margin;
		ret.y = (new_id / num_tiles_width) * height_with_margin + margin;
		ret.w = tile_width;
		ret.h = tile_height;

		return ret;

	}
	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	layers;
	p2List<ImageLayers*>	image_layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	bool PostUpdate() override;
	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(const iPoint&) const;
	iPoint WorldToMap(const iPoint&) const;
	iPoint MapToWorldIsometric(const iPoint& )const;
	iPoint WorldToMapIsometric(const iPoint&)const;


	bool IsOnCamera(SDL_Rect) const;


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadObjectGroups(pugi::xml_node& node);
	bool LoadImageLayers(pugi::xml_node& node, ImageLayers* object);

public:

	MapData data;

	//Entities

	j1Player* player = nullptr;
	Enemy* enemy = nullptr;
	Enemy* walk_enemy = nullptr;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	
	bool				draw_debug = false;
};

#endif // __j1MAP_H__