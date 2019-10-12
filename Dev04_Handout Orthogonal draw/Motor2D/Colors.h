#ifndef __COLORS_H__
#define __COLORS_H__

#include "SDL/include/SDL_stdinc.h"
struct Color
{
	Uint8 r, g, b, a;

	Color() : r(0u), g(0u), b(0u), a(255u)
	{}

	Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255u) : r(r), g(g), b(b), a(a)
	{}

	void Set(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255u)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	float* operator & ()
	{
		return (float*)this;
	}
};

extern Color Red;
extern Color Green;
extern Color Blue;


#endif // !__COLORS_H__


