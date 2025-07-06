#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class GameSettings
{
public:
	struct Resolution
	{
		unsigned int width;
		unsigned int height;
		std::string toString() const;
	};

	GameSettings();
	void loadSettings();
	void saveSettings() const;
	void applySettings(sf::RenderWindow& window);
	void handleEvent(const sf::Event& event, const sf::Vector2f& mousePos);
	void draw(sf::RenderWindow& window) const;
	void updateUI();

	float getVolume() const { return m_volume; }
	Resolution getCurrentResolution() const;
	bool isFullscreen() const { return m_fullscreen; }
	bool shouldReturnToMenu() const { return m_returnToMenu; }
	bool needsApply() const { return m_settingsChanged; }

	void setVolume(float vol);
	void nextResolution();
	void toggleFullscreen();

private:
	void handleHover(const sf::Vector2f& mousePos);
	void initUIElements();

	std::vector<Resolution> m_availableResolutions;
	size_t m_currentResolutionIndex = 0;
	float m_volume = 100.f;
	bool m_fullscreen = false;
	bool m_settingsChanged = false;
	bool m_returnToMenu = false;
	bool m_isApplyHovered = false;
	bool m_isBackHovered = false;
	bool isExitHovered = false;

	sf::Font m_font;
	sf::Text m_titleText;
	sf::Text m_resolutionText;
	sf::Text m_volumeText;
	sf::Text m_applyButton;
	sf::Text m_backButton;
	sf::RectangleShape m_volumeBar;
	sf::RectangleShape m_volumeSlider;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	const std::string m_fontPath = "Memory Cards/Resources/Fonts/arialmt.ttf";
	const int m_screenWidth = 900;
	const int m_screenHeight = 1200;
};