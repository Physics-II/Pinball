#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "ModuleSceneIntro.h"
#include "p2Point.h"
#include "math.h"
#include "Box2D\Box2D\Box2D.h"
#include "Box2D\Box2D\Dynamics\Joints\b2joint.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = true;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);
	
	
	// needed to create joints like mouse joint
	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	//---Set background chain here---
	//MUST BE STATIC!!

	// Pivot 0, 0
	int background_ch[138] = {
		222, 554,
		234, 533,
		251, 522,
		276, 515,
		307, 504,
		327, 495,
		345, 486,
		345, 323,
		323, 312,
		307, 296,
		318, 277,
		336, 259,
		345, 250,
		345, 165,
		322, 110,
		287, 69,
		250, 48,
		211, 37,
		167, 38,
		138, 44,
		107, 59,
		77, 84,
		51, 113,
		42, 133,
		35, 130,
		48, 106,
		73, 77,
		101, 54,
		133, 38,
		162, 29,
		212, 30,
		252, 40,
		273, 54,
		289, 61,
		325, 103,
		350, 159,
		350, 479,
		377, 479,
		377, 175,
		358, 117,
		326, 66,
		291, 38,
		246, 17,
		206, 9,
		154, 10,
		120, 19,
		73, 44,
		36, 74,
		16, 103,
		5, 135,
		14, 157,
		19, 172,
		9, 194,
		10, 232,
		23, 256,
		50, 274,
		64, 293,
		43, 316,
		30, 325,
		30, 493,
		71, 508,
		119, 522,
		143, 536,
		147, 553,
		-1, 553,
		-1, 0,
		551, 0,
		552, 553,
		235, 552
	};

	// Pivot 0, 0
	int bar_1[40] = {
		318, 431,
		317, 335,
		309, 343,
		307, 418,
		263, 448,
		262, 455,
		266, 460,
		273, 460,
		298, 444,
		311, 436
	};

	int bar_2[20] = {
		56, 337,
		56, 432,
		103, 462,
		110, 463,
		114, 462,
		114, 458,
		113, 449,
		67, 418,
		67, 346,
		61, 337
	};

	// Pivot 0, 0
	int triangle_1[20] = {
		85, 353,
		85, 402,
		93, 405,
		101, 414,
		106, 419,
		113, 425,
		120, 426,
		121, 414,
		104, 383,
		89, 356
	};

	int triangle_2[16] = {
		289, 354,
		289, 402,
		276, 411,
		260, 425,
		254, 425,
		254, 414,
		269, 385,
		285, 355
	};
	
	// Pivot 0, 0
	int ovni[44] = {
		78, 133,
		88, 125,
		93, 105,
		110, 91,
		138, 78,
		165, 69,
		195, 67,
		222, 70,
		250, 81,
		271, 95,
		284, 108,
		285, 122,
		297, 132,
		299, 144,
		292, 151,
		269, 152,
		218, 153,
		205, 144,
		173, 142,
		162, 151,
		86, 151,
		78, 141
	};

	// Pivot 0, 0
	int left_kicker[20] = {
		105, 449,
		113, 447,
		126, 458,
		160, 488,
		161, 493,
		156, 497,
		150, 494,
		114, 471,
		101, 460,
		101, 454
	};

	// Pivot 0, 0
	int right_kicker[20] = {
		262, 448,
		242, 465,
		215, 488,
		215, 493,
		218, 496,
		224, 496,
		252, 477,
		273, 462,
		275, 456,
		271, 450
	};

	App->physics->CreateStaticChain(0, 0, background_ch, 138);
	App->physics->CreateStaticChain(0, 0, triangle_1, 20);
	App->physics->CreateStaticChain(0, 0, triangle_2, 16);
	App->physics->CreateStaticChain(0, 0, ovni, 44);

	//l_bar = App->physics->CreateRectangle;
	r_bar = App->physics->CreateStaticChain(0, 0, bar_2, 20);
	l_kicker = App->physics->CreateChain(0, 0, left_kicker, 20); //dyn
	r_kicker = App->physics->CreateChain(0, 0, right_kicker, 20); //dyn

	/*b2RevoluteJointDef revoluteJointDef;
	revoluteJointDef.bodyA = l_kicker->body;
	revoluteJointDef.bodyB = l_bar->body;
	revoluteJointDef.collideConnected = false;
	revoluteJointDef.localAnchorA.Set(PIXEL_TO_METERS(111), PIXEL_TO_METERS(457));
	revoluteJointDef.localAnchorB.Set(PIXEL_TO_METERS(111), PIXEL_TO_METERS(457));
	revoluteJointDef.referenceAngle = 0;
	revoluteJointDef.upperAngle = (3.14 / 4);
	revoluteJointDef.lowerAngle = 0;*/

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);
	
	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}



/*PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;

	return pbody;
}*/



PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateStaticChain(int x, int y, int* points, int size)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateLoop(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

// 
update_status ModulePhysics::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if (!debug)
		return UPDATE_CONTINUE;

	bool hit = false;
	b2Body* clicked = nullptr;
	b2Vec2 pos, pos2;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				b2Vec2 pos = f->GetBody()->GetPosition();
				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			} //switch

			//create mouse joint here!
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				pos.x = PIXEL_TO_METERS(App->input->GetMouseX());
				pos.y = PIXEL_TO_METERS(App->input->GetMouseY());

				if (f->TestPoint(pos))
				{
					hit = true;
					clicked = f->GetBody();
				}
			}
		}
	}
			
				b2MouseJointDef def;

				if (hit)
				{
					def.bodyA = ground;
					def.bodyB = clicked;
					def.target = pos;
					def.dampingRatio = 0.5f;
					def.frequencyHz = 2.0f;
					def.maxForce = 100.0f * clicked->GetMass();

					mouse_joint = (b2MouseJoint*)world->CreateJoint(&def);
				}

				if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
				{
					mouse_joint->SetTarget({ PIXEL_TO_METERS(App->input->GetMouseX()), PIXEL_TO_METERS(App->input->GetMouseY()) });
					App->renderer->DrawLine((App->input->GetMouseX()), (App->input->GetMouseY()), METERS_TO_PIXELS(mouse_joint->GetAnchorB().x), METERS_TO_PIXELS(mouse_joint->GetAnchorB().y), 125, 0, 125);

				}

				if (mouse_joint && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
				{
					world->DestroyJoint(mouse_joint);
					mouse_joint = nullptr;
				} //fixture for

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	if(physA && physA->listener != NULL)
		physA->listener->OnCollision(physA, physB);

	if(physB && physB->listener != NULL)
		physB->listener->OnCollision(physB, physA);
}