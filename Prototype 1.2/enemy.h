#ifndef ENEMY_H
#define ENEMY_H

#include "game_object.h"

class Enemy : public GameObject
{
	public:
		//functions
		Enemy();//constructor
		void Create_Enemy(b2World* world_, float x, float y);// create enemy at inputted position
		void Update_Enemy(float);//update enemy
	
		//varaibles
};
#endif