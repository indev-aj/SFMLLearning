#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <SFML/Graphics.hpp>

/// <summary>
/// Class that acts as the game engine
/// </summary>

class Game
{
private:
	// Variables
	// Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	// Mouse Positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Game Objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	// Game Logic
	unsigned int points;
	int health;
	float enemySpawnTimer;
	float MAX_ENEMY_SPAWN_TIMER;
	int MAX_ENEMIES;
	bool mouseHeld;
	bool endGame;

	// Private Functions
	void initVariables();
	void initWindow();
	void initEnemies();

public:
	// Constructor & Destructor
	Game();
	virtual ~Game();

	// Accessors
	const bool running() const;
	const bool getEndGame() const;

	// Functions
	void spawnEnemy();
	void pollEvents();
	void updateMousePosition();

	void updateEnemies();
	void update();

	void renderEnemies();
	void render();

};

