#include "Card.h"
#include <iostream>

sf::SoundBuffer Card::flipSoundBuffer;

void Card::InitCard(const sf::Texture& CardAnimTexture,
    const sf::Vector2f& cardPosition,
    const sf::Texture& FrontCardTexture,
    const sf::Texture& BackCardTexture)
{
	// Инициализация спрайтов
	card.frontSprite.setTexture(FrontCardTexture);
	card.backSprite.setTexture(BackCardTexture);
	card.FlipSprite.setTexture(CardAnimTexture);

	card.frontSprite.setScale(0.8, 0.8);
	card.backSprite.setScale(0.8, 0.8);
	card.FlipSprite.setScale(0.8, 0.8);

	card.frontSprite.setOrigin(0.5f, 0.5f);
	card.backSprite.setOrigin(0.5f, 0.5f);
	card.FlipSprite.setOrigin(0.5f, 0.5f);

	card.frontSprite.setPosition(cardPosition);
	card.backSprite.setPosition(cardPosition);
	card.FlipSprite.setPosition(cardPosition);

    // Настройка анимации
    card.FlipAnimation.FrameSpeed = 10.f;
    card.FlipAnimation.SetStopAtLastFrame(true);

    // Кадры анимации - убедитесь, что координаты соответствуют вашему спрайт-листу
    card.FlipAnimation.FrameRect = {
        sf::IntRect(0, 0, 136, 300),    // Первый кадр (рубашка)
        sf::IntRect(283, 0, 136, 300),  // Второй кадр
        sf::IntRect(502, 0, 62, 300),   // Третий кадр
        sf::IntRect(644, 0, 24, 300),   // Четвёртый кадр
        sf::IntRect(753, 0, 63, 300),   // Пятый кадр
        sf::IntRect(901, 0, 125, 300),  // Шестой кадр
        sf::IntRect(1110, 0, 204, 300), // Седьмой кадр
        sf::IntRect(0, 0, 136, 300)     // Последний кадр (лицевая сторона)
    };

    // Начальное состояние
    card.currentState = CardState::BackSide;
    card.isMatched = false;
}

void Card::UpdateCard(float deltaTime)
{
	if (card.currentState != CardState::FlippingUp && card.currentState != CardState::FlippingDown)
		return;

	if (!card.FlipAnimation.IsAnimationFinished())
	{
		card.FlipAnimation.AnimationUpdate(deltaTime);
		card.FlipSprite.setTextureRect(card.FlipAnimation.GetCurrentFrame());
	}
	else
	{
		if (card.currentState == CardState::FlippingUp)
		{
			card.currentState = CardState::FrontSide;
		}
		else if (card.currentState == CardState::FlippingDown)
		{
			card.currentState = CardState::BackSide;
		}
	}
}

void Card::SetFlipSound(const sf::SoundBuffer& buffer)
{
	flipSound.setBuffer(buffer);
	flipSound.setVolume(70.f);	// Можно настроить громкость (0-100)
}

void Card::StartFlip(bool toFront)
{
	// Если уже в процессе анимации - игнорируем
	if (card.currentState == CardState::FlippingUp || card.currentState == CardState::FlippingDown)
	{
		return;
	}

	if (toFront && card.currentState == CardState::BackSide)
	{
		card.currentState = CardState::FlippingUp;
		card.FlipAnimation.PlayForward();
		flipSound.play();
	}
	else if (!toFront && card.currentState == CardState::FrontSide)
	{
		card.currentState = CardState::FlippingDown;
		card.FlipAnimation.PlayReverse();
		flipSound.play();
	}
}

void Card::MarkAsMatched()
{
	card.isMatched = true;
}

void Card::FlipToFront()
{
	// Добавляем проверку паузы
	if (FlippingPause.getElapsedTime().asSeconds() < FLIPPING_TIME_PAUSE)
		return;

	if (card.currentState == CardState::BackSide)
	{
		card.currentState = CardState::FlippingUp;
		card.FlipAnimation.ResetAnimation();
		card.FlipAnimation.SetReverse(false);
		flipSound.play();
		FlippingPause.restart();
	}
}

void Card::FlipToBack()
{
	// Добавляем проверку паузы
	if (FlippingPause.getElapsedTime().asSeconds() < FLIPPING_TIME_PAUSE)
		return;

	if (card.currentState == CardState::FrontSide)
	{
		card.currentState = CardState::FlippingDown;
		card.FlipAnimation.ResetAnimation();
		card.FlipAnimation.SetReverse(true);
		flipSound.play();
		FlippingPause.restart();
	}
}

void Card::ToggleFlip()
{
	if (card.currentState == CardState::BackSide)
	{
		FlipToFront();
	}
	else if (card.currentState == CardState::FrontSide)
	{
		FlipToBack();
	}
}

void Card::Draw(sf::RenderWindow& window)
{
	switch (card.currentState)
	{
		case CardState::BackSide:
			window.draw(card.backSprite);
			break;
		case CardState::FrontSide:
			window.draw(card.frontSprite);
			break;
		case CardState::FlippingUp:
		case CardState::FlippingDown:
			window.draw(card.FlipSprite);
			break;
	}
}