#include "enemy.h"


Enemy::Enemy()
{
	//set object type
	type_class = ENEMY;
	
	//initalise values
	move_v = 1;

	
}

void Enemy::Create_Enemy(b2World* world_,float x , float y)
{
	//start bools as false
	dead = false;
	destroyed = false;

	//starting position
	bodyInitialPosition.x = x;
	bodyInitialPosition.y = y;

	//size
	body_half_width = 0.25f;
	body_half_height = 0.25f;

	//assigning values to body
	b2BodyDef enemy_bodyDef;
	enemy_bodyDef.type = b2_dynamicBody;
	enemy_bodyDef.position.x = bodyInitialPosition.x;
	enemy_bodyDef.position.y = bodyInitialPosition.y;
	body = world_->CreateBody(&enemy_bodyDef);

	b2PolygonShape enemy_Box;
	enemy_Box.SetAsBox(body_half_width, body_half_height);

	b2FixtureDef enemy_fixtureDef;
	enemy_fixtureDef.shape = &enemy_Box;
	enemy_fixtureDef.density = 1.0f;
	enemy_fixtureDef.friction = 0.5f;
	enemy_fixtureDef.restitution = 0.1f; // bouncieness
	body->CreateFixture(&enemy_fixtureDef);


	//set sprite size to match body
	set_width(BOX2D_GFX_SIZE(2*body_half_width));
	set_height(BOX2D_GFX_SIZE(2*body_half_height));

	body->SetUserData(this);

	//set colour red
	set_colour(0xff0000ff);

}

void Enemy::Update_Enemy(float ticks)
{
	//update sprite position to match body
	float enemy_new_x = BOX2D_GFX_POS_X(body->GetPosition().x);
	float enemy_new_y = BOX2D_GFX_POS_Y(body->GetPosition().y);

	set_position(abfw::Vector3(enemy_new_x,enemy_new_y,0.f));
	set_rotation(-body->GetAngle());
}

