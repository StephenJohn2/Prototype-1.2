#ifndef BLADE_H
#define BLADE_H	

#include "game_object.h"
#include "player.h"

class Blade : public GameObject
{
	public:
		Blade();
		void Create(b2World* world_,Player player);
		void Update(float ticks, Player player);
		void alignFace(Player player);

		bool created;//stop multiple spawns of the object
	private:
		float xOffset,yOffset;//place blade outside of player
		float armLength;//put the blade a distance from player
		
};
#endif