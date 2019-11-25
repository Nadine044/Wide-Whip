#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "ModuleCollision.h"
#include "j1Player.h"
#include "Colors.h"
#include "ModuleEntityManager.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;
	p2List_item<TileSet*>* tileset = nullptr;
	p2List_item<MapLayer*>* layer = nullptr;
	p2List_item<ImageLayers*>* image_layers = nullptr;

	for (image_layers = data.image_layers.start; image_layers; image_layers = image_layers->next)
	{
		SDL_Rect section = { 0, 0, image_layers->data->image_width, image_layers->data->image_height };
		App->render->Blit(image_layers->data->texture, image_layers->data->background_new_pos1, 0, &section, image_layers->data->parallax_image);
		int m = (App->render->camera.x * (1 - image_layers->data->parallax_image));
		if (-App->render->camera.x + App->render->camera.w > image_layers->data->background_new_pos2 - image_layers->data->image_width - m + image_layers->data->image_width && !image_layers->data->on_first)
		{
			image_layers->data->background_new_pos1 += image_layers->data->image_width * 2;
			image_layers->data->on_first = true;
		}

		if (-App->render->camera.x < (image_layers->data->background_new_pos2 - image_layers->data->image_width) -App->render->camera.x * (1 - image_layers->data->parallax_image) && image_layers->data->on_first)
		{
			image_layers->data->background_new_pos1 -= image_layers->data->image_width * 2;
			image_layers->data->on_first = false;
		}

		SDL_Rect section2 = { 0, 0, image_layers->data->image_width, image_layers->data->image_height };
		App->render->Blit(image_layers->data->texture, image_layers->data->background_new_pos2 - image_layers->data->image_width, 0, &section2, image_layers->data->parallax_image);

		if (-App->render->camera.x + App->render->camera.w > ((image_layers->data->background_new_pos1 - App->render->camera.x * (1 - image_layers->data->parallax_image))) + image_layers->data->image_width && image_layers->data->on_first)
		{
			image_layers->data->background_new_pos2 += image_layers->data->image_width * 2;
			image_layers->data->on_first = false;
		}

		if (-App->render->camera.x < ((image_layers->data->background_new_pos1 - App->render->camera.x * (1 - image_layers->data->parallax_image))) && !image_layers->data->on_first)
		{
			image_layers->data->background_new_pos2 -= image_layers->data->image_width * 2;
		}
	}
	
	for (tileset = data.tilesets.start; tileset; tileset = tileset->next)
	{
		for (p2List_item<MapLayer*>* layer = data.layers.start; layer; layer = layer->next)
		{
			for (uint y = 0; y < layer->data->height_in_tiles; ++y)
			{
				for(uint x = 0; x < layer->data->width_in_tiles; ++x)
				{
					iPoint pos_in_world = MapToWorld(iPoint(x, y));
					SDL_Rect section = tileset->data->GetRectFromID(layer->data->GetID(x, y));
					if (IsOnCamera(SDL_Rect{ pos_in_world.x , pos_in_world.y, data.tile_width, data.tile_height})) //Culling
						App->render->Blit(tileset->data->texture, pos_in_world.x, pos_in_world.y, &section, layer->data->parallax_vel);
				
				}
			}
		}
	}
}

bool j1Map::PostUpdate()
{
	Draw();

	return true;
}

