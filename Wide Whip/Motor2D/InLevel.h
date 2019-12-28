#ifndef __INLEVEL_H__
#define __INLEVEL_H__

#include "j1Module.h"
class UIImage;
class InLevel : public j1Module
{
public:
	InLevel();

	bool Awake(pugi::xml_node& config) override;
	bool Start() override;
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	bool ButtonEvent(const UIButtonType type) override;
	bool SliderEvent(const UIScrollBarType type, const float ratio) override;

	float				coins_count = 0;

private:
	p2String			music;
	UIImage* ret;

};
#endif // !__INLEVEL_H__
