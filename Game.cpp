#include "Game.h"
#include "Ui.h"
#include <numeric>
#include <random>
#include <algorithm>
#include <cassert>
#include <iostream>

Game::~Game()
{
	CardsVector.clear();
}

std::vector<int> Game::GeneratePairs(int pairsCount)
{
	std::vector<int> clans;
	for (int i = 0; i < 8; ++i)
		clans.push_back(i);

	pairsCount = std::min(pairsCount, static_cast<int>(clans.size()));
	std::vector<int> pairs;

	for (int i = 0; i < pairsCount; i++)
	{
		pairs.push_back(clans[i]);
		pairs.push_back(clans[i]);
	}

	std::shuffle(pairs.begin(), pairs.end(), std::mt19937(std::random_device()()));
	return pairs;
}

void Game::InitGame()
{
	CardsVector.clear();
	currentGameState = GameStateType::GST_Menu;

	// Загрузка текстур
	if (!CardAnimTexture.loadFromFile(RESOURCES_PATH + "animation2.png") || !BackCardTexture.loadFromFile(RESOURCES_PATH + "cardback.png") || !TextureBackground.loadFromFile(RESOURCES_PATH + "background.png"))
	{
		std::cerr << "Failed to load textures!" << std::endl;
		return;
	}

	// Загрузка звуков
	if (!flipSoundBuffer.loadFromFile(SOUNDS_PATH + "flip.wav") || !matchSoundBuffer.loadFromFile(SOUNDS_PATH + "match.wav") || !nomatchSoundBuffer.loadFromFile(SOUNDS_PATH + "nomatch.wav"))
	{
		std::cerr << "Failed to load sound files!" << std::endl;
	}

	flipSound.setBuffer(flipSoundBuffer);
	matchSound.setBuffer(matchSoundBuffer);
	nomatchSound.setBuffer(nomatchSoundBuffer);

	// Фоновая музыка
	if (!backgroundMusic.openFromFile(SOUNDS_PATH + "background.ogg"))
	{
		std::cerr << "Failed to load background music!" << std::endl;
	}
	else
	{
		backgroundMusic.setLoop(true);
		backgroundMusic.setVolume(settings.getVolume());
		backgroundMusic.play();
	}

	// Загрузка лицевых текстур карт
	frontTextures.clear();
	for (int i = 1; i <= 8; ++i)
	{
		sf::Texture texture;
		if (texture.loadFromFile(RESOURCES_PATH + "card" + std::to_string(i) + ".png"))
		{
			frontTextures.push_back(std::move(texture));
		}
	}

	SpriteBackground.setTexture(TextureBackground);
	SpriteBackground.setScale(2.0f, 2.0f);

	// Генерация карт
	auto pairs = GeneratePairs(8);
	int count = 0;

	for (int y = 0; y < NUM_POINTS; ++y)
	{
		for (int x = 0; x < NUM_POINTS; ++x)
		{
			if (count >= 16)
				break;

			size_t index = count++;
			sf::Vector2f cardPos(
				startX + x * (cardWidth + spacingX),
				startY + y * (cardHeight - spacingY));

			auto card = std::make_unique<Card>();
			card->InitCard(
				CardAnimTexture,
				cardPos,
				frontTextures[pairs[index]],
				BackCardTexture);

			card->getCardType().clansType = static_cast<Clans>(pairs[index]);
			CardsVector.push_back(std::move(card));
		}
	}
	// Сброс состояния карт без нового метода
	for (auto& card : CardsVector)
	{
		card->getCardType().currentState = CardState::BackSide;
		card->getCardType().isMatched = false;
	}

	// Сброс игровых переменных
	openedCardsCount = 0;
	isWaiting = false;
	flipTimer = 0.f;
	menu.InitMenu();
	gameTimer = GAME_TIME;
	menu.MenuHUD();
}

void Game::updateVolume()
{
	float volume = settings.getVolume();

	backgroundMusic.setVolume(volume);
	flipSound.setVolume(volume);
	matchSound.setVolume(volume);
	nomatchSound.setVolume(volume);
	gameOverSound.setVolume(volume);

}

