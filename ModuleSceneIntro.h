#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void CreatePinball();
	

public:

	p2List<PhysBody*> circles;
	p2List<PhysBody*> shellders;
	p2List<PhysBody*> pinball;
	
	PhysBody* PsyDuck;
	PhysBody* sensor;
	bool sensed;

	SDL_Texture* PinballMap;

	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
