#include "mypch.h"
#include "core/texture.h"

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
}


YAML::Emitter& operator<<(YAML::Emitter& out, const TextureInfo& t) {
    out << t.name;
    return out;
}

void ToLower(std::string& stringToChange) {
    std::transform(stringToChange.begin(), stringToChange.end(), stringToChange.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
}
