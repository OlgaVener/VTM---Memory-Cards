#pragma once
#include <vector>
#include "Ui.h"
#include "AnimationManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//??????? ???????
enum class Clans
{
	BRUJAH,
	LASOMBRA,
	GANGREL,
	MALKAVIAN,
	NOSFERATU,
	TOREADOR,
	TREMERE,
	VENTRUE,
};
// std::vector<Clans> clans =
//{
//     Clans::BANU_HAQIM, Clans::BRUJAH, Clans::LASOMBRA, Clans::CAITIFF, Clans::GANGREL,
//     Clans::HECATA, Clans::MALKAVIAN, Clans::NOSFERATU, Clans::RAVNOS, Clans::SALUBRI,
//     Clans::THE_MINISTRY, Clans::THIN_BLOOD, Clans::TOREADOR, Clans::TREMERE,
//     Clans::TZIMISCE, Clans::VENTRUE
// };

enum class CardState
{
	BackSide,	  // Показывает обратную сторону
	FlippingUp,	  // Анимация переворота вверх
	FrontSide,	  // Показывает лицевую сторону
	FlippingDown  // Анимация переворота вниз
};

//????????? ?????
struct CardType
{
	// std::string clanname;
	Clans clansType;
	bool isMatched = false;
	sf::Sprite frontSprite;
	sf::Sprite backSprite;
	//sf::Texture frontTexture;
	//sf::Texture backTexture; 
	sf::Sprite FlipSprite;
	CardState currentState = CardState::BackSide;
	AAnimationManager FlipAnimation;
};

class Card
{
public:
	Card() = default;
	~Card() = default;

	// Разрешаем перемещение
	Card(Card&&) = default;
	Card& operator=(Card&&) = default;

	void InitCard(const sf::Texture& CardAnimTexture, const sf::Vector2f& cardPosition, const sf::Texture& FrontCardTexture, const sf::Texture& BackCardTexture);
	void UpdateCard(float DeltaTime);
	void SetFlipSound(const sf::SoundBuffer& buffer);
	void StartFlip(bool toFront);
	void MarkAsMatched();
	void FlipToFront();
	void FlipToBack();
	void ToggleFlip();
	void Draw(sf::RenderWindow& window);
	//void drawHighlight(sf::RenderWindow& window);
	void ResetColor()
	{
		card.frontSprite.setColor(sf::Color::White);
	}

		void ResetCard()
	{
		cardType.currentState = CardState::BackSide;  // Используем cardType
		cardType.isMatched = false;
	}

	bool isMatched() const { return card.isMatched; }
	bool operator==(const Card& other) const
	{
		return &card == &other.card;					   // Сравниваем по адресу внутренней структуры
	}
	static void LoadSounds()
	{
		if (!flipSoundBuffer.loadFromFile("Resources/Sounds/flip.wav"))
		{
			// обработка ошибки загрузки
		}
	}

	void PlayFlipSound()
	{
		flipSound.setBuffer(flipSoundBuffer);
		flipSound.play();
	}

	CardType& getCardType() { return card; }
	const CardType& getCardType() const { return card; }

private:
	CardType card;
	CardType cardType;
	const float FLIPPING_TIME_PAUSE = 0.5f;
	sf::Clock FlippingPause;
	sf::Sound flipSound;
	static sf::SoundBuffer flipSoundBuffer;	 // объявление статической переменной
};
