#include "ModuleCollision.h"


Collider::Collider(fPoint pos, float w, float h, TAG tag) : tag(tag)
{
	rect.x = pos.x;
	rect.y = pos.y;
	rect.w = w;
	rect.h = h;
}


Collider* ModuleCollision::AddCollider(fPoint pos, float w, float h, TAG tag)
{
	Collider * ret = new Collider(pos, w, h, tag);
	colliders_list.add(ret);

	return ret;
}

bool ModuleCollision::Update(float dt)
{
	
}