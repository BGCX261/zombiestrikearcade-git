#define SCROLL_SPEED		80.7F//50.7F//47.0F

#include "IntroState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"

#include "AnimationManager.h"


/**************************************************************/
// GetInstance
/*static*/ IntroState* IntroState::GetInstance(void)
{
	static IntroState s_Instance;

	return &s_Instance;
}


/**************************************************************/
// Enter
/*virtual*/ void IntroState::Enter(void)
{
	// Set background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 });	// black


	//starting_y = Game::GetInstance()->GetScreenHeight() + 20.0F;// * 15.0F;// + 170.0F;



	// Load assets
	SGD::GraphicsManager*	pGraphics			= SGD::GraphicsManager::GetInstance();
	SGD::AudioManager*		pAudio				= SGD::AudioManager::GetInstance();
	AnimationManager*		pAnimationManager	= AnimationManager::GetInstance();

	//pAnimationManager->Load("resource/config/animations/Zombie_Animation_New.xml", "zombie");
	//animation.m_strCurrAnimation = "zombie";

	pAudio->SetVoiceVolume(Game::GetInstance()->m_hMainVoice, 35);

	m_hBackgroundImage = pGraphics->LoadTexture("resource/graphics/MenuImages/emergencybroadcast.png");
	m_hinstruct = SGD::GraphicsManager::GetInstance()->LoadTexture("resource/graphics/MenuImages/ArcadeControlsIcons.png");

	m_hEmergency = pAudio->LoadAudio("resource/audio/zombieemergency.wav");

	if (IntroTimer.GetTime() < 45.0f)
	{
		float newTime = 45.0f - IntroTimer.GetTime();

		IntroTimer.AddTime(newTime);
	}

	else if (IntroTimer.GetTime() <= 0.0f)
		IntroTimer.AddTime(45.0f);

	if (ScreenTimer.GetTime() < .1f)
	{
		float newTime = .1f - ScreenTimer.GetTime();

		ScreenTimer.AddTime(newTime);
	}

	else if (ScreenTimer.GetTime() <= 0.0f)
		ScreenTimer.AddTime(.1f);

	transBack = 255;
	transTextFirst = 0;
	transText = 0;

	//IntroTimer.AddTime(45.0f - IntroTimer.GetTime());
	//ScreenTimer.AddTime(.1f - ScreenTimer.GetTime());

	pAudio->PlayAudio(m_hEmergency, false);

	//m_hBackgroundImage	= pGraphics->LoadTexture("resource/graphics/youLose.png");

	//m_hBackgroundMusic = pAudio->LoadAudio("resource/audio/JNB_Credits_PinballGroove.xwm");
	//pAudio->PlayAudio(m_hBackgroundMusic);

}


/**************************************************************/
// Exit
/*virtual*/ void IntroState::Exit(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	// Unload assets
	pGraphics->UnloadTexture(m_hBackgroundImage);
	pGraphics->UnloadTexture(m_hinstruct);

	//pAudio->UnloadAudio(m_hBackgroundMusic);

	pAudio->UnloadAudio(m_hEmergency);

	pAudio->SetVoiceVolume(Game::GetInstance()->m_hMainVoice, 100);
}


/**************************************************************/
// Input
/*virtual*/ bool IntroState::Input(void)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true || pInput->IsButtonPressed(0, 3) == true || pInput->IsButtonPressed(0, 6) == true)
	{

		//COMMENT IN WHEN AUDIO ADDED
		pAudio->StopAudio(m_hEmergency);

		Game::GetInstance()->RemoveState();
		Game::GetInstance()->AddState(GameplayState::GetInstance());
		return true;
	}



	// keep playing
	return true;
}


