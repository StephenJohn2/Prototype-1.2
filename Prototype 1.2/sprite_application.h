#ifndef _SPRITE_APPLICATION_H
#define _SPRITE_APPLICATION_H

#include <system/application.h>
#include "game_object.h"
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <input/sony_controller_input_manager.h>
#include <Box2D/Box2D.h>
#include <system/platform.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>

#include <iostream>
#include <time.h>

#include "box2d_helpers.h"
#include "player.h"
#include "platforms.h"
#include "Contact_listener.h"
#include "enemy.h"
#include "collectible.h"
#include "plant.h"
#include "spike.h"
#include "blade.h"

//total number of each objects used for array size
#define PLATFORM_NUM 8
#define PICKUP_NUM 10
#define PLANT_NUM 5
#define SPIKE_NUM 4

namespace abfw
{
	class Platform;
}

class SpriteApplication : public abfw::Application
{
public:
	SpriteApplication(abfw::Platform& platform);
	~SpriteApplication();
	void Init();
	void CleanUp();
	bool Update(float ticks);//game loop
	void Render();//draw sprites
	void Destroy();//remove enemies
	void Restart();//destroy enemies and make set them back to start
	void CreateObjects();//Create instances of each game object
	void PlantPickUps();//spawn a pickup from destroyed plants
	
	abfw::Texture* LoadTextureFromPNG(const char* filename);//loads texture
	

private:
	abfw::SonyControllerInputManager* controller_manager_;
	Contact_Listener contact_listener;
	
	//game objects
	GameObject background;
	Player player;
	Platform platform[PLATFORM_NUM];
	Enemy enemy;
	PickUp pickUp[PICKUP_NUM];
	Plant plant[PLANT_NUM];
	Spike spike[SPIKE_NUM];
	Blade blade;

	abfw::Sprite sprite;
	b2Body* game_object_body_;

	abfw::Font font_;
	abfw::SpriteRenderer* sprite_renderer_;
	float frame_rate_;
	b2World* world_;

	//textures for player
	abfw::Texture* player_tex;
	abfw::Texture* rotPlayerTex;
	//texture for pickups
	abfw::Texture* redPUTex;
	abfw::Texture* bluePUTex;
	abfw::Texture* platformTex;
	//textures for plants
	abfw::Texture* plantWallTex;
	abfw::Texture* plantBlockTex;
	abfw::Texture* rotPlantBlockTex;
	abfw::Texture* bGTex;

	float platform_width;
	float score;
	bool gameOver;

	float attackTime;
};

#endif // _SPRITE_APPLICATION_H