#ifndef __ENEMYFLY_H__
#define __ENEMYFLY_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"

class EnemyFly : public Enemy
{
public:
	EnemyFly(SDL_Rect& rect);

private:
	void GoToNextPoint(float dt) override;
	void MoveNormal(const iPoint * next_point, float dt);
	void MoveInCollision(const iPoint * next_point, float dt);

	bool CleanUp() override;
};

#endif // !__ENEMYFLY_H__
