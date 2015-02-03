#include "sprite_application.h"


SpriteApplication::SpriteApplication(abfw::Platform& platform) :
	abfw::Application(platform),
	sprite_renderer_(NULL),
	controller_manager_(NULL)
{
	platform_width = GFX_BOX2D_SIZE(10);
}

SpriteApplication::~SpriteApplication()
{
}

void SpriteApplication::Init()
{
	// load the font to draw the on-screen text
	bool font_loaded = font_.Load("comic_sans", platform_);
	if(!font_loaded)
	{
		std::cout << "Font failed to load." << std::endl;
		exit(-1);
	}

	// create a sprite renderer to draw the sprites
	sprite_renderer_ = platform_.CreateSpriteRenderer();

	// create a controller manager to receive controller input
	controller_manager_ = platform_.CreateSonyControllerInputManager();

	// define gravity vector
	b2Vec2 gravity(0.0f, 0.0f);

	// create the box2D physics world
	world_ = new b2World(gravity);
	world_->SetContactListener(&contact_listener);

	//load textures
	player_tex = LoadTextureFromPNG("Character_2.png");
	rotPlayerTex = LoadTextureFromPNG("Character_rotate.png");
	redPUTex = LoadTextureFromPNG("Red.png");
	bluePUTex = LoadTextureFromPNG("Blue.png");
	platformTex = LoadTextureFromPNG("Platform_Panel.png");
	plantWallTex = LoadTextureFromPNG("Plant_Wall.png");
	plantBlockTex = LoadTextureFromPNG("Plant_Block.png");
	rotPlantBlockTex = LoadTextureFromPNG("Plant_Block_rot.png");
	bGTex = LoadTextureFromPNG("Level_BG.png");

	//title screen
	background.set_height(platform_.height());
	background.set_width(platform_.width());
	background.set_texture(bGTex);
	background.set_position(abfw::Vector3(platform_.width()*0.5f,platform_.height()*0.5f,1));

	//set textures
	//for player
	player.set_texture(player_tex);
	//alternate pickup textures
	for(int g = 0; g < PICKUP_NUM;g+=2)
	{
		pickUp[g].set_texture(redPUTex);
		pickUp[g+1].set_texture(bluePUTex);	
	}

	//for platfroms
	for (int q = 0;q < PLATFORM_NUM; q++)
	{
		platform[q].set_texture(platformTex);
	}

	//for plat walls
	plant[0].set_texture(plantWallTex);
	plant[1].set_texture(plantWallTex);

	//for plat blocks
	plant[2].set_texture(rotPlantBlockTex);
	plant[3].set_texture(rotPlantBlockTex);
	plant[3].set_uv_width(-1.0f);
	plant[4].set_texture(plantBlockTex);
	plant[4].set_uv_height(-1.0f);

	//create object
	CreateObjects();

	score = 0;
	gameOver = false;
}

