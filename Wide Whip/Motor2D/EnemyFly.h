#ifndef __ENEMYFLY_H__
#define __ENEMYFLY_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"

class EnemyFly : public Enemy
{
public:
	EnemyFly(SDL_Rect& rect);

	bool Awake(const pugi::xml_node& flyer) override;

private:
	void GoToNextPoint(float dt) override;
	void MoveNormal(const iPoint * next_point, float dt);
	void MoveInCollision(const iPoint * next_point, float dt);
};

#endif // !__ENEMYFLY_H__
