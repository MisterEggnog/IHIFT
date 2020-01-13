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
#include "projectile.hpp"
#include "utils.hpp"

namespace MisterEggnog::IHIFT {

static constexpr int PROJECTILE_MAX_AGE = 10 * FRAMES_PER_SECOND;

Projectile::~Projectile() = default;

Projectile::Projectile(sf::Vector2f velocity, sf::Vector2f start_pos, int damage, bool friendly) noexcept : sf::RectangleShape(sf::Vector2f(4, 4)), age_(0), damage_(damage), friend_(friendly), velocity_(velocity), area_(4, 4)
{
	sf::Transformable::setPosition(start_pos);
}

Projectile::Projectile() noexcept : Projectile(sf::Vector2f(), sf::Vector2f(), 0, false)
{
	age_ = PROJECTILE_MAX_AGE;
}

bool Projectile::move() noexcept
{
/*	auto moved = velocity_ * (float)age_;
	printf("%s called: %f, %f\n", __func__, moved.x, moved.y);*/
	sf::Transformable::move(velocity_ * (float)age_++);
	return age_++ > PROJECTILE_MAX_AGE;
}

void Projectile::set_shape(sf::Vector2f dimension) noexcept
{
	sf::Transformable::setScale(dimension);
	area_ = dimension;
}

}
