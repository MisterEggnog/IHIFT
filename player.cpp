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
#include "player.hpp"
#include "utils.hpp"
#include <cmath>

#ifndef MATH_2PI
#define MATH_2PI 6.283185307179586472
#endif

namespace MisterEggnog::IHIFT {

constexpr float SPEED = 1.5f;

Player::~Player() = default;

Player::Player(float x, float y) : speed_(SPEED), movement_status_(0), area_(16, 16)
{
	texture_ = get_tile_texture(31, 1);
	sf::Sprite::setTexture(*texture_);
	sf::Sprite::setPosition(x, y);
	sf::Sprite::setScale(2.f, 2.f);
	reset_timers();
}

void Player::move_set(directions movement)
{
//	std::puts("Registered move");
	movement_status_ |= movement;
}

sf::FloatRect Player::move(sf::Vector2f& offset)
{
	float partial_movement = speed_ * 0.707107f;
	offset = sf::Vector2f(0, 0);
	switch(movement_status_) {
		case MOVE_DOWN:
		case MOVE_LEFT | MOVE_RIGHT | MOVE_DOWN:
//			std::puts("Move down");
			offset += sf::Vector2f(0, speed_);
			break;
		case MOVE_UP:
		case MOVE_LEFT | MOVE_RIGHT | MOVE_UP:
//			std::puts("Move up");
			offset += sf::Vector2f(0, -speed_);
			break;
		case MOVE_LEFT:
//			std::puts("Move left");
			offset += sf::Vector2f(-speed_, 0);
			break;
		case MOVE_RIGHT:
//			std::puts("Move right");
			offset += sf::Vector2f(speed_, 0);
			break;
		case MOVE_UP | MOVE_RIGHT:
//			std::puts("Move up-right");
			offset += sf::Vector2f(partial_movement, -partial_movement);
			break;
		case MOVE_UP | MOVE_LEFT:
//			std::puts("Move up-left");
			offset += sf::Vector2f(-partial_movement, -partial_movement);
			break;
		case MOVE_DOWN | MOVE_RIGHT:
//			std::puts("Move down-right");
			offset += sf::Vector2f(partial_movement, partial_movement);
			break;
		case MOVE_DOWN | MOVE_LEFT:
//			std::puts("Move down-left");
			offset += sf::Vector2f(-partial_movement, partial_movement);
			break;
		default: break;
	}
	movement_status_ ^= movement_status_;
	sf::Transformable::move(offset);

	// Update ability wait times
	basic_time_++;
	burst_time_++;

	return get_area();
}

static constexpr int BASIC_FIRE_COOLDOWN = 30; //!< How long does basic shot take to cooldown.
static constexpr int BURST_WAIT_TIME = 45*60;

static constexpr int CLEAR_WAIT_TIME = 2 * 60 * 60;

void Player::reset_timers() noexcept
{
	basic_time_ = BASIC_FIRE_COOLDOWN;
	burst_time_ = BURST_WAIT_TIME;
	clear_time_ = CLEAR_WAIT_TIME;
}

// Speed is in units of per 1/60s
static constexpr float BASIC_SHOT_SPEED = 5.f / 60.f;

static constexpr float BASIC_SHOT_DAMAGE = 15.f;

static int basic_projectile_damage(int base, std::mt19937& reng) noexcept
{	
	// TO DO Critical offsets?
	return base;
}

std::optional<Projectile> Player::basic_projectile(sf::Vector2f mouse_coord, std::mt19937& reng)
{
	if (basic_time_ < BASIC_FIRE_COOLDOWN)
		return {};

	basic_time_ ^= basic_time_;

	// This should get the unit vector of the direction.
	auto direction = mouse_coord - sf::Transformable::getPosition();
	direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// This should give it right speed.
	direction *= BASIC_SHOT_SPEED;

	// Put it all together.
	Projectile basic_projectile(direction, sf::Transformable::getPosition() + sf::Vector2(16.f, 16.f), basic_projectile_damage(BASIC_SHOT_DAMAGE, reng), true);
	return basic_projectile;
}

static constexpr int BURST_COUNT = 60;

std::optional<std::vector<Projectile>> Player::burst_spell(std::mt19937& reng)
{
	if (burst_time_ < BURST_WAIT_TIME)
		return {};

	burst_time_ = 0;

	std::vector<IHIFT::Projectile> projectiles;
	for (float i = 0; i < MATH_2PI; i += MATH_2PI / (float)BURST_COUNT) {
		projectiles.emplace_back(sf::Vector2(cosf(i), sinf(i)) *
			BASIC_SHOT_SPEED, sf::Transformable::getPosition() + sf::Vector2(16.f, 16.f),
			basic_projectile_damage(BASIC_SHOT_DAMAGE, reng), true);
	}

	return projectiles;
}

bool Player::can_clear()
{
	if (clear_time_ >= CLEAR_WAIT_TIME) {
		clear_time_ = 0;
		return true;
	}
	return false;
}

}
