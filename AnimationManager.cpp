#include "AnimationManager.h"

AAnimationManager::AAnimationManager()
	: bIsAnimationFinished(false)
	, bStopAtLastFrame(false)
	, FrameSpeed(0.f)
	, CurrentFrameIndex(0)
{
}

AAnimationManager::~AAnimationManager()
{
	FrameRect.clear();
}

void AAnimationManager::AnimationUpdate(float DeltaTime)
{
	if (bIsAnimationFinished)
		return;

	if (bIsReverse)
	{
		CurrentFrameIndex -= FrameSpeed * DeltaTime;
		if (CurrentFrameIndex <= 0)
		{
			CurrentFrameIndex = 0;
			bIsAnimationFinished = true;
		}
	}
	else
	{
		CurrentFrameIndex += FrameSpeed * DeltaTime;
		if (CurrentFrameIndex >= FrameRect.size() - 1)	// Изменили условие
		{
			if (bStopAtLastFrame)
			{
				CurrentFrameIndex = static_cast<float>(FrameRect.size() - 1);
				bIsAnimationFinished = true;
			}
			else
			{
				CurrentFrameIndex = 0;
			}
		}
	}
}

sf::IntRect AAnimationManager::GetCurrentFrame() const
{
	if (FrameRect.empty())
		return sf::IntRect();

	// Берем ближайший целый кадр
	size_t frame = static_cast<size_t>(CurrentFrameIndex + 0.5f);  // Округляем
	if (frame >= FrameRect.size())
		frame = FrameRect.size() - 1;

	return FrameRect[frame];
}

void AAnimationManager::SetCurrentFrame(const int Frame)
{
	CurrentFrameIndex = static_cast<float>(Frame);
	bIsAnimationFinished = false;
}

void AAnimationManager::SetStopAtLastFrame(bool bStop)
{
	bStopAtLastFrame = bStop;
}

void AAnimationManager::ResetAnimation()
{
	bIsAnimationFinished = false;
	bStopAtLastFrame = false;
	FrameSpeed = 0.f;
	CurrentFrameIndex = 0;
	FrameRect.clear();
}

bool AAnimationManager::IsAnimationFinished() const
{
	return bIsAnimationFinished;
}

int AAnimationManager::GetCurrentFrameIndex() const
{
	return static_cast<int>(CurrentFrameIndex);
}

//sf::IntRect AAnimationManager::GetCurrentFrame() const
//{
//	if (FrameRect.empty() || static_cast<size_t>(CurrentFrameIndex) >= FrameRect.size())
//	{
//		return sf::IntRect();
//	}
//	return FrameRect[static_cast<size_t>(CurrentFrameIndex)];
//}

void AAnimationManager::SetReverse(bool reverse)
{
	bIsReverse = reverse;
}

void AAnimationManager::PlayForward()
{
	bIsReverse = false;
	bIsAnimationFinished = false;
	CurrentFrameIndex = 0;
}

void AAnimationManager::PlayReverse()
{
	bIsReverse = true;
	bIsAnimationFinished = false;
	CurrentFrameIndex = static_cast<float>(FrameRect.size() - 1);
}