/**************************************************************/
// Update
/*virtual*/ void IntroState::Update(float elapsedTime)
{
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	if (IntroTimer.GetTime() < 0.0f)
	{
		pAudio->StopAudio(m_hEmergency);

		Game::GetInstance()->RemoveState();
		Game::GetInstance()->AddState(GameplayState::GetInstance());
	}

	if (IntroTimer.GetTime() < 30.0f)
	{
		if (ScreenTimer.GetTime() <= 0.0f && IntroTimer.GetTime() > 12.0f)
		{
			transBack -= 5;
			transTextFirst += 5;
			ScreenTimer.AddTime(.1f);
		}

		else if (IntroTimer.GetTime() < 12.0f)
		{
			if (ScreenTimer.GetTime() < 0.0f)
			{
				transText += 5;
				transTextFirst -= 5;
				ScreenTimer.AddTime(.1f);
			}

			//ScreenTimer.Update(elapsedTime);
		}
		
		ScreenTimer.Update(elapsedTime);
	}

	if (transBack <= 0)
	{
		transBack = 0;
	}

	if (transText >= 255)
	{
		transText = 255;
	}

	if (transTextFirst >= 255)
	{
		transTextFirst = 255;
	}

	if (transTextFirst < 0)
	{
		transTextFirst = 0;
	}

	IntroTimer.Update(elapsedTime);

	/*
	AnimationManager::GetInstance()->Update(animation, elapsedTime);

	if (starting_y == 120.0f)
	{
		Game::GetInstance()->RemoveState();
		Game::GetInstance()->AddState(GameplayState::GetInstance());
	}

	starting_y -= SCROLL_SPEED * elapsedTime;

	if (starting_y < 120.0f)
		starting_y = 120.0f;
	*/
}


/**************************************************************/
// Render
/*virtual*/ void IntroState::Render(void)
{
	SGD::GraphicsManager*	pGraphics = SGD::GraphicsManager::GetInstance();

	// Use the game's font
	const BitmapFont* pFont = Game::GetInstance()->GetFont();

	SGD::Color backColor(transBack, 255, 255, 255);
	// Draw the background image
	pGraphics->DrawTexture(m_hBackgroundImage, { 0, 0 }, 0.0f, {}, backColor);

	if (IntroTimer.GetTime() < 20.0f)
	{
		SGD::Color colorTextOne(transTextFirst, 100, 0, 0);

		pFont->Draw("2 Weeks Later", { Game::GetInstance()->GetScreenWidth() / 2 - 150, Game::GetInstance()->GetScreenHeight() / 2 - 25 }, 1.0f, colorTextOne);

		SGD::Color colorText(transText, 100, 0, 0);

		pFont->Draw("Trying to survive", { Game::GetInstance()->GetScreenWidth() / 2 - 200, Game::GetInstance()->GetScreenHeight() / 2 - 25}, 1.0f, colorText);
	}

	//if (SGD::InputManager::GetInstance()->IsControllerConnected(0) == false)
	//	pFont->Draw("Press 'ESC' to continue", { Game::GetInstance()->GetScreenWidth() / 2 - 175, Game::GetInstance()->GetScreenHeight() - 25 }, .5f, { 100, 0, 0 });
	//else
	//	pFont->Draw("Press 'Start' to continue", { Game::GetInstance()->GetScreenWidth() / 2 - 175, Game::GetInstance()->GetScreenHeight() - 25 }, .5f, { 100, 0, 0 });


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenWidth();

	// Display the mode title & its animation
	//pFont->Draw("Story Mode", { (width - (10 * 25 * 3.0f)) / 2, 10 }, 3.0f, { 255, 255, 255 });

	// Display skip input
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hinstruct, { Game::GetInstance()->GetScreenWidth() * 0.45f, Game::GetInstance()->GetScreenHeight() * 0.90f }, { 30.0f, 32.0f, 64.0f, 55.0f }, 0.0f, {}, {}, { 2.0f, 2.0f });
	pFont->Draw("Press ", { Game::GetInstance()->GetScreenWidth() * 0.35f, Game::GetInstance()->GetScreenHeight() * 0.9f }, 1.0f, { 255, 255, 255 });
	pFont->Draw(" to Skip ", { Game::GetInstance()->GetScreenWidth() * 0.55f, Game::GetInstance()->GetScreenHeight() * 0.9f }, 1.0f, { 255, 255, 255 });


	/*
	AnimationManager::GetInstance()->Render(animation, { width - 160.0f, 50 });



	// Setup text render data
	float left_start	= 10.0F;
	float y_offset		= 210.0f;
	float scale			= 1.0f;


	// story mode text
	std::string output1 = "HELLO WORLD!";
	std::string output2 = "GOODBYE WORLD!";


	pFont->Draw(output1.c_str(), { left_start, starting_y + (y_offset * 0) }, scale, { 0, 255, 0 });
	pFont->Draw(output2.c_str(), { left_start, starting_y + (y_offset * 1) }, scale, { 255, 0, 0 });
	*/
}
