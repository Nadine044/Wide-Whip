// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"
#include "p2Log.h"

// ---------------------------------------------
j1Timer::j1Timer(bool start)
{
	if (start)
	{
		Start();
		running = true;
	}
}

j1Timer::~j1Timer()
{}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
	running = true;
}

// ---------------------------------------------
uint j1Timer::Read() const
{
	if (running)
		return SDL_GetTicks() - started_at;
	else
	{
		LOG("Timer not running");
		return 0u;
	}
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	if (running)
		return float(SDL_GetTicks() - started_at) / 1000.0f;
	else
	{
		LOG("Timer not running");
		return 0u;
	}
}