iPoint j1Map::MapToWorld(const iPoint& p) const
{
	iPoint ret;

	ret.x = p.x * data.tile_width;
	ret.y = p.y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(const iPoint& p) const
{
	iPoint ret;

	ret.x = p.x / data.tile_width;
	ret.y = p.y / data.tile_height;

	return ret;
}

iPoint j1Map::MapToWorldIsometric(const iPoint& p) const
{
	iPoint ret;

	ret.x = (p.x - p.y) * data.tile_width/2;
	ret.y = (p.x + p.y) * data.tile_height / 2;;

	return ret;
}

iPoint j1Map::WorldToMapIsometric(const iPoint& p) const
{
	iPoint ret;

	//ret.x = (p.x - p.y) * data.tile_width / 2;
	//ret.y = (p.x + p.y) * data.tile_height / 2;;

	return ret;
}



// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	// Remove all backgrounds
	p2List_item<ImageLayers*>* item3;
	item3 = data.image_layers.start;

	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.image_layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

TileSet::~TileSet()
{
	App->tex->UnLoad(texture);
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}


// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}

		data.layers.add(set);
	}

	//Load Colliders
	pugi::xml_node object_group;
	for (object_group = map_file.child("map").child("objectgroup"); object_group && ret; object_group = object_group.next_sibling("objectgroup"))
	{

		if (ret == true)
		{
			ret = LoadObjectGroups(object_group);
		}

	}

	//Load background
	pugi::xml_node backgroundImage;
	for (backgroundImage = map_file.child("map").child("imagelayer"); backgroundImage && ret; backgroundImage = backgroundImage.next_sibling("imagelayer"))
	{
		ImageLayers* imgLayer = new ImageLayers();

		ret = LoadImageLayers(backgroundImage, imgLayer);

		if (ret == true)
			data.image_layers.add(imgLayer);
	}



	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width_in_tiles, l->height_in_tiles);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	layer->name.create(node.attribute("name").as_string());
	layer->width_in_tiles = node.attribute("width").as_uint();
	layer->height_in_tiles = node.attribute("height").as_uint();
	layer->parallax_vel = node.child("properties").child("property").attribute("value").as_float();

	uint size = layer->width_in_tiles * layer->height_in_tiles;
	layer->data_gid = new uint[size];
	memset(layer->data_gid, 0, sizeof(size));

	pugi::xml_node item;
	uint i = 0u;
	for (item = node.child("data").child("tile"); item; item = item.next_sibling("tile"))
	{
		layer->data_gid[i] = item.attribute("gid").as_uint();
		++i;
	}

	return true;
}

bool j1Map::LoadObjectGroups(pugi::xml_node& node)
{
	
	for (pugi::xml_node object = node.child("object"); object; object = object.next_sibling("object"))
	{
		iPoint pos = iPoint(object.attribute("x").as_float(), object.attribute("y").as_float());
		float w = object.attribute("width").as_float();
		float h = object.attribute("height").as_float();

		SDL_Rect rect_object = { pos.x, pos.y, w, h };

		p2SString type = object.attribute("type").as_string();

		if (type == "PLAYER")
		{
			j1Player* player = (j1Player*)App->module_entity_manager->CreateEntity(EntityType::PLAYER, rect_object);
			App->collisions->player = player->col;
		}

		else if (type == "WALL")
		{		
			App->collisions->AddCollider(pos, w, h, TAG::WALL, Red);
		}

		else if (type == "PLATFORM")
		{
			Collider* ret = App->collisions->AddCollider(pos, w, h, TAG::PLATFORM, Yellow);
		}

		else if (type == "WATER")
		{
			Collider* ret = App->collisions->AddCollider(pos, w, h, TAG::WATER, Blue);
		}

		else if (type == "CHANGE_LEVEL")
		{
			Collider* ret = App->collisions->AddCollider(pos, w, h, TAG::CHANGE_LEVEL, Pink);
		}
	}

	return true;

}

bool j1Map::LoadImageLayers(pugi::xml_node& node, ImageLayers* object)
{
	bool ret = true;
	pugi::xml_node image = node.child("image");
	/*object->position_x = node.attribute("offsetx").as_int();
	object->position_y = node.attribute("offsety").as_int();*/
	
	object->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
	int w, h;
	SDL_QueryTexture(object->texture, NULL, NULL, &w, &h);
	object->image_width = image.attribute("width").as_int();

	if (object->image_width <= 0)
	{
		object->image_width = w;
	}

	object->image_height = image.attribute("height").as_int();

	if (object->image_height <= 0)
	{
		object->image_height = h;
	}

	object->parallax_image = node.child("properties").child("property").attribute("value").as_float();
	

	
	return true;
}

bool j1Map::IsOnCamera(SDL_Rect rect) const
{
	SDL_Rect r;
	r.x = -App->render->camera.x;
	r.y = -App->render->camera.y;
	r.w = App->render->camera.w;
	r.h = App->render->camera.h;
	return !(r.x >= (rect.x + rect.w) || (r.x + r.w) <= rect.x || r.y >= (rect.y + rect.h) || (r.y + r.h) <= rect.y);

}