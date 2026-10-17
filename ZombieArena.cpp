#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include "Player.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include <iostream>

int main() {
	enum class State {PAUSED, LEVELING_UP, GAME_OVER, PLAYING};
	State state = State::GAME_OVER;

	Vector2f resolution;
	resolution.x = static_cast<float>(VideoMode::getDesktopMode().size.x);
	resolution.y = static_cast<float>(VideoMode::getDesktopMode().size.y);

	RenderWindow window(VideoMode(VideoMode::getDesktopMode().size), "Zombie Arena", Style::Default);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect({ 0,0 }, { resolution.x, resolution.y }));

	// Here is our clock for timing everything
	Clock clock;
	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class
	Player player;

	// The boundaries of the arena
	IntRect arena;

	// Create the background
	VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background_sheet.png");

	// Prepare for a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;

	// The main game loop


	while (window.isOpen()) {

		while (const std::optional event = window.pollEvent()) 
		{

			if (event->is<Event::Closed>()) {
				window.close();
			}

			else if (const auto* keyPressed = event->getIf<Event::KeyPressed>()) 
			{
				std::cout << "Caught keyPressed event." << "\n";
				std::cout << "State: " << static_cast<int>(state) << "\n";
				// Pause/Unpause/Restart using Return key
				if (keyPressed->code == sf::Keyboard::Key::Enter)
				{
					if (state == State::PLAYING)
					{
						state = State::PAUSED;
					}
					else if (state == State::PAUSED)
					{
						state = State::PLAYING;
						// Reset the clock so there isn't a frame jump
						clock.restart();
					}
					else if (state == State::GAME_OVER)
					{
						state = State::LEVELING_UP;
					}
				}

				// Handle the levelling up state choices (Num1 to Num6)
				if (state == State::LEVELING_UP)
				{
					if (keyPressed->code >= sf::Keyboard::Key::Num1 && keyPressed->code <= sf::Keyboard::Key::Num6)
					{
						state = State::PLAYING;
					}

					if (state == State::PLAYING)
					{
						// Prepare the level
						// We will modify the next two lines later
						arena.size.x = 500;
						arena.size.y = 500;
						arena.position.x = 0;
						arena.position.y = 0;

						// Pass the vertex array by reference 
						// to the createBackground function
						int tileSize = createBackground(background, arena);

						// Spawn the player in the middle of the arena
						player.spawn(arena, resolution, tileSize);

						// Create a horde of zombies
						numZombies = 10;

						// Delete the previously allocated memory (if it exists)
						delete[] zombies;
						zombies = createHorde(numZombies, arena);
						numZombiesAlive = numZombies;


						// Reset the clock so there isn't a frame jump
						clock.restart();
					}
				}
			}
			// Handle the player quitting via Escape
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}

			// Handle controls while playing
			if (state == State::PLAYING)
			{
				// Handle the pressing and releasing of the WASD keys
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				{
					player.moveUp();
				}
				else
				{
					player.stopUp();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					player.moveDown();
				}
				else
				{
					player.stopDown();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				{
					player.moveLeft();
				}
				else
				{
					player.stopLeft();
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					player.moveRight();
				}
				else
				{
					player.stopRight();
				}

			}// End WASD while playing

			/*
			****************
			UPDATE THE FRAME
			****************
			*/
			if (state == State::PLAYING)
			{
				// Update the delta time
				sf::Time dt = clock.restart();
				// Update the total game time
				gameTimeTotal += dt;
				// Make a decimal fraction of 1 from the delta time
				float dtAsSeconds = dt.asSeconds();

				// Where is the mouse pointer
				mouseScreenPosition = sf::Mouse::getPosition();

				// Convert mouse position to world coordinates of mainView
				mouseWorldPosition = window.mapPixelToCoords(
					sf::Mouse::getPosition(), mainView);

				// Update the player
				player.update(dtAsSeconds, sf::Mouse::getPosition());

				// Make a note of the players new position
				sf::Vector2f playerPosition(player.getCenter());

				// Make the view centre around the player				
				mainView.setCenter(player.getCenter());
			}// End updating the scene

			/*
			**************
			Draw the scene
			**************
			*/
			window.clear();

			if (state == State::PLAYING)
			{	
				window.clear();
				// set the mainView to be displayed in the window
				// And draw everything related to it
				window.setView(mainView);
				// Draw the background
				window.draw(background, &textureBackground);

				// Draw the player
				window.draw(player.getSprite());
			}

			if (state == State::LEVELING_UP)
			{
			}

			if (state == State::PAUSED)
			{
			}

			if (state == State::GAME_OVER)
			{
			}

			window.display();
		}

	}

	return 0;
}