#pragma once
#include <vector>
#include "Ui.h"
#include "GameSettings.h"
#include <memory>
#include "Card.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum class GameStateType
{
	GST_Menu,
	GST_Playing,
	GST_Pause,
	GST_Won,
	GST_Gameover,
	GST_Settings,
	GST_None
};

class Game
{
public:
	~Game();

	void InitGame();
	void Update(float deltaTime, const sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window);
	void HandleClick(sf::RenderWindow& window);

	void setState(GameStateType state) { currentGameState = state; }
	void setMusicVolume(float volume) { backgroundMusic.setVolume(volume); }
	void updateVolume();
	GameStateType getState() const { return currentGameState; }
	Menu& GetMenu() { return menu; }
	GameSettings& GetSettings() { return settings; }
	void ApplySettings()
	{
		backgroundMusic.setVolume(settings.getVolume());
		flipSound.setVolume(settings.getVolume());
		matchSound.setVolume(settings.getVolume());
		nomatchSound.setVolume(settings.getVolume());
		gameOverSound.setVolume(settings.getVolume());
	}

	// Ресурсы
	sf::Texture CardAnimTexture;
	sf::Texture FrontCardTexture;
	sf::Texture BackCardTexture;
	sf::Texture TextureBackground;
	sf::Sprite SpriteBackground;
	std::vector<std::unique_ptr<Card>> CardsVector;

	// Звуковые эффекты
	sf::SoundBuffer flipSoundBuffer;
	sf::SoundBuffer matchSoundBuffer;
	sf::SoundBuffer nomatchSoundBuffer;
	sf::SoundBuffer gameOverSoundBuffer;

	sf::Sound flipSound;
	sf::Sound matchSound;
	sf::Sound nomatchSound;
	sf::Sound gameOverSound;

	sf::Music backgroundMusic;

private:
	GameStateType currentGameState = GameStateType::GST_None;
	GameSettings settings;

	std::vector<int> GeneratePairs(int pairsCount);
	void CheckMatch();

	int openedCardsCount = 0;
	bool isWaiting = false;
	float flipTimer = 1.f;
	float flipCooldown = 1.5f;
	float gameTimer = 60.0f;

	Menu menu;
	std::vector<sf::Texture> frontTextures;
};