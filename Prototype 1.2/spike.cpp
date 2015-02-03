#include "spike.h"


Spike::Spike()
{
	type_class = SPIKE;
}


void Spike::CreateSpike(b2World* world_, float x , float y, float width, float height)
{
	dead = false;

	body_half_width = width;
	body_half_height = height;

	//set position
	bodyInitialPosition.x = x;
	bodyInitialPosition.y = y;

	// setup the plant definition
	b2BodyDef spike_body_def;
	spike_body_def.position.x = bodyInitialPosition.x;
	spike_body_def.position.y = bodyInitialPosition.y;

	// create the plant
	body = world_->CreateBody(&spike_body_def);

	// set the shape for the plant
	b2PolygonShape plant_shape;
	plant_shape.SetAsBox(body_half_width, body_half_height);

	// bind the shape to the body
	body -> CreateFixture(&plant_shape, 0.0f);

	//set plant sprite size
	set_width(BOX2D_GFX_SIZE(2*body_half_width));
	set_height(BOX2D_GFX_SIZE(2*body_half_height));

	//set plant sprite position
	float spike_x = BOX2D_GFX_POS_X(body->GetPosition().x);
	float spike_y = BOX2D_GFX_POS_Y(body->GetPosition().y);

	body->SetUserData(this);

	set_colour(0xff0000ff);

	set_position(abfw::Vector3(spike_x,spike_y,1));
}