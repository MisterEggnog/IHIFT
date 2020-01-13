/*
	Trash dyi bullet hell/roguelike.
	Copyright (C) 2020  Baldwin, Josiah

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "entity.hpp"
#include "monster.hpp"
#include "player.hpp"
#include "projectile.hpp"
#include "utils.hpp"
#include <cstdio>
#include <cstring>
#include <random>
#include <map>
#include <unordered_map>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

// Unix tools
#include <unistd.h>

#define WINDOW_START_WIDTH  1000
#define WINDOW_START_HEIGHT 562

using namespace MisterEggnog;

struct Game {
	std::mt19937 reng_;                           //!< Rng engine.
	IHIFT::Player player_;                       //!< Player token.
	std::unordered_map<int, sf::FloatRect> areas_; //!< List of area's entities and id's. Negative one is the id of the player.
	std::unordered_map<int, IHIFT::entity_type> types_; //!< List of entities.
	std::map<int, IHIFT::Projectile> projectiles_; //!< List of currently living Projectiles.
	std::map<int, std::unique_ptr<IHIFT::Monster>> monsters_;

	Game() noexcept;

	/** Draw, execute function.
	* This function delegates work to other functions, except for events.
	*/
	void game_loop(sf::RenderWindow* window) noexcept;

	/** Render function
	* @arg window: Window drawn to.
	*/
	void draw(sf::RenderWindow* window) noexcept;

	/** Moves all entities.
	* Also dispose of old Projectiles.
	*/
	void move_entities(sf::RenderWindow* window) noexcept;

	/** Handle keyboard operations.
	*/
	void key_press() noexcept;

	/** Handles mouse operations.
	* @arg window: Window to determine where the mouse is operating.
	*/
	void mouse_handle(sf::RenderWindow* window) noexcept;
};

int main(int argc, char** argv)
{
	std::puts("I Hope I Finish This");
	std::puts("");

    std::puts("IHIFT  Copyright (C) 2020  Baldwin, Josiah\n"
    			"This program comes with ABSOLUTELY NO WARRANTY; for details pass '-w'.\n"
    			"This is free software, and you are welcome to redistribute it\n"
    			"under certain conditions; pass '-c' for details.\n");

	if (argc > 1) {
		char flags = 0;
		int c;

		opterr = 0;

		while ((c = getopt(argc, argv, "hcw")) != -1)
			switch (c) {
				case 'h':
					flags ^= 1 << 0;
					puts("Left mouse to fire basic shot.\n"
					 "Space to fire burst\n"
					 "WASD/Arrow keys to move\n"
					 "Num 4 to delete all living projectiles.\n"
					 "P to clear timers, (this is a debug feature.)");
					break;
				case 'c':
					flags ^= 1 << 1;
					std::printf("TODO whatever the hell this \'%c\' argument is supposed to do.\n", 'c');
					break;
				case 'w':
					flags ^= 1 << 2;
					std::printf("TODO whatever the hell this \'%c\' argument is supposed to do.\n", 'w');
					break;
				default: break;
			}
	}
	else
		std::puts("Pass -h for more info");

	Game game;
	std::printf("Game init complete. Line %i\n", __LINE__);
	sf::RenderWindow window(sf::VideoMode(WINDOW_START_WIDTH, WINDOW_START_HEIGHT),
		"I Hope I Finish This");
	window.setFramerateLimit(60);
	
	game.game_loop(&window);
}

Game::Game() noexcept
{
/*	std::random_device rd;
	reng_.seed(rd());*/

	types_[-1] = IHIFT::PLAYER_TYPE;
	auto id = IHIFT::new_id();
	monsters_[id] = std::move(IHIFT::Monster::slime_factory(reng_));
	monsters_[id]->setPosition(WINDOW_START_WIDTH / 2, WINDOW_START_HEIGHT /2);
}

	
void Game::game_loop(sf::RenderWindow* window) noexcept
{
	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window->close();
					break;
				default: break;
			}
		}
		key_press();
		move_entities(window);
		mouse_handle(window);
		draw(window);
	}
}

void Game::draw(sf::RenderWindow* window) noexcept
{
	window->clear(sf::Color::Black);

	for (auto& i: projectiles_) {
		window->draw(i.second);
	}

	for (auto& i: monsters_) {
		window->draw(*(i.second));
	}

	window->draw(player_);

	window->display();
}

void Game::move_entities(sf::RenderWindow* window) noexcept
{
	{
		sf::Vector2f offset;
		areas_[-1] = player_.move(offset);

		if (0) {
			auto view_port = window->getView();
			view_port.move(offset);
			window->setView(view_port);
		}
	}

	{
		sf::Vector2f player_position = player_.getPosition();
		for (auto& i: monsters_) {
//			i.second->move(reng_, player_position);
			auto projectiles = i.second->attack(reng_, player_position);

			for (auto i = 0U; i < projectiles.size(); i++) {
				int id = IHIFT::new_id();
				projectiles_.try_emplace(id, std::move(projectiles[i]));
			}
		}
	}

	std::vector<int> projectiles_to_kill_;
	for (auto& i: projectiles_) {
		if (i.second.move()) {
			projectiles_to_kill_.push_back(i.first);
		}
		else
			areas_[i.first] = i.second.get_area();
	}
	IHIFT::dispose(projectiles_to_kill_, projectiles_);
	IHIFT::dispose(projectiles_to_kill_, types_);
	IHIFT::dispose(projectiles_to_kill_, areas_);
}

void Game::key_press() noexcept
{
	if (IHIFT::register_down()) {
		player_.move_set(IHIFT::Player::MOVE_DOWN);
	}
	if (IHIFT::register_up()) {
		player_.move_set(IHIFT::Player::MOVE_UP);
	}
	if (IHIFT::register_left()) {
		player_.move_set(IHIFT::Player::MOVE_LEFT);
	}
	if (IHIFT::register_right()) {
		player_.move_set(IHIFT::Player::MOVE_RIGHT);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P))
		player_.reset_timers();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		auto projectiles = player_.burst_spell(reng_);
		if (projectiles) {
//			printf("%zi\n", (*projectiles).size());
			for (auto i: *projectiles) {
				int id = IHIFT::new_id();
				projectiles_[id] = i;
				types_[id] = IHIFT::FBEAM;
				areas_[id] = i.get_area();
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
		if (player_.can_clear()) {
			projectiles_.clear();
		}
}

void Game::mouse_handle(sf::RenderWindow* window) noexcept
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		auto projectile = player_.basic_projectile(window->mapPixelToCoords(sf::Mouse::getPosition(*window)), reng_);
		if (projectile) {
			int id = IHIFT::new_id();
			projectiles_[id] = *projectile;
			types_[id] = IHIFT::FBEAM;
			areas_[id] = projectile->get_area();
		}
	}
}