void SpriteApplication::CreateObjects()
{
	player.Create_Player(world_, platform_.width(), platform_.height());
//	enemy.Create_Enemy(world_, GFX_BOX2D_POS_X(platform_.width()*0.65f),GFX_BOX2D_POS_Y(platform_.height()-30));

	//create walls
	//boundries
	platform[0].Create_platform(world_,GFX_BOX2D_POS_X(0),GFX_BOX2D_POS_Y(0),GFX_BOX2D_SIZE(platform_.width()),platform_width);//roof
	platform[1].Create_platform(world_,GFX_BOX2D_POS_X(0),GFX_BOX2D_POS_Y(platform_.height()),GFX_BOX2D_SIZE(platform_.width()),platform_width);//ground
	platform[2].Create_platform(world_,GFX_BOX2D_POS_X(0),GFX_BOX2D_POS_Y(platform_.height()),platform_width,GFX_BOX2D_SIZE(platform_.height()));//left wall
	platform[3].Create_platform(world_,GFX_BOX2D_POS_X(platform_.width()),GFX_BOX2D_POS_Y(0),platform_width,GFX_BOX2D_SIZE(platform_.height()));//right wall

	platform[4].Create_platform(world_,GFX_BOX2D_POS_X(platform_.width()*0.9f),GFX_BOX2D_POS_Y(platform_.height()*0.9f),
		GFX_BOX2D_SIZE(platform_.width()*0.125f),GFX_BOX2D_SIZE(platform_.height()*0.125f));	//bottom right
	platform[5].Create_platform(world_,GFX_BOX2D_POS_X(platform_.width()*0.175f),GFX_BOX2D_POS_Y(platform_.height()*0.95f),
		GFX_BOX2D_SIZE(platform_.width()*0.25f),GFX_BOX2D_SIZE(platform_.height()*0.08f));//botttom left
	platform[6].Create_platform(world_,GFX_BOX2D_POS_X(platform_.width()*0.325f),GFX_BOX2D_POS_Y(platform_.height()*0.36f),
		GFX_BOX2D_SIZE(platform_.width()*0.025f),GFX_BOX2D_SIZE(platform_.height()*0.35f));//divider
	platform[7].Create_platform(world_,GFX_BOX2D_POS_X(platform_.width()*0.22f),GFX_BOX2D_POS_Y(platform_.height()*0.63f),
		GFX_BOX2D_SIZE(platform_.width()*0.1f),GFX_BOX2D_SIZE(platform_.height()*0.08f));//tunnel roof
	
	//create pickups on ceiling
	pickUp[0].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.75f),GFX_BOX2D_POS_Y(30));
	pickUp[1].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.7f),GFX_BOX2D_POS_Y(30));
	pickUp[2].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.65f),GFX_BOX2D_POS_Y(30));
	pickUp[3].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.6f),GFX_BOX2D_POS_Y(30));
	pickUp[4].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.55f),GFX_BOX2D_POS_Y(30));
	pickUp[5].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.5f),GFX_BOX2D_POS_Y(30));
	pickUp[9].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.8f),GFX_BOX2D_POS_Y(30));

	//create plants
	//tunnel blockers
	plant[0].CreatePlant(world_,GFX_BOX2D_POS_X(platform_.width()*0.16f),GFX_BOX2D_POS_Y(platform_.height()*0.82f),GFX_BOX2D_SIZE(20)
		,GFX_BOX2D_SIZE(platform_.height()*0.12f));
	plant[1].CreatePlant(world_,GFX_BOX2D_POS_X(platform_.width()*0.29f),GFX_BOX2D_POS_Y(platform_.height()*0.82),GFX_BOX2D_SIZE(20),
		GFX_BOX2D_SIZE(platform_.height()*0.12f));
	//treasure holders
	plant[2].CreatePlant(world_,GFX_BOX2D_POS_X(30),GFX_BOX2D_POS_Y(platform_.height()*0.325),GFX_BOX2D_SIZE(20),
		GFX_BOX2D_SIZE(20));//left
	plant[3].CreatePlant(world_,GFX_BOX2D_POS_X(platform_.width()*0.28f),GFX_BOX2D_POS_Y(platform_.height()*0.325),GFX_BOX2D_SIZE(20),
		GFX_BOX2D_SIZE(20));//right
	plant[4].CreatePlant(world_,GFX_BOX2D_POS_X(platform_.width()*0.16f),GFX_BOX2D_POS_Y(30),GFX_BOX2D_SIZE(20)
		,GFX_BOX2D_SIZE(20));//top

	//create spike
	//floor
	spike[0].CreateSpike(world_,GFX_BOX2D_POS_X(platform_.width()*0.575f),GFX_BOX2D_POS_Y(platform_.height()-15),
		GFX_BOX2D_SIZE(platform_.width()*0.2f),GFX_BOX2D_SIZE(5));

	//top left
	spike[1].CreateSpike(world_,GFX_BOX2D_POS_X(0),GFX_BOX2D_POS_Y(0),
		GFX_BOX2D_SIZE(40),GFX_BOX2D_SIZE(40));
	spike[1].set_rotation(40);

	//top right
	spike[2].CreateSpike(world_,GFX_BOX2D_POS_X(platform_.width()*0.32f),GFX_BOX2D_POS_Y(5),
		GFX_BOX2D_SIZE(40),GFX_BOX2D_SIZE(40));
	spike[2].set_rotation(40);

	//bottom right
	spike[3].CreateSpike(world_,GFX_BOX2D_POS_X(platform_.width()*0.31f),GFX_BOX2D_POS_Y(platform_.height()*0.545f),
		GFX_BOX2D_SIZE(25),GFX_BOX2D_SIZE(25));
	spike[3].set_rotation(40);
}

