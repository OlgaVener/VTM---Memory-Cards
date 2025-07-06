#include "GameSettings.h"
#include <fstream>
#include <iostream>

std::string GameSettings::Resolution::toString() const
{
	return std::to_string(width) + "x" + std::to_string(height);
}

GameSettings::GameSettings()
{
	m_availableResolutions = {
		{ 600, 800 },
		{ 900, 1200 },
		{ 1200, 1600 }
	};

	if (!m_font.loadFromFile(m_fontPath))
	{
		std::cerr << "Error loading font for settings menu!" << std::endl;
	}

	if (!m_backgroundTexture.loadFromFile("Memory Cards/Resources/menubg.png"))
	{
		std::cerr << "Failed to load background texture!" << std::endl;
	}
	m_backgroundSprite.setTexture(m_backgroundTexture);
	m_backgroundSprite.setScale(1.22f, 1.f);

	initUIElements();
	loadSettings();
}

void GameSettings::initUIElements()
{
	m_titleText.setFont(m_font);
	m_titleText.setString("SETTINGS");
	m_titleText.setCharacterSize(50);
	m_titleText.setFillColor(sf::Color::White);

	m_resolutionText.setFont(m_font);
	m_resolutionText.setCharacterSize(30);
	m_resolutionText.setFillColor(sf::Color::White);

	m_volumeText.setFont(m_font);
	m_volumeText.setCharacterSize(30);
	m_volumeText.setFillColor(sf::Color::White);

	m_applyButton.setFont(m_font);
	m_applyButton.setString("APPLY");
	m_applyButton.setCharacterSize(30);
	m_applyButton.setFillColor(sf::Color::White);

	m_backButton.setFont(m_font);
	m_backButton.setString("BACK");
	m_backButton.setCharacterSize(30);
	m_backButton.setFillColor(sf::Color::White);

	m_volumeBar.setSize(sf::Vector2f(200, 10));
	m_volumeBar.setFillColor(sf::Color(100, 100, 100));

	m_volumeSlider.setSize(sf::Vector2f(10, 20));
	m_volumeSlider.setFillColor(sf::Color::White);

	updateUI();
}

void GameSettings::handleHover(const sf::Vector2f& mousePos)
{
	bool isApplyHovered = m_applyButton.getGlobalBounds().contains(mousePos);
	if (isApplyHovered != m_isApplyHovered)
	{
		m_isApplyHovered = isApplyHovered;
		m_applyButton.setFillColor(m_isApplyHovered ? sf::Color::Red : sf::Color::White);
	}

	bool isBackHovered = m_backButton.getGlobalBounds().contains(mousePos);
	if (isBackHovered != m_isBackHovered)
	{
		m_isBackHovered = isBackHovered;
		m_backButton.setFillColor(m_isBackHovered ? sf::Color::Red : sf::Color::White);
	}
}


void GameSettings::updateUI()
{
	// Центрирование заголовка
	sf::FloatRect titleBounds = m_titleText.getLocalBounds();
	m_titleText.setOrigin(titleBounds.width / 2.0f, titleBounds.height / 2.0f);
	m_titleText.setPosition(m_screenWidth / 2.0f, 450.0f);

	// Центрирование текста разрешения
	m_resolutionText.setString("Resolution: " + getCurrentResolution().toString());
	sf::FloatRect resBounds = m_resolutionText.getLocalBounds();
	m_resolutionText.setOrigin(resBounds.width / 2.0f, resBounds.height / 2.0f);
	m_resolutionText.setPosition(m_screenWidth / 2.0f, 550.0f);

	// Центрирование текста громкости
	m_volumeText.setString("Volume: " + std::to_string(static_cast<int>(m_volume)) + "%");
	sf::FloatRect volBounds = m_volumeText.getLocalBounds();
	m_volumeText.setOrigin(volBounds.width / 2.0f, volBounds.height / 2.0f);
	m_volumeText.setPosition(m_screenWidth / 2.0f, 600.0f);

	// Позиционирование ползунка громкости (центрируем относительно текста)
	m_volumeBar.setSize(sf::Vector2f(300.0f, 10.0f));  // Увеличили длину
	m_volumeBar.setPosition(m_screenWidth / 2.0f - m_volumeBar.getSize().x / 2.0f, 630.0f);
	m_volumeSlider.setPosition(
		m_volumeBar.getPosition().x + (m_volume / 100.0f) * m_volumeBar.getSize().x - 5.0f,
		625.0f);

	// Центрирование кнопок
	m_applyButton.setString("APPLY");
	sf::FloatRect applyBounds = m_applyButton.getLocalBounds();
	m_applyButton.setOrigin(applyBounds.width / 2.0f, applyBounds.height / 2.0f);
	m_applyButton.setPosition(m_screenWidth / 2.0f - 100.0f, 700.0f);

	m_backButton.setString("BACK");
	sf::FloatRect backBounds = m_backButton.getLocalBounds();
	m_backButton.setOrigin(backBounds.width / 2.0f, backBounds.height / 2.0f);
	m_backButton.setPosition(m_screenWidth / 2.0f + 100.0f, 700.0f);
}

