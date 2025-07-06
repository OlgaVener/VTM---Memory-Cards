#include "Game.h"
#include "GameSettings.h"

int main()
{
	// Инициализация окна
	sf::VideoMode windowMode(SCREEN_WIDTH, SCREEN_HEIGHT);
	sf::RenderWindow window(windowMode, "Memory Cards");

	// Настройка view
	sf::View gameView;
	gameView.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	gameView.setCenter(gameView.getSize().x / 2, gameView.getSize().y / 2);
	window.setView(gameView);

	// Ограничение FPS
	window.setFramerateLimit(120);

	// Инициализация игровых объектов
	Game game;
	game.InitGame();
	Card::LoadSounds();

	// Переменные состояния
	bool showSettings = false;
	sf::Clock clock;

	// Главный игровой цикл
	while (window.isOpen())
	{
		// Обработка событий
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

			if (showSettings)
			{
				// Обработка событий в меню настроек
				game.GetSettings().handleEvent(event, mousePos);

				if (game.GetSettings().shouldReturnToMenu())
				{
					showSettings = false;
				}

				if (game.GetSettings().needsApply())
				{
					game.GetSettings().applySettings(window);
					window.setView(gameView);
					game.ApplySettings();
				}

			}
			else
			{
				// Обработка событий в основном меню/игре
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					// Проверка клика по кнопке настроек
					if (game.getState() == GameStateType::GST_Menu && game.GetMenu().GetSettingsText().getGlobalBounds().contains(mousePos))
					{
						showSettings = true;
					}
					else
					{
						game.HandleClick(window);
					}
				}
			}
		}

		// Обновление игры
		float deltaTime = clock.restart().asSeconds();

		if (!showSettings)
		{
			game.Update(deltaTime, window);
		}

		// Отрисовка
		window.clear();

		if (showSettings)
		{
			game.GetSettings().draw(window);
		}
		else
		{
			game.Draw(window);
		}

		window.display();
	}

	// Сохранение данных при выходе
	game.GetMenu().saveHighScore();
	game.GetSettings().saveSettings();


	return 0;
}