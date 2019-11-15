#ifndef __ENTITY_H__
#define __ENTITY_H__

class Entity
{
public:
	enum class type
	{
		UNKNOW = -1,
		PLAYER,
		ENEMY,
		MAX
	};

	Entity();

	bool Update(float dt);
};

#endif // !__ENTITY_H__

