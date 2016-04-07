#include "stdafx.h"
#include "Player.h"
#include "Scene.h"
#include "Bullet.h"
#include "Monitor.h"
#include "Obstacle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Monitor* monitor = new Monitor();
sf::Text displayText;

void MonitorFetch()
{
	std::string text;
	monitor->Fetch(text);
	std::cout << text << std::endl;
	displayText.setString(text);
}

vector<Obstacle*> SpawnNewObstacle(int width, int height, Vector2f playerPos, vector<Obstacle*> obstacles);

int main()
{
	// initialize random seed: 
	srand(time(NULL));

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.TTF");

	displayText.setFont(font);
	displayText.setPosition(20, 20);
	displayText.setString("");
	displayText.setColor(sf::Color::Green);

	// Render window width and height
	int windowWidth = 800;
	int windowHeight = 600;

	// Full screen width and height
	int fullWidth = windowWidth;
	int fullHeight = windowHeight;

	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Asteroid Wars!!!");

	// Create a view
	View view;
	view.reset(FloatRect(0, 0, windowWidth, windowHeight));
	view.setViewport(FloatRect(0, 0, 1.0f, 1.0f));
	Vector2f camPosition(windowWidth / 2, windowHeight / 2);

	// Create radar view
	View radarView;
	radarView.setViewport(sf::FloatRect(0.755f, 0, 0.245f, 0.245f));

	// Create radar outline view
	View radarOutlineView;
	radarOutlineView.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));

	// Thread
	sf::Thread thread(&MonitorFetch);
	sf::Clock messageClock;
	sf::Clock asteroidClock;
	sf::Clock spawnClock;
	messageClock.restart();
	asteroidClock.restart();
	spawnClock.restart();

	// Window frame rate
	window.setFramerateLimit(60);

	// Class instances
	Player player(windowWidth, windowHeight, fullWidth, fullHeight);
	Scene scene(windowWidth, windowHeight, fullWidth, fullHeight);
	vector<Obstacle*>::iterator m_obstacleIterator;

	int score = 0;
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setPosition(600, 20);
	scoreText.setString("Score: " + std::to_string(score));
	scoreText.setColor(sf::Color::Yellow);

	// Create obstalces
	vector<Obstacle*> obstacles;
	Obstacle obstacleInstance;

	int noOfObstacles = 1;

	for (int i = 0; i < noOfObstacles; i++)
	{
		obstacles = obstacleInstance.CreateObstacle(fullWidth, fullHeight, player.GetPosition(), obstacles);
	}
	// Start game loop 
	while (window.isOpen())
	{
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		//prepare frame
		window.clear();

		radarView.reset(FloatRect(0, 0, fullWidth, fullHeight));

		// Update & Draw
		player.Update();

		// Check for new message
		if (messageClock.getElapsedTime().asSeconds() > 0.5)
		{
			if (monitor->NewMessage() == true)
			{
				asteroidClock.restart();
				thread.launch();
			}
			messageClock.restart();
		}

		// Reset text to blank
		if (asteroidClock.getElapsedTime().asSeconds() > 2.0f)
		{
			displayText.setString("");
		}

		// Spawn Asteroid/Obstacle
		if (spawnClock.getElapsedTime().asSeconds() > 2)
		{
			if (obstacles.size() < noOfObstacles)
			{
				obstacles = SpawnNewObstacle(fullWidth, fullHeight, player.GetPosition(), obstacles);
				spawnClock.restart();
				monitor->Deposit("Asteroid Spawned");
			}
		}

		scene.Draw(window);
		player.Draw(window);

		window.draw(displayText);
		window.draw(scoreText);

		#pragma region Obstacles
		// Update/Draw obstacles
		for (m_obstacleIterator = obstacles.begin(); m_obstacleIterator != obstacles.end(); ++m_obstacleIterator)
		{
			(*m_obstacleIterator)->Draw(window);
			(*m_obstacleIterator)->Update();

			// Bullet/Obstacle collision
			if (player.CheckBulletObstacleCollision((*m_obstacleIterator)))
			{
				// Remove boid
				obstacles.erase(m_obstacleIterator);
				monitor->Deposit("Asteroid Destroyed");
				score += 10;
				scoreText.setString("Score: " + std::to_string(score));
				spawnClock.restart();
				break;
			}

			// Player/Obstacle collision
			if (player.CheckObstacleCollision((*m_obstacleIterator)))
			{
				monitor->Deposit("Asteroid Destroyed");
				obstacles.erase(m_obstacleIterator);
				score += 10;
				scoreText.setString("Score: " + std::to_string(score));
				spawnClock.restart();
				break;
			}
		}// End iterator for obstacles 
		#pragma endregion

		#pragma region RadarView

		//window.setView(radarOutlineView);

		//scene.DrawRadarOutline(window);

		//window.setView(radarView);

		//scene.DrawOnRadar(window);
		//player.DrawOnRadar(window);

		//// Update/Draw obstacles
		//for (m_obstacleIterator = obstacles.begin(); m_obstacleIterator != obstacles.end(); ++m_obstacleIterator)
		//{
		//	(*m_obstacleIterator)->DrawOnRadar(window);
		//}
		#pragma endregion

		window.setView(window.getDefaultView());

		// Finally, display rendered frame on screen 
		window.display();
	} //loop back for next frame

	return EXIT_SUCCESS;
}

vector<Obstacle*> SpawnNewObstacle(int width, int height, Vector2f playerPos, vector<Obstacle*> obstacles)
{
	// Generate random number
	Vector2f obstaclePosition;
	obstaclePosition.x = rand() % width + 1;
	obstaclePosition.y = rand() % height + 1;

	float distance = 0;

	distance = sqrt(((obstaclePosition.x - playerPos.x)*(obstaclePosition.x - playerPos.x)) +
		((obstaclePosition.y - playerPos.y)*(obstaclePosition.y - playerPos.y)));

	while (distance <= 200)
	{
		obstaclePosition.x = rand() % width + 1;
		obstaclePosition.y = rand() % height + 1;

		distance = sqrt(((obstaclePosition.x - playerPos.x)*(obstaclePosition.x - playerPos.x)) +
			((obstaclePosition.y - playerPos.y)*(obstaclePosition.y - playerPos.y)));
	}

	// Generate random angle
	float angle = ((rand() % 360) / 100.0f) * 3.1415f;

	Obstacle *obstacle = new Obstacle(obstaclePosition.x, obstaclePosition.y, angle, width, height);

	obstacles.push_back(obstacle);

	return obstacles;
}












