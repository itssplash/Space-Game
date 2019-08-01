#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <sstream>
#include "Button.h"
#include <vector>
int main()
{
	end:
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1080,720), "Space Game");
	sf::FloatRect windowbounds({ 0.f,0.f }, window.getDefaultView().getSize());
	window.setFramerateLimit(60);
	sf::Texture t;
	sf::Sprite s;
	if (!t.loadFromFile("joy.jpg"))
	{
		return EXIT_FAILURE;
	}
	s.setTexture(t);
	s.setScale(0.9f, 1.0f);


	//Button
	Button btn({200,25},"START", sf::Color::Transparent, sf::Color::Black, "Pacifico.ttf");
	Button btn1({ 200,25 }, "EXIT", sf::Color::Transparent, sf::Color::White, "Pacifico.ttf");
	Button btn2({ 200,25 }, "RESTART", sf::Color::Transparent, sf::Color::White, "Pacifico.ttf");
	btn.setPos({ 480,600 });
	btn1.setPos({ 900,10 });
	btn2.setPos({ 900,60 });

	//player
	sf::Texture pl;
	sf::Sprite player;
	if (!pl.loadFromFile("ufo.png"))
	{
		return EXIT_FAILURE;
	}
	player.setTexture(pl);
	player.setScale(0.15f, 0.15f);
	player.setPosition(540, 640);


	//bullet
	sf::RectangleShape bullet;
	bullet.setSize(sf::Vector2f(10,5));
	bullet.setFillColor(sf::Color::Red);
	std::vector<sf::RectangleShape>bullets;
	bullets.push_back(sf::RectangleShape(bullet));


	//Enemy
	sf::Texture e;
	sf::Sprite enemy;
	if (!e.loadFromFile("sp.png"))
	{
		return EXIT_FAILURE;
	}
	enemy.setTexture(e);
	enemy.setColor(sf::Color::Magenta);
	enemy.setScale(0.15f, 0.15f);
	std::vector<sf::Sprite>enemies;
	enemies.push_back(sf::Sprite(enemy));


	sf::Vector2f plyrcenter;
	int shootTimer = 0;
	int respawnTime = 0;

	//score
	int score = 0;

	sf::Font arial;
	arial.loadFromFile("Pacifico.ttf");

	std::ostringstream ssScore;
	ssScore << "Score -> " << score;

	sf::Text lblScore;
	lblScore.setCharacterSize(30);
	lblScore.setPosition({ 10, 10 });
	lblScore.setFont(arial);
	lblScore.setString(ssScore.str());

	//GameOver
	sf::Texture gm;
	sf::Sprite gameover;
	if (!gm.loadFromFile("gameover.jpg"))
	{
		return EXIT_FAILURE;
	}
	gameover.setTexture(gm);
	gameover.setScale(0.6f, 0.7f);


	sf::Texture bg;
	sf::Sprite g;
	if (!bg.loadFromFile("C48.jpg"))
	{
		return EXIT_FAILURE;
	}
	g.setTexture(bg);
	g.setScale(0.7f, 0.8f);


	int count = 0;
	int flag = 0;


	sf::Image icon;
	if (!icon.loadFromFile("o.png"))
	{
		return EXIT_FAILURE;
	}
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());



	//Sound
	sf::Music m;
	if (!m.openFromFile("bulletsound.ogg"))
	{
		return EXIT_FAILURE;
	}
	//First window
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (btn.ismoseover(window))
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				//Window after Start button
				while (window.isOpen())
				{
					sf::Event event1;
					while (window.pollEvent(event1))
					{
						if (event1.type == sf::Event::Closed)
							window.close();
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
					{
						player.move(-10,0);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
					{
						player.move(10, 0);
					}
					sf::Vector2f posply(player.getPosition().x, player.getPosition().y);
					posply.x = std::max(posply.x, windowbounds.left);
					posply.x = std::min(posply.x, windowbounds.left + windowbounds.width - player.getGlobalBounds().height);
					player.setPosition(posply);

					//Bullet
					plyrcenter = sf::Vector2f(player.getPosition().x + player.getGlobalBounds().height - 40, player.getPosition().y);
					if (shootTimer < 5)
						shootTimer++;

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootTimer>=5)
					{
						m.play();
						bullet.setPosition(plyrcenter);
						bullets.push_back(sf::RectangleShape(bullet));

						shootTimer = 0;

					}
					

					for (int i = 0; i < bullets.size(); i++)
					{
						bullets[i].move(0.f, -10.0f);
						if (bullets[i].getPosition().y <= 0)
						{
							bullets.erase(bullets.begin() + i);
						}
					}
					//Enemy
			
					if (respawnTime < 20)
					{
						respawnTime++;
					}
					if (respawnTime >= 20)
					{
						enemy.setPosition((rand() % int(window.getSize().x-50)), 0.f);
						enemies.push_back(sf::Sprite(enemy));

						respawnTime = 0;
					}
					for (int i = 0; i < enemies.size(); i++)
					{
						enemies[i].move(0.f, 5.f);
						if (enemies[i].getPosition().y >= window.getSize().y-100)
						{
							count++;
							enemies.erase(enemies.begin() + i);
							if (count > 4)
							{
								flag = 4;
							}
						}
					}
					
					//Collision
					for (int i = 0; i < bullets.size(); i++)
					{
						for (int k = 0; k < enemies.size(); k++)
						{
							if (bullets[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
							{
								bullets.erase(bullets.begin() + i);
								enemies.erase(enemies.begin() + k);
								score++;
								ssScore.str("");
								ssScore << "Score ->" << score;
								lblScore.setString(ssScore.str());
								break;
							}
						}
					}
					for (int i = 0; i < enemies.size(); i++)
					{
					    if (enemies[i].getGlobalBounds().intersects(player.getGlobalBounds()))
					    { 
							while (window.isOpen())
							{
								sf::Event event;
								while (window.pollEvent(event))
								{
									if (event.type == sf::Event::Closed)
										window.close();

								}
								window.clear();
								window.draw(gameover);
								window.draw(lblScore);
								window.display();
							}
						
				     	}
					}
			
			
	
					window.clear();
					if (flag == 4)
					{
						while (window.isOpen())
						{
							sf::Event event;
							while (window.pollEvent(event))
							{
								if (event.type == sf::Event::Closed)
									window.close();

							}
							window.clear();
							window.draw(gameover);
							window.draw(lblScore);
							window.display();
						}
					}
					window.draw(g);
					window.draw(player);
					window.draw(lblScore);
					btn1.draw(window);
					btn2.draw(window);
					if (btn1.ismoseover(window))
					{
					
						if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							//Window after Start button
							while (window.isOpen())
							{
								sf::Event event2;
								while (window.pollEvent(event2))
								{
									if (event2.type == sf::Event::Closed)
										window.close();
								}
								window.clear();
								window.draw(gameover);
								window.display();
							}
						}
						
					}
					if (btn2.ismoseover(window))
					{

						if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
						{
							goto end;
						}

					}
					for (int i = 0; i < enemies.size(); i++)
					{
						window.draw(enemies[i]);
					}
					for (int i = 0; i < bullets.size(); i++)
					{
						window.draw(bullets[i]);
					}
					window.display();
				}
			}
		}

		window.clear();
		window.draw(s);
		btn.draw(window);
		window.display();
	}

	return 0;
}