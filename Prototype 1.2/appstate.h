#pragma once

namespace abfw
{
	class ControllerInputManager;
	class AudioManager;
	class Font;
	class SpriteRenderer;
	class Platform;
	class Application;
	class Texture;
}

class GameApplication;

enum GAMESTATE {INTRO, MENU, GAME}; // possible gamestates (globally scoped so that the application class and every state can utilize the type)

class AppState
{
public:

	virtual void InitializeState() = 0;
	virtual void TerminateState() = 0;
	virtual GAMESTATE Update(const float& ticks_, const int& frame_counter_, const abfw::ControllerInputManager& controller_manager_) = 0; // returns state to change app to
	virtual void Render(const float frame_rate_, abfw::Font& font_, abfw::SpriteRenderer* sprite_renderer_) = 0;
	
protected:
	AppState(abfw::Platform* platform, const GameApplication* application, abfw::AudioManager* audio_manager) :
	platform_(platform),
	application_(application),
	audio_manager_(audio_manager)
	{
	}
	
	virtual ~AppState()
	{
	}

	class abfw::Platform* platform_;
	class abfw::AudioManager* audio_manager_;
	const class GameApplication* application_;
};