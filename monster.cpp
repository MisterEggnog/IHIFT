
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

struct Slime_mover : public virtual IHIFT::Monster {
	char movement_pattern_;
	int movement_period_;

	Slime_mover() noexcept = default;

	virtual ~Slime_mover() = default;

	virtual sf::FloatRect move(std::mt19937& reng, const sf::Vector2f& player_position)
	{
		return sf::FloatRect();
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

struct Slime_attacker : public virtual IHIFT::Monster {
	int base_damage_;
	int offset_min_;
	int offset_max_;

	Slime_attacker() noexcept = default;

	virtual ~Slime_attacker() = default;

	virtual std::vector<IHIFT::Projectile> attack(std::mt19937& reng, const sf::Vector2f& player_position)
	{
		std::vector<IHIFT::Projectile> projectiles;
		std::uniform_int_distribution critical_offset(offset_min_, offset_max_);

		std::uniform_int_distribution offset(0, 99);
		int pattern = offset(reng);

		// Straight to player
		if (pattern == 0) {
			auto pointer_to_player = player_position - sf::Transformable::getPosition();
				pointer_to_player *= 0.1f / std::hypotf(pointer_to_player.x, pointer_to_player.y);
//			printf("%f:%f", pointer_to_player.x, pointer_to_player.y);

			auto projectile = IHIFT::Projectile(pointer_to_player, sf::Transformable::getPosition() + sf::Transformable::getScale() * 0.5f, base_damage_ + critical_offset(reng), false);
			projectile.setFillColor(sf::Color::Green);
			projectile.setScale(4, 2);
			projectile.setRotation(std::atan2(pointer_to_player.x, pointer_to_player.y) * ONE_EIGHTY_DIV_PI);
			projectiles.push_back(std::move(projectile));
		}

		// Ray
		if (pattern == 1) {
			auto pointer_to_player = player_position - sf::Transformable::getPosition();
			pointer_to_player
		}

		return projectiles;
	}
};

}

// Voldemort types, lol

std::unique_ptr<IHIFT::Monster> IHIFT::Monster::slime_factory(std::mt19937& reng)
{
	struct Slime final : public Slime_mover, public Slime_attacker {
		~Slime() = default;
	};

	std::unique_ptr<Slime> monster = std::make_unique<Slime>();

	// General monster setup.
	monster->texture_ = IHIFT::get_tile_texture(12, 1);
	monster->setTexture(*monster->texture_);
	monster->setScale(2, 3);
	monster->setColor(sf::Color::Green);

	// Setup attack damage.
	monster->base_damage_ = 4;
	monster->offset_min_ = 1;
	monster->offset_max_ = 2;

	return monster;
}