void GameSettings::loadSettings()
{
	std::ifstream file("settings.cfg");
	if (file)
	{
		file >> m_currentResolutionIndex;
		file >> m_fullscreen;
		file >> m_volume;

		// Валидация данных
		if (m_currentResolutionIndex >= m_availableResolutions.size())
		{
			m_currentResolutionIndex = 0;
		}
		m_volume = (m_volume < 0.f) ? 0.f : (m_volume > 100.f) ? 100.f
															   : m_volume;
	}
	updateUI();
}

void GameSettings::setVolume(float vol)
{
	m_volume = (vol < 0.f) ? 0.f : (vol > 100.f) ? 100.f
												 : vol;
	updateUI();
	m_settingsChanged = true;
}

void GameSettings::saveSettings() const
{
	std::ofstream file("settings.cfg");
	if (file)
	{
		file << m_currentResolutionIndex << "\n"
			 << m_fullscreen << "\n"
			 << m_volume;
	}
}

void GameSettings::applySettings(sf::RenderWindow& window)
{
	// Применяем настройки только если они изменились
	static Resolution lastResolution = getCurrentResolution();
	static bool lastFullscreen = m_fullscreen;

	if (getCurrentResolution().width != lastResolution.width || getCurrentResolution().height != lastResolution.height || m_fullscreen != lastFullscreen)
	{
		auto res = getCurrentResolution();
		window.create(
			sf::VideoMode(res.width, res.height),
			"Memory Cards",
			m_fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
		window.setVerticalSyncEnabled(true);

		lastResolution = res;
		lastFullscreen = m_fullscreen;
	}
}

void GameSettings::handleEvent(const sf::Event& event, const sf::Vector2f& mousePos)
{
	handleHover(mousePos);

	m_returnToMenu = false;
	m_settingsChanged = false;

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		if (m_resolutionText.getGlobalBounds().contains(mousePos))
		{
			nextResolution();
		}
		else if (m_volumeBar.getGlobalBounds().contains(mousePos))
		{
			float relativePos = (mousePos.x - m_volumeBar.getPosition().x) / m_volumeBar.getSize().x;
			setVolume(relativePos * 100.f);
		}
		else if (m_applyButton.getGlobalBounds().contains(mousePos))
		{
			m_settingsChanged = true;
		}
		else if (m_backButton.getGlobalBounds().contains(mousePos))
		{
			m_returnToMenu = true;
		}
	}
	else if (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		// Обработка перетаскивания ползунка без применения настроек
		if (m_volumeSlider.getGlobalBounds().contains(mousePos) || m_volumeBar.getGlobalBounds().contains(mousePos))
		{
			float relativePos = (mousePos.x - m_volumeBar.getPosition().x) / m_volumeBar.getSize().x;
			float newVolume = std::max(0.f, std::min(100.f, relativePos * 100.f));
			setVolume(newVolume);
		}
	}
}

void GameSettings::draw(sf::RenderWindow& window) const
{
	window.draw(m_backgroundSprite);
	window.draw(m_titleText);
	window.draw(m_resolutionText);
	window.draw(m_volumeText);
	window.draw(m_volumeBar);
	window.draw(m_volumeSlider);
	window.draw(m_applyButton);
	window.draw(m_backButton);
}

GameSettings::Resolution GameSettings::getCurrentResolution() const
{
	return m_availableResolutions[m_currentResolutionIndex];
}


void GameSettings::nextResolution()
{
	m_currentResolutionIndex = (m_currentResolutionIndex + 1) % m_availableResolutions.size();
	updateUI();
}

void GameSettings::toggleFullscreen()
{
	m_fullscreen = !m_fullscreen;
	updateUI();
}