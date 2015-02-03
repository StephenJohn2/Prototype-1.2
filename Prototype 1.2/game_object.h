#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

#include <graphics/sprite.h>
#include <Box2D\Box2D.h>
#include "box2d_helpers.h"
#include <stdlib.h>
#include <time.h>
#include <input/vita/sony_controller_input_manager_vita.h>
#include <iostream>

class GameObject : public abfw::Sprite
{
public:
	GameObject();
	virtual ~GameObject();

	//object Type
	enum ObjectType {PLAYER, PLATFORM, ENEMY, PICKUP, WALL, PLANT, SPIKE, BLADE};//used for checking what type of objects are colliding
	ObjectType type_class;

	
	b2Body* body;//physaical body of object
	bool dead;//flag for killing 
	bool destroyed;// to check if object has removed from game

	virtual b2Body* getBody() {
		return body;
	};

//	virtual void setBody(b2Body* newBody) {
//		body = newBody;
//	};

protected:
	abfw::Vector2 velocity;
	

	abfw::Vector2 bodyInitialPosition;
	float body_half_width;
	float body_half_height;
	b2Vec2 force;//force for causing movement
	float move_v;//speed at which bodies move
	
	float uv_x,uv_y, uv_width,uv_height;//initial uv values
};

#endif // _GAME_OBJECT_H