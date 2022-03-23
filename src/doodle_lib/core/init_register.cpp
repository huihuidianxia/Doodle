#include "init_register.h"
#include <core/core_sig.h>
#include <long_task/process_pool.h>
namespace doodle {

std::multimap<std::int32_t, std::function<void()>>& init_register::registered_functions() {
  return init_p;
}
void init_register::reg_class() {
  auto l_then = g_main_loop().attach<one_process_t>([]() {
    DOODLE_LOG_INFO("开始反射注册");
  });
  auto& l_map = registered_functions();
  for (auto it = l_map.begin(), end = l_map.end();
       it != end;
       it = l_map.upper_bound(it->first)) {
    DOODLE_LOG_INFO("初始化优先级 {}", it->first);
    l_then = l_then.then<one_process_t>([key = it->first, this]() {
      auto l_p = registered_functions().equal_range(key);
      std::for_each(l_p.first, l_p.second,
                    [](const std::multimap<std::int32_t, std::function<void()>>::value_type& i) {
                      i.second();
                    });
    });
  }
  l_then.then<one_process_t>([&]() {
    DOODLE_LOG_INFO("结束开始反射注册");
    g_reg()->ctx<core_sig>().init_end();
  });
}
init_register& init_register::instance() noexcept {
  static init_register l_r{};
  return l_r;
}
void init_register::init_run() {
  for (auto&& ref_ : entt::resolve()) {
    bool is_ch{false};
    for (auto&& l_base : ref_.base()) {
      if (l_base == entt::resolve<base_registrar>()) {
        is_ch = true;
        break;
      }
    }
    if (is_ch) {
      auto l_i = ref_.construct();
      l_i.cast<base_registrar&>().init();
    }
  }
}
init_register::init_register()  = default;
init_register::~init_register() = default;
namespace {
constexpr auto meta_init_registrar_lab = []() {
  entt::meta<init_register::base_registrar>()
      .type()
      .func<&init_register::base_registrar::init>("init"_hs);
};
class meta_init_registrar
    : public init_register::registrar_lambda<meta_init_registrar_lab, 1> {};
}  // namespace
}  // namespace doodle
