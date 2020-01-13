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
#ifndef MISTEREGGNOG_IHIFT_ENTITY_HPP_INCLUDED
#define MISTEREGGNOG_IHIFT_ENTITY_HPP_INCLUDED
/** Header for functions to handle entities.
 * @file entity.hpp
 */

#include <map>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics/Rect.hpp>

namespace MisterEggnog::IHIFT {

/** List entity type in entity pool.
 * \headerfile ""
 */
enum entity_type {
	PLAYER_TYPE,  //!< Player.
	FLASER,       //!< Laser shot by player.
	ELASER,       //!< Laser shot by monster.
	MONSTER,      //!< Monster.
	FBEAM,        //!< Friendly solid beam, that hurts to walk through.
	EBEAM,        //!< Enemy solid beam, that hurts to walk through.
	WALL          //!< Non-visible wall.
};

/** Get new entity id.
 * @return New entity id.
 */
int new_id() noexcept;

/** Find entities that are intersecting.
 * @arg pool: Pool of entities.
 * @return Collision pairs.
 */
std::vector<std::pair<int, int>> get_collisions(const std::unordered_map<int, sf::IntRect>& pool) noexcept;

/** Dispose select entities.
 * @arg entities_to_dispose: List of id's to kill.
 * @arg entity_map: List of entities.
 */
template <class T>
inline void dispose(const std::vector<int>& entities_to_dispose, std::unordered_map<int, T>& entity_map) noexcept
{
	for (int i: entities_to_dispose)
		entity_map.erase(i);
}

/** Dispose select entities.
 * @arg entities_to_dispose: List of id's to kill.
 * @arg entity_map: List of entities.
 */
template <class T>
inline void dispose(const std::vector<int>& entities_to_dispose, std::map<int, T>& entity_map) noexcept
{
	for (int i: entities_to_dispose)
		entity_map.erase(i);
}

}

#endif // MISTEREGGNOG_IHIFT_ENTITY_HPP_INCLUDED
