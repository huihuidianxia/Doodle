//
// Created by TD on 2021/9/18.
//

#include "command.h"

#include <doodle_lib/doodle_app.h>
#include <doodle_lib/file_warp/maya_file.h>
#include <doodle_lib/lib_warp/imgui_warp.h>

namespace doodle {

command_base::command_base()
    : p_name(),
      p_show_str(),
      p_meta_var(),
      reg(g_reg()) {}

bool command_base::set_parent(const entt::handle& in_ptr) {
  p_meta_var = in_ptr;
  return true;
}

bool command_base_list::render() {
  auto k_ = true;
  for (auto i : p_list) {
    dear::TreeNode{i->class_name().c_str()} && [&]() {
      k_ &= i->render();
    };
  }
  return k_;
}
bool command_base_list::set_data(const entt::handle& in_any) {
  auto k_ = true;
  for (auto i : p_list)
    k_ &= i->set_data(in_any);
  return k_;
}

bool command_base_list::set_parent(const entt::handle& in_ptr) {
  auto k_ = true;
  for (auto i : p_list)
    k_ &= i->set_parent(in_ptr);
  return k_;
}
}  // namespace doodle
