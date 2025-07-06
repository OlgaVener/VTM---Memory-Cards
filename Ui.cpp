#include "Ui.h"
#include "Game.h"
#include <iostream>

void Menu::InitMenu()
{
	if (!titlefont.loadFromFile(TITLEFONT_PATH)) {}
    if (!mainfont.loadFromFile(FONT_PATH)) {}
    if (!MenuTexture.loadFromFile(RESOURCES_PATH + "menubg.png")) {}
    
    MenuSprite.setTexture(MenuTexture);
    MenuSprite.setScale(1.22f, 1.f);
    MenuHUD();
    UpdateHUD(60.0f, GameStateType::GST_Menu);
    loadHighScore();
}

void Menu::CenterText(sf::Text& text, float yPosition)
{
	sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.0f,
		bounds.top + bounds.height / 2.0f);
	text.setPosition(SCREEN_WIDTH / 2.0f, yPosition);
}
void Menu::MenuHUD()
{
	titleText.setFont(titlefont);
	titleText.setString("VTM - Memory Cards");
	titleText.setCharacterSize(60);
	titleText.setFillColor(sf::Color::White);
	CenterText(titleText, 450.f);

	startgameText.setFont(mainfont);
	startgameText.setString("New Game");
	startgameText.setCharacterSize(40);
	startgameText.setFillColor(sf::Color::White);
	CenterText(startgameText, 550.f);

	settingsText.setFont(mainfont);
	settingsText.setString("Settings");
	settingsText.setCharacterSize(40);
	settingsText.setFillColor(sf::Color::White);
	CenterText(settingsText, 650.f);

	exitText.setFont(mainfont);
	exitText.setString("Exit");
	exitText.setCharacterSize(40);
	exitText.setFillColor(sf::Color::White);
	CenterText(exitText, 750.f);
}

void Menu::UpdateHUD(float gameTimer, GameStateType currentGameState)
{
	timerText.setFont(mainfont);
	scoreText.setFont(mainfont);

	timerText.setString("Time: " + std::to_string(static_cast<int>(gameTimer)));
	scoreText.setString("Score: " + std::to_string(score) + "\nHigh: " + std::to_string(highScore));

	timerText.setCharacterSize(35);
	scoreText.setCharacterSize(35);

	timerText.setFillColor(sf::Color::White);
	scoreText.setFillColor(sf::Color::White);

	timerText.setPosition(15.f, 15.f);
	scoreText.setPosition(SCREEN_WIDTH - 560.f, 850.f);

	winText.setFont(mainfont);
	looseText.setFont(mainfont);
	winText.setCharacterSize(60);
	looseText.setCharacterSize(60);
	winText.setFillColor(sf::Color::White);
	looseText.setFillColor(sf::Color::Red);

	CenterText(winText, SCREEN_HEIGHT / 2.0f);
	CenterText(looseText, SCREEN_HEIGHT / 2.0f);

	if (currentGameState == GameStateType::GST_Playing)
	{
		scoreText.setString("Score: " + std::to_string(score));
		scoreText.setPosition(SCREEN_WIDTH - 150.f, 5.f);
	}
	else if (currentGameState == GameStateType::GST_Won)
	{
		scoreText.setString("Score: " + std::to_string(getLastScore()));
	}
	else if (currentGameState == GameStateType::GST_Menu)
	{
		scoreText.setString("Last Score: " + std::to_string(lastScore) + "\nHigh Score: " + std::to_string(highScore));
		CenterText(scoreText, SCREEN_HEIGHT - 330.f);
	}
}

void Menu::ResetGame()
{
	if (score > highScore)
	{
		highScore = score;
		saveHighScore();
	}
	score = 0;
}

void Menu::setLastScore(int points)
{
	lastScore = points;
	if (lastScore > highScore)
	{
		highScore = lastScore;
		saveHighScore();
	}
}

void Menu::finalizeGame(bool isWin)
{
	if (isWin)
	{
		lastScore = score;
		if (score > highScore)
		{
			highScore = score;
			saveHighScore();
		}
	}
	else
	{
		lastScore = 0;
	}
	score = 0;
}

void Menu::HandleHover(const sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// New Game hover
	bool isStartHovered = startgameText.getGlobalBounds().contains(mousePos);
	if (isStartHovered != isStartGameHovered)
	{
		isStartGameHovered = isStartHovered;
		startgameText.setFillColor(isStartGameHovered ? sf::Color::Red : sf::Color::White);
	}

	// Settings hover
	bool currentSettingsHovered = settingsText.getGlobalBounds().contains(mousePos);
	if (currentSettingsHovered != isSettingsHovered)
	{
		isSettingsHovered = currentSettingsHovered;
		settingsText.setFillColor(isSettingsHovered ? sf::Color::Red : sf::Color::White);
	}

	// Exit hover
	bool currentExitHovered = exitText.getGlobalBounds().contains(mousePos);
	if (currentExitHovered != isExitHovered)
	{
		isExitHovered = currentExitHovered;
		exitText.setFillColor(isExitHovered ? sf::Color::Red : sf::Color::White);
	}
}

void Menu::Draw(sf::RenderWindow& window, GameStateType currentGameState)
{
	switch (currentGameState)
	{
		case GameStateType::GST_Menu:
			window.draw(MenuSprite);
			window.draw(titleText);
			window.draw(startgameText);
			window.draw(settingsText);
			window.draw(exitText);
			window.draw(scoreText);
			break;

		case GameStateType::GST_Playing:
			window.draw(timerText);
			window.draw(scoreText);
			break;

		case GameStateType::GST_Won:
			winText.setString("Congratulations! You Won!\nScore: " + std::to_string(lastScore));
			window.draw(winText);
			break;

		case GameStateType::GST_Gameover:
			looseText.setString("Time's up! Returning to menu\nScore: 0");
			window.draw(looseText);
			break;

		default:
			break;
	}
}


void Menu::saveHighScore() const
{
	std::ofstream file("highscore.dat", std::ios::binary);
	if (file)
	{
		file.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));
	}
}

void Menu::loadHighScore()
{
	std::ifstream file("highscore.dat", std::ios::binary);
	if (file)
	{
		file.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
	}
}