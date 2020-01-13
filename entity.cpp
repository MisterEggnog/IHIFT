
#include "entity.hpp"

namespace MisterEggnog::IHIFT {

int new_id() noexcept
{
	static int id_counter = 0;
	return id_counter++;
}

}
