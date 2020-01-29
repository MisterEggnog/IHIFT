
#include "utils.hpp"
#include <memory>
#include <unordered_map>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Rect.hpp>

#define TEXTURE_PIXELS 16
#define PADDING 1

static int tie_shorts(short x, short y)
{
	union {
		short partial[2];
		int full;
	};
	partial[0] = x;
	partial[1] = y;
	return full;
}

static sf::Image static_image_loader(const char* file_name)
{
	sf::Image texture_map;
	if (!texture_map.loadFromFile(file_name)) {
		std::fprintf(stderr, "Cannot find texture file: %s\n", file_name);
		std::exit(EXIT_FAILURE);
	}
	return texture_map;
}

static int image_upper_position(int texture_number)
{
	return (TEXTURE_PIXELS+PADDING)*texture_number;
}

const std::shared_ptr<sf::Texture> MisterEggnog::IHIFT::get_tile_texture(short x, short y)
{
	static sf::Image texture_map = static_image_loader("tilesheet.png");
	static std::unordered_map<int, std::shared_ptr<sf::Texture>> rendered_textures;
	try {
		return rendered_textures.at(tie_shorts(x,y));
	}
	catch(...) {}
	int max_texture_side = texture_map.getSize().x / (TEXTURE_PIXELS + PADDING);
	if (x > max_texture_side || y > max_texture_side) {
		std::printf("x,y out of range.\n"
					"Need 0<=n<%i\n"
					"Received %i,%i.\n", max_texture_side, x, y);
		std::exit(EXIT_FAILURE);
	}
	sf::Rect texture_area(image_upper_position(x), image_upper_position(y), TEXTURE_PIXELS, TEXTURE_PIXELS);
	auto texture_ptr = std::make_shared<sf::Texture>();
	texture_ptr->loadFromImage(texture_map, texture_area);
	rendered_textures[tie_shorts(x,y)] = texture_ptr;
	return texture_ptr;
}

/*
if rendered_textures contains x,y
	return texture at x,y
max_texture_side = image_length / (16+1)
if x or y > max_texture_side
	print error
	exit
rect(trx, try, 16, 16)
tex->load image, rect
instert tex
return tex

*/

#include <cmath>

sf::Vector2f MisterEggnog::IHIFT::unit_vector(sf::Vector2f vc)
{
	return vc * (1 / std::hypot(vc.x, vc.y));
}

#include <SFML/Window/Keyboard.hpp>

namespace MisterEggnog::IHIFT {

bool register_left()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
}

bool register_right()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
}

bool register_up()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
}

bool register_down()
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
}

} // namespace MisterEggnog::IHIFT
