#include "model/Work.hpp"
#include <nlohmann/json.hpp>
#include <string>

void to_json(nlohmann::json &j, const Work &work) {
    j = nlohmann::json{{"value", work.value}};
}

std::string Work::serialize() const {
    nlohmann::json j = *this;
    return j.dump();
}
