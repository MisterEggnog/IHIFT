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
#ifndef MISTEREGGNOG_IHIFT_MONSTER_HPP_INCLUDED
#define MISTEREGGNOG_IHIFT_MONSTER_HPP_INCLUDED

#include <functional>
#include <memory>
#include <random>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
class Transformable;
}

namespace MisterEggnog::IHIFT {

class Projectile;

/** Monster class.
 * Call the various factory functions to get Monsters.
 */
class Monster final : public sf::Sprite {
	int health_;                            //!< Current health of the monster.
	sf::Vector2f area_;                     //!< Current area of the monster.
	std::shared_ptr<sf::Texture> texture_; //!< Texture of the monster.

	/** Move the Monster.
	* @arg reng: Random number generator.
	* @arg player_position: Current position of the Player.
	* @return Current area of the Monster.
	*/
	std::function<void(std::mt19937&, const sf::Vector2f&, sf::Transformable& monster)> move_fn_;

	/** Fire some Projectiles.
	* @arg reng: Random number generator.
	* @arg player_position: Current position of the Player.
	* @return Vector of Projectiles.
	*/
	std::function<std::vector<Projectile>(std::mt19937&, const sf::Vector2f, const sf::Transformable& monster)> attack_fn_;

	Monster(int health, sf::Vector2f area) noexcept : health_(health), area_(area) {}
	Monster& operator=(const Monster&) = delete;
	Monster(const Monster&) = delete;

	sf::FloatRect get_area() const;
public:

	~Monster() final;

	Monster() noexcept : Monster(0, sf::Vector2f(0,0)) {}
	Monster(Monster&&) noexcept = default;
	Monster& operator=(Monster&&) noexcept = default;

	/** Reduce entity health.
	* @arg damage_amount: How much health the monster loses.
	*/
	void damage(int damage_amount)
	{
		health_ -= damage_amount;
	}

	/** Get current health of Monster.
	* @return Current health.
	*/
	int get_current_health() const
	{
		return health_;
	}

	/** Still alive?
	* @return If health is not equal to 0.
	*/
	bool still_alive() const
	{
		return health_ != 0;
	}

	/** Move the Monster.
	* @arg reng: Random number generator.
	* @arg player_position: Current position of the Player.
	* @return Current area of the Monster.
	*/
	sf::FloatRect move(std::mt19937& reng, const sf::Vector2f& player_position)
	{
		move_fn_(reng, player_position, *this);
		return get_area();
	}

	/** Fire some Projectiles.
	* @arg reng: Random number generator.
	* @arg player_position: Current position of the Player.
	* @return Vector of Projectiles.
	*/
	std::vector<Projectile> attack(std::mt19937& reng, const sf::Vector2f& player_position)
	{
		return attack_fn_(reng, player_position, *this);
	}

	/** Build basic slime monster.
	* @arg reng: Random number generator.
	* @return Slime monster.
	*/
	static Monster slime_factory(std::mt19937& reng);
	
};

}

#endif // MISTEREGGNOG_IHIFT_MONSTER_HPP_INCLUDED
