//
// Created by TD on 2023/1/13.
//

#pragma once

#include "doodle_core/metadata/work_task.h"
#include <doodle_core/database_task/details/tool.h>
#include <doodle_core/doodle_core_fwd.h>
namespace doodle::database_n {

template <>
struct sql_com<doodle::database> : detail::sql_create_table_base<tables::entity> {
  registry_ptr reg_;
  sql_com() = default;
  sql_com(registry_ptr reg) : reg_{std::move(reg)} {}
  void create_table(conn_ptr& in_ptr);
  void insert(conn_ptr& in_ptr, const std::vector<entt::entity>& in_id);
  /**
   *
   * @param in_ptr
   * @param in_handle id与之相对的实体
   */
  void select(conn_ptr& in_ptr, std::map<std::int64_t, entt::entity>& in_handle);
  void destroy(conn_ptr& in_ptr, const std::vector<std::int64_t>& in_handle);
};
}  // namespace doodle::database_n