void SpriteApplication::CleanUp()
{
	// free up the sprite renderer
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	// free up physics world
	delete world_;
	world_ = NULL;

	// free up controller manager
	delete controller_manager_;
	controller_manager_ = NULL;
}

bool SpriteApplication::Update(float ticks)
{
	// calculate the frame rate
	frame_rate_ = 1.0f / ticks;

	controller_manager_->Update();

		// update physics world
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// update physics
	world_->Step(timeStep, velocityIterations, positionIterations);


	// get the first controller attached to the hardware platform
	// we only have one on PS Vita
	const abfw::SonyController* controller = controller_manager_->GetController(0);

	// check we have a valid controller object (one that isn't NULL)
	if(controller)
	{
		if ((controller->buttons_down() & (ABFW_SONY_CTRL_START | ABFW_SONY_CTRL_SELECT)) == (ABFW_SONY_CTRL_START | ABFW_SONY_CTRL_SELECT))
		{
			std::cout << "start and select pressed" << std::endl;
		}
	}

	//update player if alive
	if(player.dead != true)
	{
		player.Update(controller_manager_,ticks, gameOver);
	}
	else
	{
		Restart();
	}
	
	//create and update blade
	if(player.attacking == true)
	{
		if(blade.created == false)
		{
			blade.Create(world_, player);
			attackTime = 0;
		}
	}
	else if (blade.destroyed == false && blade.created == true && attackTime > 20)
	{
		//destroy blade
		world_->DestroyBody(blade.body);
		blade.body = NULL;
		blade.destroyed = true;
		blade.created = false;
	}
	
	if(blade.created == true)
	{
		blade.Update(ticks,player);
		attackTime ++;
	}

	//update enemy if alive
/*	if(enemy.dead != true)
	{
		enemy.Update_Enemy(ticks);
	}
	else
	{
		Destroy();
	}*/

	//remove from game if collected
	for(int i = 0;i < PICKUP_NUM;i++)
	{
		if(pickUp[i].dead == true && pickUp[i].destroyed == false)
		{
			//destroy enemy
			world_->DestroyBody(pickUp[i].body);
			pickUp[i].body = NULL;
			pickUp[i].destroyed = true;
			//increase score
			score+=1;
		}
		else if(pickUp[i].dead == false)
		{
			pickUp[i].Update(ticks);
		}
	}

	//change sprite for horizontal or vertical movement
	if (player.horizontal == false)
	{
		player.set_texture(rotPlayerTex);
	}
	else
	{
		player.set_texture(player_tex);
	}

	//check for destroyed plants
	for(int g = 0; g < PLANT_NUM;g++)
	{
		if(plant[g].dead == true && plant[g].destroyed == false)
		{
			world_->DestroyBody(plant[g].body);
			plant[g].body = NULL;
			plant[g].destroyed = true;
		}
	}
	
	//span extra pickup from certain plants
	PlantPickUps();

	//win condition
	if(score == PICKUP_NUM)
	{
		gameOver = true;
	}

	//manually restart
	if (controller->buttons_down() & ABFW_SONY_CTRL_START )
	{
		Restart();
	}
	

	// return if the application is still running
	// this should always be true on PS Vita
	return true;
}

