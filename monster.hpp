#ifndef MISTEREGGNOG_IHIFT_MONSTER_HPP_INCLUDED
#define MISTEREGGNOG_IHIFT_MONSTER_HPP_INCLUDED

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

/** Abstract Monster class.
 * Call the various factory functions to get Monsters.
 */
class Monster : public sf::Sprite {
protected:
	int health_;            //!< Current health of the monster.
	sf::Vector2f area_;     //!< Current area of the monster.
	std::shared_ptr<sf::Texture> texture_;

	Monster(int health, sf::Vector2f area) noexcept : health_(health), area_(area) {}
	Monster() noexcept : Monster(0, sf::Vector2f(0,0)) {}
	Monster& operator=(const Monster&) = delete;
	Monster(const Monster&) = delete;
	Monster(Monster&&) = delete;
	Monster& operator=(Monster&&) noexcept = delete;
public:

	virtual ~Monster();

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
	virtual sf::FloatRect move(std::mt19937& reng, const sf::Vector2f& player_position) = 0;

	/** Fire some Projectiles.
	* @arg reng: Random number generator.
	* @arg player_position: Current position of the Player.
	* @return Vector of Projectiles.
	*/
	virtual std::vector<Projectile> attack(std::mt19937& reng, const sf::Vector2f& player_position) = 0;

	/** Build basic slime monster.
	* @arg reng: Random number generator.
	* @return Slime monster.
	*/
	static std::unique_ptr<Monster> slime_factory(std::mt19937& reng);
	
};

}

#endif // MISTEREGGNOG_IHIFT_MONSTER_HPP_INCLUDED
