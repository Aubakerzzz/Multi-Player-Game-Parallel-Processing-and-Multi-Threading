#include <SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include <pthread.h>
using namespace std;

// Global variables
const int windowWidth = 800;
const int windowHeight = 800;

//player 1
const float playerSpeed = 4000.f;
bool movingUp = false;
bool movingDown = false;
bool movingLeft = false;
bool movingRight = false;


//player 2

const float playerSpeed1 = 4000.f;
bool movingw = false;
bool movings = false;
bool movinga = false;
bool movingd = false;

//Random Sprites On Board
sf::Sprite spritesRandom[10];	


// Struct for passing data to thread
struct ThreadData {
    sf::Sprite* player;
    sf::Time* deltaTime;
};


//player 2

struct ThreadData1 {
    sf::Sprite* player1;
    sf::Time* deltaTime1;
};



// Thread function for player movement1
// Thread function for player movement
void* playerMovementThread(void* data) {
    ThreadData* threadData = static_cast<ThreadData*>(data);
    sf::Sprite* player = threadData->player;
    sf::Time* deltaTime = threadData->deltaTime;

    while (true) {
        // Calculate player's movement direction based on key presses
        float xDirection = 0.f;
        float yDirection = 0.f;
        if (movingUp) yDirection -= 1.f;
        if (movingDown) yDirection += 1.f;
        if (movingLeft) xDirection -= 1.f;
        if (movingRight) xDirection += 1.f;

        // Calculate the potential new position of the player sprite
        sf::Vector2f newPosition = player->getPosition() + sf::Vector2f(xDirection, yDirection) * playerSpeed * deltaTime->asSeconds();

        // Check if the new position is within the boundaries of the grid
        if (newPosition.x < 0) {
            newPosition.x = 0;
        } else if (newPosition.x > 800 - player->getGlobalBounds().width) {
            newPosition.x = 800 - player->getGlobalBounds().width;
        }
        if (newPosition.y < 0) {
            newPosition.y = 0;
        } else if (newPosition.y > 800 - player->getGlobalBounds().height) {
            newPosition.y = 800 - player->getGlobalBounds().height;
        }

        // Move the player sprite to the new position
        player->setPosition(newPosition);

        // Check for collision with random sprites
        for (int i = 0; i < 10; i++) {
            sf::Sprite& randomSprite = spritesRandom[i];
            if (player->getGlobalBounds().intersects(randomSprite.getGlobalBounds())) {
                // Remove the random sprite from the board
                spritesRandom[i].setPosition(sf::Vector2f(-1000, -1000));
            }
        }

        // Sleep for a short period to reduce CPU usage
        sf::sleep(sf::milliseconds(10));
    }
}




// Thread function for player movement 2
void* playerMovementThread1(void* data1) {
    ThreadData1* threadData1 = static_cast<ThreadData1*>(data1);
    sf::Sprite* player1 = threadData1->player1;
    sf::Time* deltaTime1 = threadData1->deltaTime1;

    while (true) {
        // Calculate player's movement direction based on key presses
        float xDirection1 = 0.f;
        float yDirection1 = 0.f;
        if (movingw)
            yDirection1 -= 1.f;
        if (movings)
            yDirection1 += 1.f;
        if (movinga)
            xDirection1 -= 1.f;
        if (movingd)
            xDirection1 += 1.f;

        // Calculate new position of player sprite
        sf::Vector2f movement1(xDirection1, yDirection1);
        float movementLength1 = (movement1.x * movement1.x + movement1.y * movement1.y);
        if (movementLength1 > 0.f) {
            movement1 /= movementLength1;
            movement1 *= playerSpeed1 * deltaTime1->asSeconds();
            sf::Vector2f newPosition = player1->getPosition() + movement1;

            // Check if new position is outside the grid, and adjust movement vector if necessary
            if (newPosition.x < 0.f)
                movement1.x = -player1->getPosition().x;
            else if (newPosition.x > 800.f)
                movement1.x = 800.f - player1->getPosition().x;
            if (newPosition.y < 0.f)
                movement1.y = -player1->getPosition().y;
            else if (newPosition.y > 800.f)
                movement1.y = 800.f - player1->getPosition().y;

            // Check if player sprite is colliding with any of the random sprites
            for (int i = 0; i < 10; i++) {
                sf::FloatRect playerBounds = player1->getGlobalBounds();
                sf::FloatRect spriteBounds = spritesRandom[i].getGlobalBounds();
                if (playerBounds.intersects(spriteBounds)) {
                    // Remove the sprite from the board
                    spritesRandom[i].setPosition(sf::Vector2f(-1000.f, -1000.f));
                }
            }

            // Move player sprite
            player1->move(movement1);
        }

        // Sleep for a short period to reduce CPU usage
        sf::sleep(sf::milliseconds(10));
    }
}


