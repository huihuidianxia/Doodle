//
// Created by TD on 2021/9/16.
//

#include "assets_file_widgets.h"

#include <doodle_lib/gui/action/command.h>
#include <doodle_lib/gui/action/command_files.h>
#include <doodle_lib/gui/action/command_meta.h>
#include <doodle_lib/gui/factory/attribute_factory_interface.h>
#include <doodle_lib/lib_warp/imgui_warp.h>
#include <doodle_lib/metadata/metadata_cpp.h>
#include <doodle_lib/core/image_loader.h>
#include <doodle_lib/metadata/image_icon.h>
#include <doodle_lib/long_task/process_pool.h>
#include <doodle_lib/gui/widgets/screenshot_widget.h>
#include <entt/entt.hpp>
#include <doodle_lib/long_task/drop_file_data.h>

namespace doodle {
namespace details {
void table_column::render(const entt::handle& in_ptr) {
  imgui::TableNextColumn();
  frame_render(in_ptr);
}
void table_column::set_select(const entt::handle& in_) {
  table->p_current_select = in_;

  table->select_change(in_);
}

bool column_id::frame_render(const entt::handle& in_ptr) {
  if (dear::Selectable(in_ptr.get<database>().get_id_str(),
                       in_ptr == table->p_current_select,
                       ImGuiSelectableFlags_SpanAllColumns)) {
    set_select(in_ptr);
  }
  return true;
}
bool column_version::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<assets_file>())
    dear::Text(in_ptr.get<assets_file>().get_version_str());
  return true;
}
bool column_comment::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<comment_vector>()) {
    auto& k_com = in_ptr.get<comment_vector>();
    dear::Text(k_com.get().empty() ? std::string{} : k_com.get().front().get_comment());
  } else {
    dear::Text(std::string{});
  }
  return true;
}
bool column_path::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<assets_path_vector>()) {
    auto& k_path = in_ptr.get<assets_path_vector>();
    // string k_all_str{};
    string k_line_str{fmt::format("{}", k_path)};

    dear::Text(k_line_str.c_str());
    // if (!k_all_str.empty()) {
    //   imgui::SameLine();
    //   dear::HelpMarker{"(...)", k_all_str.c_str()};
    // }
  }
  return true;
}
bool column_time::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<time_point_wrap>())
    dear::Text(in_ptr.get<time_point_wrap>().show_str());
  return true;
}
bool column_user::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<assets_file>())
    dear::Text(in_ptr.get<assets_file>().get_user());
  return true;
}
bool column_assets::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<assets>())
    dear::Text(in_ptr.get<assets>().str());

  return true;
}
bool column_season::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<season>())
    dear::Text(in_ptr.get<season>().str());
  return true;
}
bool column_episodes::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<episodes>())
    dear::Text(in_ptr.get<episodes>().str());
  return true;
}
bool column_shot::frame_render(const entt::handle& in_ptr) {
  if (in_ptr.all_of<shot>())
    dear::Text(in_ptr.get<shot>().str());
  return true;
}

void icon_render::render(const entt::handle& in) {
}

}  // namespace details

void assets_file_widgets::set_select(const entt::handle& in) {
}

assets_file_widgets::assets_file_widgets()
    : p_current_select() {
}
void assets_file_widgets::init() {
  g_reg()->set<assets_file_widgets&>(*this);
}
void assets_file_widgets::succeeded() {
}
void assets_file_widgets::failed() {
}
void assets_file_widgets::aborted() {
}
void assets_file_widgets::update(chrono::duration<chrono::system_clock::rep, chrono::system_clock::period>, void* data) {
  auto k_ = g_reg()->try_ctx<handle_list>();

  if (k_) {
    auto& k_list = *k_;
    const static auto l_size{5u};
    ImGui::Columns(l_size, "assets_file_widgets", false);

    image_loader k_load{};

    ImGuiListClipper clipper{};
    clipper.Begin((boost::numeric_cast<std::int32_t>(k_list.size()) / l_size) + 1);
    while (clipper.Step()) {
      for (int l_i = clipper.DisplayStart; l_i < clipper.DisplayEnd; ++l_i) {
        for (int l_j = 0; l_j < l_size; ++l_j) {
          if ((l_i * l_size + l_j) < k_list.size()) {
            auto&& i = k_list[l_i * l_size + l_j];
            std::shared_ptr<void> l_image{};
            std::string l_name{};
            if (i.any_of<image_icon>()) {
              /// @brief 如果有图标就渲染
              auto&& k_icon = i.get<image_icon>();
              if (!k_icon.image)
                k_load.load(i);
              l_image = k_icon.image;
            } else {
              l_image = k_load.default_image();
              /// @brief 否则默认图标
            }
            if (i.all_of<assets_file>()) {
              /// @brief 渲染名称
              l_name = i.get<assets_file>().show_str();
            } else {
              /// @brief 否则渲染id
              if (i.all_of<database>())
                l_name = i.get<database>().get_id_str();
            }
            dear::IDScope(magic_enum::enum_integer(i.entity())) && [&]() {
              if (imgui::ImageButton(l_image.get(), {64.f, 64.f}))
                p_current_select = i;
              dear::PopupContextItem{} && [this, i]() {
                render_context_menu(i);
              };
              dear::Text(l_name);
            };
          }
          imgui::NextColumn();
        }
      }
    }
  }
}

void assets_file_widgets::render_context_menu(const entt::handle& in_) {
  if (dear::MenuItem("打开") && in_.all_of<assets_file>()) {
    auto k_path = g_reg()->ctx<project>().get_path() / in_.get<assets_file>().path;
    FSys::open_explorer(FSys::is_directory(k_path) ? k_path : k_path.parent_path());
  }
  if (dear::MenuItem("截图")) {
    g_main_loop()
        .attach<screenshot_widget>(in_)
        .then<one_process_t>([=]() {
          in_.patch<database>(database::save{});
        });
  }
}

assets_file_widgets::~assets_file_widgets() = default;

}  // namespace doodle