void SpriteApplication::Render()
{
	// set up sprite renderer for drawing
	sprite_renderer_->Begin();

	//
	// draw all sprites here
	//

	//sprite_renderer_->DrawSprite(background);//must be done first so transparent background

	if(player.dead == false)
	{
		sprite_renderer_->DrawSprite(player);
		if(blade.destroyed == false)
		{
			sprite_renderer_->DrawSprite(blade);
		}
	}


/*	if(enemy.dead == false)
	{
		sprite_renderer_->DrawSprite(enemy);
	}*/

	for(int u = 0; u < PICKUP_NUM; u++)
	{
		if(pickUp[u].dead == false)
		{
			sprite_renderer_->DrawSprite(pickUp[u]);
		}
	}

	for(int g = 0; g < PLANT_NUM; g++)
	{
		if(plant[g].dead == false)
		{
			sprite_renderer_->DrawSprite(plant[g]);
		}
	}

	
	for(int a = 0; a < SPIKE_NUM; a++)
	{
		sprite_renderer_->DrawSprite(spike[a]);
	}

	//draw platfroms
	for (int wall_render = 0;wall_render < PLATFORM_NUM;wall_render++)
	{
		//platform[wall_render].set_texture(platform_tex);//set texture
		sprite_renderer_->DrawSprite(platform[wall_render]);
	}


//	font_.RenderText(sprite_renderer_, abfw::Vector3(10.0f, 5.0f, -0.9f), 1.0f, 0xff00ff00, abfw::TJ_LEFT, "Project Zero");
	font_.RenderText(sprite_renderer_, abfw::Vector3(815.0f, 40.0f, -0.9f), 1.0f, 0xff00ffff, abfw::TJ_LEFT, "health  : %.0f", player.health);	//display player health
	font_.RenderText(sprite_renderer_, abfw::Vector3(815.0f, 5.0f, -0.9f), 1.0f, 0xff00ffff, abfw::TJ_LEFT, "Score : %.0f", score);//display player score
	font_.RenderText(sprite_renderer_, abfw::Vector3(850.0f, 510.0f, -0.9f), 1.0f, 0xff00ffff, abfw::TJ_LEFT, "FPS: %.1f", frame_rate_);

	if(gameOver == true)
	{
		font_.RenderText(sprite_renderer_, abfw::Vector3(350.0f,200.0f,0.9f), 3.0f, 0xff00ffff, abfw::TJ_LEFT, "YOU WIN!!");
		font_.RenderText(sprite_renderer_, abfw::Vector3(350.0f,250.0f,0.9f), 3.0f, 0xff00ffff, abfw::TJ_LEFT, "Press Start");
		font_.RenderText(sprite_renderer_, abfw::Vector3(350.0f,300.0f,0.9f), 3.0f, 0xff00ffff, abfw::TJ_LEFT, "to replay");
	}

	// tell sprite renderer that all sprites have been drawn
	sprite_renderer_->End();
}

void SpriteApplication::Destroy()
{
	if(enemy.destroyed == false)
	{
		world_->DestroyBody(enemy.body);
		enemy.body = NULL;
		enemy.destroyed = true;
	}
}

void SpriteApplication::Restart()
{
	//destroy dead player
	if(player.destroyed == false)
	{
		world_->DestroyBody(player.body);
		player.body = NULL;
		player.destroyed = true;
	}

	Destroy();//enemies

	//destroy pickups
	for ( int y = 0; y < PICKUP_NUM; y++)
	{
		pickUp[y].dead = true;
		pickUp[y].spawned = false;//reset for another play
		if(pickUp[y].destroyed == false)
		{
			world_->DestroyBody(pickUp[y].body);
			pickUp[y].body = NULL;
			pickUp[y].destroyed = true;
		}
	}

	for(int s = 0; s < PLANT_NUM; s++)
	{
		//destroy plants
		if(plant[s].destroyed == false)
		{
			world_->DestroyBody(plant[s].body);
			plant[s].body = NULL;
			plant[s].destroyed = true;
		}
	}

	score = 0;

	//create new versions at starting position
	CreateObjects();

	gameOver = false;
}

void SpriteApplication::PlantPickUps()
{
	//left plant
	if(plant[2].destroyed == true && pickUp[6].spawned == false)
	{
		pickUp[6].Create_pickup(world_,GFX_BOX2D_POS_X(30),GFX_BOX2D_POS_Y(platform_.height()*0.325));
		pickUp[6].spawned = true;
	}

	//right plant
	if(plant[3].destroyed == true && pickUp[7].spawned == false)
	{
		pickUp[7].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.26f),GFX_BOX2D_POS_Y(platform_.height()*0.325));
		pickUp[7].spawned = true;
	}

	//top plant
	if(plant[4].destroyed == true && pickUp[8].spawned == false)
	{
		pickUp[8].Create_pickup(world_,GFX_BOX2D_POS_X(platform_.width()*0.16f),GFX_BOX2D_POS_Y(30));
		pickUp[8].spawned = true;
	}

}

abfw::Texture* SpriteApplication::LoadTextureFromPNG(const char* filename)
{
	abfw::PNGLoader png_loader;
	abfw::ImageData image_data;
	png_loader.Load(filename, platform_, image_data);
	
	if(image_data.image() == NULL)
	{
		return NULL;
	}
	else
	{
		abfw::Texture* texture;
		texture = platform_.CreateTexture(image_data);
		return texture;
	}
}
