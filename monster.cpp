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
#include "monster.hpp"
#include "projectile.hpp"
#include "utils.hpp"
#include <cmath>
#include <cstdio>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

#define ONE_EIGHTY_DIV_PI 57.29577951308232087721

using namespace MisterEggnog;

IHIFT::Monster::~Monster() = default;

namespace {

struct Slime_mover {
	char movement_pattern_;
	int movement_period_;

	Slime_mover() noexcept = default;

	void operator()(std::mt19937& reng, const sf::Vector2f& player_position, sf::Transformable& monster)
	{
	}
};
/*	sf::FloatRect move(std::mt19937& reng, const sf::Vector2f& player_position) final
	{
		// Movement patterns currently include 0-1

		if (movement_pattern_ < 0) {
			std::uniform_int_distribution movement_pattern(0, 1);
			movement_pattern_ = movement_pattern(reng);
			movement_pattern_ ^= movement_pattern_;
		}

		if (movement_pattern_ == 0) {
			if (movement_period_ == 0)
				movement_period_ = 7 * 60;

			if (movement_period_ < (3 * 60)) {
				auto unit_vec_to_player = player_position - sf::Transformable::getPosition();
				unit_vec_to_player *= 3.f / (std::hypotf(unit_vec_to_player.x, unit_vec_to_player.y));
				sf::Transformable::move(unit_vec_to_player);
			}
			else
				printf("Pause\n");

			printf("%i\n", movement_period_);

		}

		if (movement_period_ == 0)
			movement_pattern_ = -1;
		movement_period_--;
		return sf::FloatRect();
	}*/

struct Slime_attacker {
	int base_damage_;
	int offset_min_;
	int offset_max_;

	std::vector<IHIFT::Projectile> operator()(std::mt19937& reng, const sf::Vector2f& player_position, const sf::Transformable& monster)
	{
		std::vector<IHIFT::Projectile> projectiles;
		std::uniform_int_distribution critical_offset(offset_min_, offset_max_);

		std::uniform_int_distribution offset(0, 99);
		int pattern = offset(reng);

		// Straight to player
		if (pattern == 0) {
			auto pointer_to_player = player_position - monster.getPosition();
				pointer_to_player *= 0.1f / std::hypotf(pointer_to_player.x, pointer_to_player.y);
//			printf("%f:%f", pointer_to_player.x, pointer_to_player.y);

			auto projectile = IHIFT::Projectile(pointer_to_player, monster.getPosition() + monster.getScale() * 0.5f, base_damage_ + critical_offset(reng), false);
			projectile.setFillColor(sf::Color::Green);
			projectile.setScale(4, 2);
			projectile.setRotation(std::atan2(pointer_to_player.x, pointer_to_player.y) * ONE_EIGHTY_DIV_PI);
			projectiles.push_back(std::move(projectile));
		}

		// Ray
		if (pattern == 1) {
			auto pointer_to_player = player_position - monster.getPosition();
//			pointer_to_player
		}

		return projectiles;
	}
};

}

IHIFT::Monster IHIFT::Monster::slime_factory(std::mt19937& reng)
{
	Monster slime;

	// General monster setup.
	slime.texture_ = IHIFT::get_tile_texture(12, 1);
	slime.setTexture(*slime.texture_);
	slime.setScale(2, 3);
	slime.setColor(sf::Color::Green);

	// Setup attack damage.
	Slime_attacker attack{4, 1, 2};
	slime.attack_fn_ = attack;

	// Setup move
	Slime_mover mover;
	slime.move_fn_ = mover;

	return slime;
}
