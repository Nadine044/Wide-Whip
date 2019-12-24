#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Map.h"
#include "j1Input.h"
#include "j1Render.h"


j1PathFinding::j1PathFinding() : j1Module(), last_path(DEFAULT_PATH_LENGTH)
{
	name.create("pathfinding");
}

bool j1PathFinding::Start()
{
	debug_tex = App->tex->Load("maps/path2.png");

	return true;
}

bool j1PathFinding::PreUpdate()
{

	/*static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding_module->CreatePath(last_path, origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}*/

	return true;
}

bool j1PathFinding::PostUpdate()
{

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		draw_debug = !draw_debug;


	if (draw_debug)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p);
		p = App->map->MapToWorld(p);

		SDL_Rect rect = { 0, 0, 32, 32 };
		App->render->Blit(debug_tex, p.x, p.y, &rect);

		const p2DynArray<iPoint>* path = App->pathfinding_module->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(*path->At(i));
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}

	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	return true;
}


// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{

	return (pos.x >= 0 && pos.x <= (int)App->map->data.width * App->map->data.tile_width &&
			pos.y >= 0 && pos.y <= (int)App->map->data.height * App->map->data.tile_height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{

	bool ret = true;
	if (CheckBoundaries(pos))
	{
		if (App->map->data.layers.start->data->GetID(pos.x, pos.y) != 0)
			ret = false;

	}
	else
		ret = false;


	return ret;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while(item)
	{
		if(item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while(item)
	{
		if(item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if(App->pathfinding_module->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if(App->pathfinding_module->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if(App->pathfinding_module->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if(App->pathfinding_module->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(p2DynArray<iPoint>& path, const iPoint& origin, const iPoint& destination)
{
	int ret = -1;
	// TODO 1: if origin or destination are not walkable, return -1
	path.Clear();
	if (!IsWalkable(origin) || !CheckBoundaries(origin) || !IsWalkable(destination) || !CheckBoundaries(destination))
	{
		return ret;
	}

	PathList visiting;
	PathList visited;

	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list

	visiting.list.add(PathNode(0, 0, origin, nullptr));

	while (visiting.list.count() != 0)
	{
		// TODO 3: Move the lowest score cell from open list to the closed list

		PathNode* current = &visited.list.add(visiting.GetNodeLowestScore()->data)->data;
		visiting.list.del(visiting.Find(current->pos));

		// TODO 4: If we just added the destination, we are done!
	// Backtrack to create the final path
	// Use the Pathnode::parent and Flip() the path when you are finish
		if (current->pos == destination)
		{
			const PathNode* parent_current = &visited.list.end->data;
			path.PushBack(parent_current->pos);
			while (parent_current->pos != origin)
			{
				parent_current = parent_current->parent;
				path.PushBack(parent_current->pos);
			}
			return (int)visited.list.count();
		}

		// TODO 5: Fill a list of all adjancent nodes


		PathList neighbours; 
		if (current->FindWalkableAdjacents(neighbours) > 0)
		{
			// TODO 6: Iterate adjancent nodes:
// ignore nodes in the closed list
// If it is NOT found, calculate its F and add it to the open list
// If it is already in the open list, check if it is a better path (compare G)
// If it is a better path, Update the parent

			for (p2List_item<PathNode>* iter = neighbours.list.start; iter; iter = iter->next)
			{
				iter->data.CalculateF(destination);
				if (visited.Find(iter->data.pos) == nullptr)
				{
					visiting.list.add(iter->data);
					iter->data.parent = current;
				}
				else if(visiting.Find(iter->data.pos) != nullptr)
				{
					if (visiting.Find(iter->data.pos)->data.g >= iter->data.g)
					{
						iter->data.parent = current;
					}
				}
			}
		}
		neighbours.list.clear();

	}

	visiting.list.clear();
	visited.list.clear();


	return -1;
}

const iPoint* j1PathFinding::GetNextHorizontalPoint(p2DynArray<iPoint>& path, const iPoint* current_point) const
{
	int index = path.Find(current_point);
	for (int i = index-1; i >= 0; --i)
	{
		if (path.At(i)->x != current_point->x)
		{
			return path.At(i);
		}
	}

	return nullptr;
}

const iPoint* j1PathFinding::GetNextVerticalPoint(p2DynArray<iPoint>& path, const iPoint* current_point) const
{
	int index = path.Find(current_point);
	for (int i = index - 1; i >= 0; --i)
	{
		if (path.At(i)->y != current_point->y)
		{
			return path.At(i);
		}
	}

	return nullptr;
}

