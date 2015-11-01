#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h" 

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
	ray_on = false;
	sensed = false;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	circle = App->textures->Load("pinball/wheel.png");
	ball_texture = App->textures->Load("pinball/pokeball.png");
	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	PinballMap = App->textures->Load("pinball/bluetable_base.png");

	CreatePinball();
	// TODO: Homework - create a sensor

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	char title[50];
	sprintf_s(title, "Balls: %d Score: %06d", lives, score);
	App->window->SetTitle(title);

	if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 16, b2_dynamicBody));
		// TODO 8: Make sure to add yourself as collision callback to the circle you creates
	}

	// Prepare for raycast ------------------------------------------------------
	
	/*iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);*/

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	/*c = boxes.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}

	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}*/
	int ball_x, ball_y;
	ball->GetPosition(ball_x, ball_y);
	App->renderer->Blit(PinballMap, 0, 0);
	App->renderer->Blit(ball_texture, ball_x, ball_y, NULL, 1.0f, ball->GetRotation());

	if (ball_y > 1000)
	{
		ball->SetLinearSpeed(0, 0);
		ball->SetAngularSpeed(0);
		ball->SetPosition(400, 300);
		lives--;
	}

	if (lives <= 0)
		lives = 3;


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::CreatePinball()
{

	//borders of the map
	/*
	int PinballTable[50] = {
		669, 645,
		510, 63,
		489, 38,
		459, 23,
		423, 10,
		274, 10,
		229, 23,
		193, 55,
		164, 170,
		184, 241,
		170, 256,
		152, 254,
		178, 349,
		156, 365,
		112, 363,
		62, 552,
		227, 648,
		412, 648,
		594, 548,
		550, 367,
		507, 370,
		478, 347,
		518, 236,
		530, 247,
		633, 647,
	
	};
	
	pinball.add(App->physics->CreatePinballChain(0, 0, PinballTable, 50));*/

	//the 2 triangles of the low part 
	int TrRight[10] = {
		438, 509,
		498, 482,
		481, 381,
		469, 381,
		431, 490
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, TrRight, 10));

	int TrLeft[10] = {
		188, 380,
		223, 500,
		215, 507,
		155, 472,
		174, 378
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, TrLeft, 10));

	//Upper left chain under the slide
	int ChainUnderSlide[26] = {
		306, 35,
		304, 53,
		269, 67,
		258, 124,
		245, 127,
		251, 57,
		229, 58,
		217, 127,
		205, 125,
		223, 41,
		242, 29,
		265, 23,
		303, 21
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, ChainUnderSlide, 26));

	//Upper right chain under the fan
	int ChainUnderFan[22] = {
		412, 65,
		414, 116,
		457, 174,
		465, 172,
		456, 101,
		483, 101,
		496, 171,
		471, 240,
		503, 170,
		483, 66,
		425, 67
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, ChainUnderFan, 22));

	//Fan
	int MiddleFan[20] = {
		290, 238,
		298, 180,
		333, 165,
		370, 181,
		371, 236,
		385, 238,
		376, 162,
		331, 148,
		288, 158,
		278, 226
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, MiddleFan, 20));

	int MiddlePartOfFan[8] = {
		325, 240,
		338, 241,
		338, 193,
		325, 192
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, MiddlePartOfFan, 8));

	//walls
	int LeftLowWall[10] = {
		137, 389,
		111, 508,
		213, 557,
		116, 504,
		143, 387
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, LeftLowWall, 10));

	int RightLowWall[10] = {
		521, 390,
		545, 508,
		434, 558,
		540, 503,
		518, 397
	};
	pinball.add(App->physics->CreatePinballChain(0, 0, RightLowWall, 10));

	//upper circles
	pinball.add(App->physics->CreateCircle(305, 98, 18, b2_staticBody));
	pinball.add(App->physics->CreateCircle(338, 76, 18, b2_staticBody));
	pinball.add(App->physics->CreateCircle(373, 100, 18, b2_staticBody));

	
	ball = App->physics->CreateCircle(400, 300, 40, b2_dynamicBody);
}

// TODO 8: Now just define collision callback for the circle and play bonus_fx audio
