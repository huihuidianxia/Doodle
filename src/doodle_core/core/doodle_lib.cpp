﻿//
// Created by TD on 2021/6/17.
//

#include "doodle_lib.h"

#include <doodle_core/core/core_set.h>
#include <doodle_core/core/core_sig.h>
#include <doodle_core/core/program_info.h>
#include <doodle_core/database_task/sqlite_client.h>
#include <doodle_core/metadata/metadata_cpp.h>
#include <doodle_core/metadata/rules.h>

#include <boost/locale.hpp>
#include <core/status_info.h>
#include <date/tz.h>
#include <exception/exception.h>
#include <logger/logger.h>
namespace doodle {

class static_value {
 public:
  doodle_lib* p_lib;
  static static_value& get() {
    static static_value install{};
    return install;
  }
};

class doodle_lib::impl {
 public:
  std::shared_ptr<core_set> p_set{};
  std::shared_ptr<boost::asio::io_context> io_context_{};

  logger_ctr_ptr p_log{};
  registry_ptr reg{};
};

doodle_lib::doodle_lib() : ptr(std::make_unique<impl>()) {
  /// @brief 最先初始化的局部静态变量
  static_value::get().p_lib = this;
  /// @brief 初始化其他
  ptr->p_set.reset(new core_set{});
  ptr->io_context_   = std::make_shared<boost::asio::io_context>();
  ptr->p_thread_pool = std::make_shared<thread_pool>(std::thread::hardware_concurrency() * 2);
  ptr->p_log         = std::make_shared<logger_ctrl>();
  ptr->reg           = std::make_shared<entt::registry>();

  boost::locale::generator k_gen{};
  k_gen.categories(boost::locale::all_categories ^ boost::locale::formatting_facet ^ boost::locale::parsing_facet);
  FSys::path::imbue(k_gen("zh_CN.UTF-8"));
  ptr->loop_bounded_pool.timiter_ = core_set::get_set().p_max_thread;
  /// 创建依赖性
  ptr->reg->on_construct<assets_file>().connect<&entt::registry::get_or_emplace<time_point_wrap>>();

  ptr->reg->ctx().emplace<database_info>().path_ = ":memory:"s;
  ptr->reg->ctx().emplace<project>("C:/", "tmp_project");
  ptr->reg->ctx().emplace<project_config::base_config>();
  ptr->reg->ctx().emplace<user::current_user>();
  ptr->reg->ctx().emplace<program_info>();

  auto& k_sig = ptr->reg->ctx().emplace<core_sig>();
  ptr->reg->ctx().emplace<status_info>();
  k_sig.save.connect(2, []() {
    std::make_shared<database_n::sqlite_file>()->async_save(
        g_reg()->ctx().at<::doodle::database_info>().path_,
        [](auto) { DOODLE_LOG_INFO("保存项目 {}", g_reg()->ctx().at<::doodle::database_info>().path_); }
    );
  });
  core_set::get_set().lib_ptr = this;
  g_reg()->ctx().emplace<database_n::file_translator_ptr>(std::make_shared<database_n::sqlite_file>());
}

FSys::path doodle_lib::create_time_database() {
  auto k_local_path = core_set::get_set().get_cache_root("tzdata");
  if (FSys::is_empty(k_local_path)) {
    auto k_path = cmrc::DoodleLibResource::get_filesystem().iterate_directory("resource/tzdata");
    for (const auto& i : k_path) {
      FSys::ofstream k_ofstream{k_local_path / i.filename(), std::ios::out | std::ios::binary};
      DOODLE_LOG_INFO("开始创建数据库 {}", k_local_path / i.filename());

      DOODLE_CHICK(k_ofstream, doodle_error{"无法创建数据库 {}", k_local_path / i.filename()});
      auto k_file = cmrc::DoodleLibResource::get_filesystem().open("resource/tzdata/" + i.filename());
      k_ofstream.write(k_file.begin(), boost::numeric_cast<std::int64_t>(k_file.size()));
    }
  }
  date::set_install(k_local_path.generic_string());
  DOODLE_LOG_INFO("初始化时区数据库: {}", k_local_path.generic_string());
  return k_local_path;
}
doodle_lib& doodle_lib::Get() { return *static_value::get().p_lib; }

registry_ptr& doodle_lib::reg_attr() const { return ptr->reg; }

boost::asio::io_context& doodle_lib::io_context_attr() const { return *ptr->io_context_; }

bool doodle_lib::operator==(const doodle_lib& in_rhs) const { return ptr == in_rhs.ptr; }
core_set& doodle_lib::core_set_attr() const { return *ptr->p_set; }

doodle_lib::~doodle_lib() {
  /// @brief  清理变量
  static_value::get().p_lib = nullptr;
}

registry_ptr& g_reg() { return doodle_lib::Get().reg_attr(); }

boost::asio::io_context& g_io_context() { return doodle_lib::Get().io_context_attr(); }
boost::asio::thread_pool& g_thread() { return doodle_lib::Get().thread_attr(); }

}  // namespace doodle
