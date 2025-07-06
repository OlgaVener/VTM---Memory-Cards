#pragma once
#include "SFML/Audio.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <assert.h>

// Размер экрана
const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 1200;


const int NUM_POINTS = 4;

const float cardWidth = 180.f;
const float cardHeight = 300.f;
const float spacingX = 20.f;  // расстояние между карточками по x
const float spacingY = 20.f;  // расстояние между карточками по y
const float startX = 75.f;	  // расстояние от края окна по x
const float startY = 70.f;	  // расстояние от края окна по y

// ���� � ��������
const std::string RESOURCES_PATH = "Memory Cards/Resources/";
const std::string FONT_PATH = "Memory Cards/Resources/Fonts/arialmt.ttf";
const std::string TITLEFONT_PATH = "Memory Cards/Resources/Fonts/17383.otf";
const std::string SOUNDS_PATH = "Memory Cards/Resources/Sounds/";

// ��������
const float FLIP_ANIMATION_TIME = 0.5f;
const float GAME_TIME = 60.0f;
const float FLIPPING_TIME_PAUSE = 0.5f;
