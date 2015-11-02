#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleRender.h"
#include "ModuleInput.h"


ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	Ball.body = App->physics->CreateCircle(610, 747, 40, b2_dynamicBody);
	Ball.texture = App->textures->Load("pinball/pokeball.png");

	int r_f[14] = {
		386, 910,
		374, 911,
		315, 955,
		318, 965,
		325, 965,
		390, 937,
		394, 918
	};
	r_flipper.body = App->physics->CreatePolygon({ 0, 0, 0, 0 }, r_f, 14, b2_dynamicBody, 1.0f, 0.5f, false);

	int l_f[10] = {
		212, 911,
		269, 958,
		262, 968,
		191, 938,
		189, 915
	};
	l_flipper.body = App->physics->CreatePolygon({ 0, 0, 0, 0 }, l_f, 10, b2_dynamicBody, 1.0f, 0.5f, false);

	r_flipper_anchor = App->physics->CreateCircle(380, 930, 1, b2_staticBody);
	App->physics->CreateRevoluteJoint(r_flipper.body, r_flipper_anchor, 0, 0, 0, 0);

	l_flipper_anchor = App->physics->CreateCircle(220, 900, 1, b2_staticBody);
	App->physics->CreateRevoluteJoint(l_flipper.body, l_flipper_anchor, 0, 0, 0, 0);

	pusher.body = App->physics->CreateRectangle(590, 1100, 42, 80);
	pusher_anchor = App->physics->CreateCircle(570, 1040, 1, b2_staticBody);
	App->physics->CreateLineJoint(pusher.body, pusher_anchor, 1, 1, 0, 0, 15, 0.5);
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		r_flipper.body->Turn(360);
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		pusher.body->Push(0,-2200);
	}

	int ball_x, ball_y;
	Ball.body->GetPosition(ball_x, ball_y);
	App->renderer->Blit(Ball.texture, ball_x, ball_y, NULL, 1.0f, Ball.body->GetRotation());
	if (ball_y > 1000)
	{
		Ball.body->SetLinearSpeed(0, 0);
		Ball.body->SetAngularSpeed(0);
		Ball.body->SetPosition(610, 747);
		lives--;
	}

	if (lives <= 0)
		lives = 3;
	return UPDATE_CONTINUE;
}



