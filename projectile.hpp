#ifndef MISTEREGGNOG_IHIFT_PROJECTILE_HPP_INCLUDED
#define MISTEREGGNOG_IHIFT_PROJECTILE_HPP_INCLUDED

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace MisterEggnog::IHIFT {

/** Entity that travels in a straight line.
 * Projectiles die when they collide with the Player, a Wall, or a Monster.
 * All projectiles will die after a certain period of time, currently 60 seconds.
 */
class Projectile final : public sf::RectangleShape {
	int age_;               //!< How old is this projectile?
	int damage_;            //!< How much damage this deals.
	bool friend_;           //!< Was the projectile shot by the player?
	sf::Vector2f velocity_; //!< How fast does this travel?
	sf::Vector2f area_;     //!< Current size of the sprite.
public:

	Projectile() noexcept;

	/** Do everything constructor.
	* Things need to be pretty slow here.
	* @arg velocity: Speed projectile travels at.
	* @arg start_pos: Where the projectile spawns, relative to the board.
	* @arg damage: How much the projectile hurts.
	* @arg friendly: True if the projectile will not hurt the player.
	*/
	Projectile(sf::Vector2f velocity, sf::Vector2f start_pos, int damage, bool friendly) noexcept;

	~Projectile() final;

	/** Is the projectile shot by the player?
	* @return Is the projectile shot by the player?
	*/
	bool is_friendly() const noexcept { return friend_; }

	/** Change projectile ally status.
	* @arg status: Ally status.
	*/
	void set_friendly(bool status) noexcept { friend_ = status; }

	/** How much damage does it yield?
	* @return How much damage does it yield?
	*/
	int get_damage() const noexcept { return damage_; }

	/** Change payload.
	* @arg damage: How much damage.
	*/
	void set_damage(int damage) noexcept { damage_ = damage; }

	/** Move projectile & age it.
	* @return age_ < max_age_
	*/
	bool move() noexcept;

	/** Get the rectangle enclosed by the Projectile.
	* @return The area & location of the Projectile.
	*/
	sf::FloatRect get_area() const
	{
		return sf::FloatRect(sf::Transformable::getPosition(), area_);
	}

	/** Change rectangle settings.
	* @arg dimension: New width & height of **this**.
	*/
	void set_shape(sf::Vector2f dimension) noexcept;

	/** Change rectangle settings.
	* @arg x: New width of **this**.
	* @arg y: New height of **this**.
	*/
	void set_shape(float x, float y) noexcept { set_shape(sf::Vector2(x, y)); }
};

}

#endif // MISTEREGGNOG_IHIFT_PROJECTILE_HPP_INCLUDED
