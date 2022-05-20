#include "Game.h"

// Constructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initFonts();
	this->initTexts();
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
	this->health = 20;
	this->MAX_ENEMY_SPAWN_TIMER = 20.f;
	this->enemySpawnTimer = MAX_ENEMY_SPAWN_TIMER;
	this->MAX_ENEMIES = 5;
	this->mouseHeld = false;
	this->endGame = false;
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
	this->enemy.setFillColor(sf::Color::Cyan);
}

void Game::initFonts()
{
	if (!this->font.loadFromFile("Fonts/Eatday.ttf")) {
		std::cout << "ERROR::GAME::INITFONTS::Failed to load fonts!" << std::endl;
	}
}

void Game::initTexts()
{
	this->uiText.setFont(this->font);
	this->uiText.setCharacterSize(12);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("UI Text");
}

// Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::getEndGame() const
{
	return this->endGame;
}

// Functions

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

void Game::updateText()
{
	std::stringstream ss;
	ss << "Points: " << this->points << "\n"
		<< "Health: " << this->health << "\n";

	this->uiText.setString(ss.str());
}

void Game::renderText(sf::RenderTarget& target)
{
	target.draw(this->uiText);
}

void Game::spawnEnemy()
{
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int> (this->window->getSize().x - this->enemy.getSize().x)),
		0.f
	);

	// Randomize enemy type
	int type = rand() % 5;

	switch (type)
	{
	case 0:
		this->enemy.setSize(sf::Vector2f(29.f, 20.f));
		this->enemy.setFillColor(sf::Color::Magenta);
		break;
	case 1:
		this->enemy.setSize(sf::Vector2f(30.f, 30.f));
		this->enemy.setFillColor(sf::Color::Blue);
		break;
	case 2:
		this->enemy.setSize(sf::Vector2f(50.f, 50.f));
		this->enemy.setFillColor(sf::Color::Red);
		break;
	case 3:
		this->enemy.setSize(sf::Vector2f(70.f, 70.f));
		this->enemy.setFillColor(sf::Color::Cyan);
		break;
	case 4:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Green);
		break;
	default:
		this->enemy.setSize(sf::Vector2f(100.f, 100.f));
		this->enemy.setFillColor(sf::Color::Yellow);
		break;
	}
		
	this->enemies.push_back(this->enemy);
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

	// Moves and check if enemy is out of window
	for (int i = 0; i < this->enemies.size(); i++) {
		bool deleted = false;
		this->enemies[i].move(0.f, 5.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y) {
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;

			std::cout << "Health: " << this->health << std::endl;
		}
	}

	// Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (this->mouseHeld == false) {
			bool deleted = false;

			for (size_t i = 0; i < enemies.size() && !deleted; i++) {
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
					// Gain points
					if (this->enemies[i].getFillColor() == sf::Color::Magenta)
						this->points += 10;
					else if (this->enemies[i].getFillColor() == sf::Color::Blue)
						this->points += 8;
					else if (this->enemies[i].getFillColor() == sf::Color::Red)
						this->points += 6;
					else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
						this->points += 5;
					else if (this->enemies[i].getFillColor() == sf::Color::Green)
						this->points += 4;
					else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
						this->points += 2;

					std::cout << "Points: " << this->points << std::endl;
					
					// Delete enemy
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);
				}
			}
			this->mouseHeld = true;
		}
	}
	else {
		this->mouseHeld = false;
	}
}

void Game::renderEnemies(sf::RenderTarget& target)
{
	for (auto& e : this->enemies) {
		target.draw(e);
	}
}

void Game::update()
{
	this->pollEvents();

	if (!this->endGame) {
		this->updateMousePosition();
		this->updateText();
		this->updateEnemies();
	}

	// End game condition
	if (this->health <= 0)
		this->endGame = true;
}

void Game::render()
{
	this->window->clear();

	// Draw game objects
	this->renderEnemies(*this->window);
	this->renderText(*this->window);
	this->window->display();
}