int main() {
    // Create window and player sprite
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML with pthread.h");
    
    	
    // Set up grid variables
    const int GRID_SIZE = 10;
    const int CELL_SIZE = window.getSize().x / GRID_SIZE;
    
    //player 1
    sf::Texture playerTexture;
    playerTexture.loadFromFile("player.png");
    playerTexture.setSmooth(true); // enable texture smoothing
    sf::Sprite player(playerTexture);
    player.setOrigin(playerTexture.getSize().x / 2.f, playerTexture.getSize().y / 2.f);
    player.setScale(0.05f, 0.05f);
    player.setPosition(CELL_SIZE * 5 + CELL_SIZE / 2.f, CELL_SIZE + CELL_SIZE / 2.f);
    // Create delta time object for smooth movement
    sf::Clock clock;
    sf::Time deltaTime = sf::Time::Zero;
    
    //player 2
    sf::Texture playerTexture1;
    playerTexture1.loadFromFile("player.png");
    playerTexture1.setSmooth(true); // enable texture smoothing
    sf::Sprite player1(playerTexture1);
    player1.setOrigin(playerTexture1.getSize().x / 2.f, playerTexture1.getSize().y / 2.f);
    player1.setScale(0.05f, 0.05f);
    // set scale to reduce size
    player1.setPosition(CELL_SIZE * 5 + CELL_SIZE / 2.f, CELL_SIZE * (GRID_SIZE - 1) + CELL_SIZE / 2.f);
    // Create delta time object for smooth movement
    sf::Clock clock1;
    sf::Time deltaTime1 = sf::Time::Zero;
    
    
    
    // Create thread for player1 movement
    ThreadData threadData = { &player, &deltaTime };
    pthread_t thread;
    pthread_create(&thread, NULL, playerMovementThread, &threadData);
    // Create thread for player2 movement
    ThreadData1 threadData1 = { &player1, &deltaTime1 };
    pthread_t thread1;
    pthread_create(&thread1, NULL, playerMovementThread1, &threadData1);
    



    // Create cell shape
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cell.setFillColor(sf::Color::White);
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color::Black);


	// Random Sprites On The Board
	
	sf::Texture spriteTexture;
	spriteTexture.loadFromFile("coin.png");
	spriteTexture.setSmooth(true);


	// Set up random number generator
	srand(time(NULL));

	// Generate random positions for sprites and set their positions
	for (int i = 0; i < 10; i++) {
		int x, y;
		bool positionOccupied;
		do {
		    // Generate random position
		    x = rand() % GRID_SIZE;
		    y = rand() % GRID_SIZE;

		    // Check if position is occupied by another sprite
		    positionOccupied = false;
		    for (int j = 0; j < i; j++) {
		        if (spritesRandom[j].getPosition() == sf::Vector2f(x * CELL_SIZE + CELL_SIZE / 2.f, y * CELL_SIZE + CELL_SIZE / 2.f)) {
		            positionOccupied = true;
		            break;
		        }
		    }
		} while (positionOccupied);

		// Set sprite texture, scale and position
		spritesRandom[i].setTexture(spriteTexture);
		spritesRandom[i].setScale(0.05f, 0.05f);
		spritesRandom[i].setOrigin(spriteTexture.getSize().x / 2.f, spriteTexture.getSize().y / 2.f);
		spritesRandom[i].setPosition(x * CELL_SIZE + CELL_SIZE / 2.f, y * CELL_SIZE + CELL_SIZE / 2.f);
	}
	    
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) 
                	movingUp = true;
                else if (event.key.code == sf::Keyboard::Down) 
                	movingDown = true;
                else if (event.key.code == sf::Keyboard::Left) 
                	movingLeft = true;
                else if (event.key.code == sf::Keyboard::Right) 
                	movingRight = true;
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Up) 
                	movingUp = false;
                else if (event.key.code == sf::Keyboard::Down) 
                	movingDown = false;
               else if (event.key.code == sf::Keyboard::Left) 
               		movingLeft = false;
				else if (event.key.code == sf::Keyboard::Right) 
					movingRight = false;
			}
			
		    if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) 
                	movingw = true;
                else if (event.key.code == sf::Keyboard::S) 
                	movings = true;
                else if (event.key.code == sf::Keyboard::A) 
                	movinga = true;
                else if (event.key.code == sf::Keyboard::D) 
                	movingd = true;
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) 
                	movingw = false;
                else if (event.key.code == sf::Keyboard::S) 
                	movings = false;
               else if (event.key.code == sf::Keyboard::A) 
               		movinga = false;
				else if (event.key.code == sf::Keyboard::D) 
					movingd = false;
			}
		}
		  // Calculate delta time for smooth movement
    	 deltaTime = clock.restart();
	   	 // Calculate delta time for smooth movement
   		 deltaTime1 = clock1.restart();

    // Clear window and draw player
    window.clear();
            

		// Draw grid
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE; y++) {
                // Set position of cell
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                // Draw cell
                window.draw(cell);
            }
        }
    	 // Draw sprites
		for (int i = 0; i < 10; i++) {
    		window.draw(spritesRandom[i]);
		}
		
	window.draw(player);
    window.draw(player1);
    
    window.display();
}

// Wait for player movement thread to finish
pthread_join(thread, NULL);
pthread_join(thread1, NULL);

return 0;

}
