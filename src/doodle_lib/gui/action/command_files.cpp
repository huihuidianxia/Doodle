//
// Created by TD on 2021/9/18.
//

#include "command_files.h"

#include <doodle_lib/core/open_file_dialog.h>
#include <doodle_lib/metadata/metadata_cpp.h>
namespace doodle {
comm_files_up::comm_files_up()
    : p_root(),
      p_parent() {
  p_name     = "添加文件";
  p_show_str = make_imgui_name(this,
                               "添加文件",
                               "添加",
                               "替换");
}

bool comm_files_up::render() {
  if (p_root) {
    if (imgui::Button(p_show_str["添加文件"].c_str()))
      open_file_dialog{
          "open_get_files",
          "获得文件",
          ".*",
          ".",
          "",
          1}
          .show(
              [this](const std::vector<FSys::path>& in_p) {
                p_files = in_p;
              });
    if (imgui::Button(p_show_str["添加"].c_str())) {
      auto k_f=  p_root->get_path_file();
      ;
    }
    if (imgui::Button(p_show_str["替换"].c_str())) {
    }
  }
  return true;
}

bool comm_files_up::add_data(const metadata_ptr& in_parent, const metadata_ptr& in) {
  if (in_parent) {
    p_parent = in_parent;
  }
  if (details::is_class<assets_file>(in))
    p_root = std::dynamic_pointer_cast<assets_file>(in);
  return true;
}
}  // namespace doodle