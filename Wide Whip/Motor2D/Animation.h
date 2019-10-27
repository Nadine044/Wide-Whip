#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 150

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];

	float current_frame = 0.0f;
	int last_frame = 0;

private:

	int loops = 0;

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame()
	{
		current_frame += speed;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		current_frame = 0.0f;
	}

	void LoadAnimation(pugi::xml_node node_anim)
	{
		speed = node_anim.attribute("speed").as_float();
		loop = node_anim.attribute("loop").as_bool();

		SDL_Rect frame;
		for (pugi::xml_node frame_node = node_anim.child("frame"); frame_node; frame_node = frame_node.next_sibling("frame"))
		{
			frame.x = frame_node.attribute("x").as_int();
			frame.y = frame_node.attribute("y").as_int();
			frame.w = frame_node.attribute("w").as_int();
			frame.h = frame_node.attribute("h").as_int();

			PushBack(frame);
		}

	}
};

#endif