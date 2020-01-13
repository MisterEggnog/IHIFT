/** Header for functions to handle entities.
 * @file entity.hpp
 */
#ifndef MISTEREGGNOG_IHIFT_ENTITY_HPP_INCLUDED
#define MISTEREGGNOG_IHIFT_ENTITY_HPP_INCLUDED

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
