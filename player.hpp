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
#ifndef MISTEREGGNOG_IHIFT_PLAYER_HPP_INCLUDED
#define MISTEREGGNOG_IHIFT_PLAYER_HPP_INCLUDED

#include "projectile.hpp"
#include <memory>
#include <optional>
#include <random>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
	class Texture;
}

namespace MisterEggnog::IHIFT {

/** Data associated with the player.
 * Only one instance should exist.
 * \todo Critical damage.
 */
class Player final : public sf::Sprite {
	float speed_;                          //!< How fast the player moves.
	char movement_status_;                 //!< Current movement combinations from the keyboard.
	sf::Vector2f area_;                    //!< Current size of the sprite.
	int basic_time_;                       //!< Iterator for basic fire clock.
	int burst_time_;                       //!< Iterator for burst fire clock.
	int clear_time_;                       //!< How long it takes the clear screen spell to cooldown.
	std::shared_ptr<sf::Texture> texture_; //!< Pointer to the players texture.
public:

	/**
	* Gives the relative directions assuming, -x is left, +x is right, -y is up, and +y is down.
	* More than one of these directions can be applied at a time, together they can give compound directions.
	*/
	enum directions {
		MOVE_UP    = 1 << 0,
		MOVE_LEFT  = 1 << 1,
		MOVE_DOWN  = 1 << 2,
		MOVE_RIGHT = 1 << 3
	};

	~Player() final;
	
	/**
	* Builds object at position (x,y).
	* @arg x: X position.
	* @arg y: Y position.
	*/
	Player(float x, float y);

	Player() : Player(0,0) {}

	/**
	* Tells the Player object what direction to move.
	* \todo Need to define what movement combinations are permited.
	*/
	void move_set(directions movement);

	/**
	* Changes position according to current movement_status_ and speed_.
	* @return Current area of the player.
	*/
	sf::FloatRect move(sf::Vector2f& offset);

	/** Reposition player.
	* Repositions the player at x,y.
	* @arg position_vector: x,y position.
	*/
	void set_position(sf::Vector2f position_vector) noexcept;

	/** Reposition player.
	* Repositions the player at x,y.
	* @arg x: X position.
	* @arg y: Y position.
	*/
	void set_position(float x, float y) noexcept
	{
		set_position(sf::Vector2f(x, y));
	}

	/** Get the rectangle enclosed by the Player.
	* @return The area & location of the Player.
	*/
	sf::FloatRect get_area() const
	{
		return sf::FloatRect(sf::Transformable::getPosition(), area_);
	}

	/** Reset internal timers.
	*/
	void reset_timers() noexcept;

	/** Basic shot factory.
	* Builds a basic (left mouse button) shot.
	* @arg mouse_coord: Position in the window where the mouse is fired.
	* @arg reng: Random number engine used for criticals.
	* @return Projectile traveling in the direction of the mouse, if
	* it has been more than basic_spell_cooldown_ since last called.
	*/
	std::optional<Projectile> basic_projectile(sf::Vector2f mouse_coord, std::mt19937& reng);

	/** Burst spell shot factory.
	* Builds a collection of projectiles that shoot from the player in a ring.
	* @arg reng: Random number engine used for criticals.
	* @return Vector of \link Projectile Projectiles \endlink in a ring if it has been more than
	* burst_spell_cooldown_ since last called.
	*/
	std::optional<std::vector<Projectile>> burst_spell(std::mt19937& reng);

	/** Clear screen spell is availible to use.
	* @return Clear screen spell is availible to use.
	*/
	bool can_clear();

	
};

}

#endif // MISTEREGGNOG_IHIFT_PLAYER_HPP_INCLUDED
