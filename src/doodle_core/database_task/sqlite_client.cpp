//
// Created by TD on 2022/6/2.
//

#include "sqlite_client.h"

#include <database_task/insert.h>
#include <database_task/select.h>
#include <database_task/update.h>
#include <database_task/delete_data.h>

#include <thread_pool/asio_pool.h>
#include <thread_pool/process_pool.h>
#include <doodle_core/core/core_sig.h>
#include <metadata/metadata.h>

#include <range/v3/all.hpp>
#include <core/status_info.h>

namespace doodle::database_n {

void sqlite_client::open_sqlite(const FSys::path& in_path, bool only_ctx) {
  g_reg()->ctx().at<core_sig>().project_begin_open(in_path);
  g_pool()
      .post<database_n::select>(database_n::select::arg{in_path, only_ctx})
      .then<one_process_t>([]() {
        g_reg()->ctx().at<core_sig>().project_end_open();
      });
}

void sqlite_client::update_entt() {
  std::vector<entt::entity> delete_list;
  std::vector<entt::entity> all_list;
  std::vector<entt::entity> install_list;
  std::vector<entt::entity> update_list;
  std::vector<entt::entity> next_delete_list;

  auto l_dv = g_reg()->view<data_status_delete, database>();
  for (auto&& [e, d] : l_dv.each()) {
    if (d.is_install()) {
      delete_list.push_back(e);
    } else {
      next_delete_list.push_back(e);
    }
  }

  auto l_sv = g_reg()->view<data_status_save, database>();
  for (auto&& [e, d] : l_sv.each()) {
    if (d.is_install()) {
      update_list.push_back(e);
    } else {
      install_list.push_back(e);
    }
  }
  all_list |= ranges::actions::push_back(delete_list) |
              ranges::actions::push_back(install_list) |
              ranges::actions::push_back(update_list) |
              ranges::actions::push_back(next_delete_list) |
              ranges::actions::unique;

  if (all_list.empty()) {
    /// \brief 只更新上下文
    g_pool().post<doodle::one_process_t>([=]() {
              g_reg()->ctx().at<core_sig>().save_begin({});
            })
        .then<database_n::update_data>(update_list)
        .then<one_process_t>([]() {
          g_reg()->ctx().at<core_sig>().save_end({});
        });
    return;
  }

  auto l_list = all_list | ranges::view::transform([](auto e) {
                  return make_handle(e);
                }) |
                ranges::to_vector;
  auto l_next = g_pool().post<doodle::one_process_t>([=]() {
    g_reg()->ctx().at<core_sig>().save_begin(l_list);
    /// \brief 删除没有插入的
    g_reg()->destroy(next_delete_list.begin(), next_delete_list.end());
  });

  if (!install_list.empty()) {
    l_next.then<database_n::insert>(install_list);
  }
  if (!update_list.empty()) {
    l_next.then<database_n::update_data>(update_list);
  }
  if (!delete_list.empty()) {
    l_next.then<database_n::delete_data>(delete_list);
  }
  l_next.then<one_process_t>([=]() {
    auto l_sv = g_reg()->view<data_status_save>();
    g_reg()->remove<data_status_save>(l_sv.begin(), l_sv.end());
    auto l_dv = g_reg()->view<data_status_delete>();
    g_reg()->remove<data_status_save>(l_dv.begin(), l_dv.end());
    g_reg()->ctx().at<core_sig>().save_end(l_list);

    g_reg()->ctx().at<status_info>().need_save = false;
  });
}
void sqlite_client::create_sqlite() {
  g_pool()
      .post<database_n::insert>(std::vector<entt::entity>{})
      .then<database_n::update_data>(std::vector<entt::entity>{});
}
}  // namespace doodle::database_n
