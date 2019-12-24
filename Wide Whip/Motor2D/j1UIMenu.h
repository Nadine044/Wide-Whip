#ifndef __UIMENU_H__
#define __UIMENU_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"

class UIMenu : public j1Module
{
public:
	UIMenu();

	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;
};



#endif // !__UIMENU_H__
