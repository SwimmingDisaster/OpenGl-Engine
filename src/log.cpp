#include "log.h"


std::ostream& operator<<(std::ostream& out, const glm::vec3& g)
{
	return out << glm::to_string(g);
}
std::ostream& operator<<(std::ostream& out, const glm::vec4& g)
{
	return out << glm::to_string(g);
}
std::ostream& operator<<(std::ostream& out, const glm::mat4& g)
{
	return out << glm::to_string(g);
}
