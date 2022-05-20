#include "Game.h"

// Constructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initEnemies();
}

// Destructor
Game::~Game()
{
	delete this->window;
}

// Private Functions
void Game::initVariables()
{
	this->window = nullptr;

	// Game Logics
	this->points = 0;
	this->MAX_ENEMY_SPAWN_TIMER = 20.f;
	this->enemySpawnTimer = MAX_ENEMY_SPAWN_TIMER;
	this->MAX_ENEMIES = 5;
}

void Game::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new sf::RenderWindow(this->videoMode, "Moving Around");
	this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
	this->enemy.setPosition(10.f, 10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Cyan);
}

// Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

// Functions
void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int> (this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	this->enemy.setFillColor(sf::Color::Green);
	this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			break;
		default:
			break;
		}
	}
}

void Game::updateMousePosition()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
	// Update the timer for enemy spawning
	if (this->enemies.size() < this->MAX_ENEMIES) {
		if (this->enemySpawnTimer >= this->MAX_ENEMY_SPAWN_TIMER) {
			// Spawn enemy and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
			this->enemySpawnTimer += 1.f;
	}

	// Delete the enemies
	for (auto& e : this->enemies) {
		e.move(0.f, 5.f);
	}

	for (int i = 0; i < this->enemies.size(); i++) {
		bool deleted = false;
		this->enemies[i].move(0.f, 1.f);

		// Check if clicked upon
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
				deleted = true;

				// Gain points
				this->points += 5;
				std::cout << "Points: " << this->points << std::endl;
			}
		}

		// Check if enemy out of window
		if (this->enemies[i].getPosition().y > this->window->getSize().y)
			deleted = true;

		if (deleted)
			this->enemies.erase(this->enemies.begin() + i);
	}
}

void Game::update()
{
	this->pollEvents();
	this->updateMousePosition();
	this->updateEnemies();
}

void Game::renderEnemies()
{
	for (auto& e : this->enemies) {
		this->window->draw(e);
	}
}

void Game::render()
{
	this->window->clear();

	// Draw game objects
	this->renderEnemies();

	this->window->display();
}