void Game::Update(float deltaTime, const sf::RenderWindow& window)
{
	if (currentGameState == GameStateType::GST_Menu)
	{
		menu.HandleHover(window);
	}

	if (currentGameState == GameStateType::GST_Playing)
	{
		gameTimer -= deltaTime;

		if (isWaiting)
		{
			flipTimer += deltaTime;
			if (flipTimer >= flipCooldown)
			{
				isWaiting = false;
				flipTimer = 0.f;
				CheckMatch();
			}
		}

		if (CardsVector.empty())
		{
			currentGameState = GameStateType::GST_Won;
			menu.addScore(static_cast<int>(gameTimer * 10));
			menu.finalizeGame(true);
		}
		else if (gameTimer <= 0)
		{
			currentGameState = GameStateType::GST_Gameover;
			menu.finalizeGame(false);
		}

		for (auto& card : CardsVector)
		{
			card->UpdateCard(deltaTime);
		}

		// Удаление совпавших карт
		auto it = CardsVector.begin();
		while (it != CardsVector.end())
		{
			if ((*it)->isMatched())
				it = CardsVector.erase(it);
			else
				++it;
		}
	}
	else if (currentGameState == GameStateType::GST_Won || currentGameState == GameStateType::GST_Gameover)
	{
		static sf::Clock winClock;
		if (winClock.getElapsedTime().asSeconds() >= 2.0f)
		{
			currentGameState = GameStateType::GST_Menu;
			InitGame();
			menu.ResetGame();
			winClock.restart();
		}
	}

	menu.UpdateHUD(gameTimer, currentGameState);
}

void Game::Draw(sf::RenderWindow& window)
{
	if (currentGameState == GameStateType::GST_Playing)
	{
		window.draw(SpriteBackground);
		for (const auto& card : CardsVector)
		{
			card->Draw(window);
		}
	}
	else if (currentGameState == GameStateType::GST_Settings)
	{
		settings.draw(window);
		return;
	}

	menu.Draw(window, currentGameState);
}

void Game::HandleClick(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	if (isWaiting || currentGameState == GameStateType::GST_Gameover)
		return;

	if (openedCardsCount >= 2) return;

	if (currentGameState == GameStateType::GST_Playing) 
	{
		for (auto& card : CardsVector)
		{
			if (!card->isMatched() && card->getCardType().backSprite.getGlobalBounds().contains(mousePos) && card->getCardType().currentState == CardState::BackSide)
			{
				card->StartFlip(true);
				openedCardsCount++;

				if (openedCardsCount == 2)
				{
					isWaiting = true;
					flipTimer = 0.f;
				}
				break;
			}
		}
	}
	else if (currentGameState == GameStateType::GST_Menu)
	{
		if (menu.GetStartGameText().getGlobalBounds().contains(mousePos))
		{
			currentGameState = GameStateType::GST_Playing;
			gameTimer = GAME_TIME;
		}
		else if (menu.GetSettingsText().getGlobalBounds().contains(mousePos))
		{
			currentGameState = GameStateType::GST_Settings;
		}
		else if (menu.GetExitText().getGlobalBounds().contains(mousePos))
		{
			window.close();
		}
	}
	else if (currentGameState == GameStateType::GST_Settings)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			settings.handleEvent(event, window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}

		if (settings.needsApply())
		{
			settings.saveSettings();
			settings.applySettings(window);
			updateVolume();
		}

		if (settings.shouldReturnToMenu())
		{
			currentGameState = GameStateType::GST_Menu;
		}
	}
}


void Game::CheckMatch()
{
	std::vector<Card*> flippedCards;
	for (auto& card : CardsVector)
	{
		if (card->getCardType().currentState == CardState::FrontSide && !card->isMatched())
		{
			flippedCards.push_back(card.get());
		}
	}

	if (flippedCards.size() == 2)
	{
		if (flippedCards[0]->getCardType().clansType == flippedCards[1]->getCardType().clansType)
		{
			flippedCards[0]->MarkAsMatched();
			flippedCards[1]->MarkAsMatched();
			menu.addScore(100);
			matchSound.play();
		}
		else
		{
			flippedCards[0]->StartFlip(false);
			sf::Clock delayClock;
			while (delayClock.getElapsedTime().asSeconds() < 0.3f)
			{
			}
			flippedCards[1]->StartFlip(false);
			nomatchSound.play();
		}
		openedCardsCount = 0;
	}
}