#ifndef PLAYER_H
#define PLAYER_H

#include "game_object.h"

class Player : public GameObject
{
	public:
		//functions
		Player();//constructor
		void Create_Player(b2World* world_, float width, float height);// make player body and match it to sprite
		void Update(abfw::SonyControllerInputManager* controller_manager_, float, bool gameOver);//updates player position
		void Player_Input(abfw::SonyControllerInputManager* controller_manager_);//takes control input
		void Knockback(float,float);//knock player back when they take damage

		//varaibles
		b2Vec2 gravity;//personal gravity

		float jump_v;//jump force
		float jump_gap;//time between being able to jump
		float health;//player health

		bool attacking;//attacking
		bool hurting;//is player taking damage
		bool gameOver;//has game stopped
		bool horizontal;	
		bool move_right,moveUp;//flag for telling player direction used to change animation/place blade
		bool grounded;//player is touching a platform
		b2Vec2 currentPos;

	private:	
		float damage;//amount of damage taken
		b2Vec2 knockbackForce;//push player back when hit
		
		enum gravityDirection {UP, DOWN, LEFT, RIGHT};
		gravityDirection gDir;

		float jumpCutOff,moveCutOff;//how far the sticks have to be pushed to trigger action
};
#endif 