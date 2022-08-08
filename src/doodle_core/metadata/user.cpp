﻿//
// Created by TD on 2021/5/7.
//

#include <doodle_core/metadata/user.h>
#include <doodle_core/core/core_set.h>
#include <pin_yin/convert.h>

namespace doodle {

void to_json(nlohmann::json& j, const user& p) {
  j["string_"] = p.p_string_;
}
void from_json(const nlohmann::json& j, user& p) {
  j.at("string_").get_to(p.p_string_);
  p.p_ENUS = convert::Get().toEn(p.p_string_);
}
user::user()
    : p_string_(),
      p_ENUS() {}

user::user(std::string in_string)
    : p_string_(std::move(in_string)) {
  p_ENUS = convert::Get().toEn(p_string_);
}
user::user(std::string in_string, std::string in_ENUS)
    : p_string_(std::move(in_string)),
      p_ENUS(std::move(in_ENUS)) {
}
const std::string& user::get_name() const {
  return p_string_;
}
void user::set_name(const std::string& in_string) {
  p_string_ = in_string;
  p_ENUS    = convert::Get().toEn(p_string_);
}

const std::string& user::get_enus() const {
  return p_ENUS;
}
bool user::operator==(const user& in_rhs) const {
  return p_string_ == in_rhs.p_string_;
}
bool user::operator<(const user& in_rhs) const {
  return p_string_ < in_rhs.p_string_;
}
void user::set_user_ctx(entt::registry& in_reg) {
  auto l_v  = in_reg.view<user>().each();
  auto l_it = ranges::find_if(l_v, [&](auto&& in) -> bool {
    return std::get<user>(in).p_string == core_set::get_user();
  });
  if (l_it != l_v.end()) {

  }
}
void user::has_user_in_ctx(entt::registry& in_reg) {
}
void user::set_user_entt(entt::registry& in_reg) {
}

}  // namespace doodle
