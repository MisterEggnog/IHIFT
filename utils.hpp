
#ifndef MISTEREGGNOG_IHIFT_UTILITY_HPP_INCLUDED
#define MISTEREGGNOG_IHIFT_UTILITY_HPP_INCLUDED

#include <memory>
#include <SFML/System/Vector2.hpp>

// Forward Declarations
namespace sf {
	class Texture;
}

namespace MisterEggnog::IHIFT {

/** How many times a second the game is updated.
 */
constexpr int FRAMES_PER_SECOND = 60;

/** Get textures.
 * Get tile number x,y from source image tiles.png.
 * @warning Program will exit if x,y are out of range of tiles.png.
 * @note If tiles.png is not in the path of the binary, the program will not run.
 *
 * Found in utils.h
 */
const std::shared_ptr<sf::Texture> get_tile_texture(short x, short y);

/** Get unit vector.
 * Returns vector pointing in the direction of vc, with a length of 1.
 * @arg vc: Source of unit vector.
 * @return Unit vector of vc.
 */
sf::Vector2f unit_vector(sf::Vector2f vc);

/** Check left/A key pressed.
 * \return True if user presses '\emoji :arrow_left:' or 'A' key.
 *
 * Found in utils.h
 */
bool register_left();
/** Check right/D key pressed.
 * \return True if user presses '\emoji :arrow_right:' or 'D' key.
 *
 * Found in utils.h
 */
bool register_right();
/** Check up/W key pressed.
 * \return True if user presses '\emoji :arrow_up:' or 'W' key.
 *
 * Found in utils.h
 */
bool register_up();
/** Check up/S key pressed.
 * \return True if user presses '\emoji :arrow_down:' or 'S' key.
 *
 * Found in utils.h
 */
bool register_down();

}

/**
 * \namespace MisterEggnog
 * Namespace for Josiah "MisterEggnog" Baldwin's c++ code.
 * \note May remove later.
 */
/**
 * \namespace MisterEggnog::IHIFT
 * Namespace for the I Hope I Finish This members.
 * Most of the functions that aren't in classes are in utils.h.
 */
#endif // MISTEREGGNOG_IHIFT_UTILITY_HPP_INCLUDED
