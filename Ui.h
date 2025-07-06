#pragma once
#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <fstream>

enum class GameStateType;

class Menu
{
public:
	void InitMenu();
	void MenuHUD();
	void UpdateHUD(float gameTimer, GameStateType currentGameState);
	void addScore(int points) { score += points; }
	void finalizeGame(bool isWin);
	void loadHighScore();
	void saveHighScore() const;
	const sf::Font& getFont() const { return mainfont; }
	sf::Text& GetStartGameText() { return startgameText; }
	sf::Text& GetSettingsText() { return settingsText; }
	sf::Text& GetExitText() { return exitText; }
	void HandleHover(const sf::RenderWindow& window);
	void ResetGame();
	void Draw(sf::RenderWindow& window, GameStateType currentGameState);
	void setLastScore(int points);
	int getScore() const { return score; }
	int getHighScore() const { return highScore; }
	int getLastScore() const { return lastScore; }
	sf::Sprite getSpriteMenu() const { return MenuSprite; }

private:
	void CenterText(sf::Text& text, float yPosition);

	int score = 0;
	int lastScore = 0;
	int highScore = 0;
	bool isStartGameHovered = false;
	bool isSettingsHovered = false;
	bool isExitHovered = false;

	sf::Texture MenuTexture;
	sf::Sprite MenuSprite;
	sf::Font titlefont;
	sf::Font mainfont;

	sf::Text timerText;
	sf::Text scoreText;
	sf::Text titleText;
	sf::Text startgameText;
	sf::Text settingsText;
	sf::Text exitText;
	sf::Text winText;
	sf::Text looseText;
};