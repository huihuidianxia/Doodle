#pragma once

#include <doodle_lib/doodle_lib_fwd.h>

namespace doodle::gui {

class DOODLELIB_API base_render {
 public:
  base_render() = default;
  virtual bool operator()(const entt::handle &in_handle){};
};

template <class T>
base_render make_render() {
}

class base_edit {
 protected:
  virtual void init_(const entt::handle &in)       = 0;
  virtual void save_(const entt::handle &in) const = 0;

 public:
  bool is_modify{false};
  void init(const entt::handle &in);
  virtual void render(const entt::handle &in) = 0;
  void save(const entt::handle &in);
};

namespace details {
/// , std::enable_if_t<!doodle::details::is_smart_pointer<T>::value, bool> = true

class gui_cache_null_data {
 public:
};

class gui_cache_select {
 public:
  bool select;
};

template <class T, class BaseType = gui_cache_null_data>
class gui_cache : public BaseType {
 public:
  using base_type = BaseType;
  std::string name_id;
  std::string_view name;
  T data;
  template <class IN_T, std::enable_if_t<!doodle::details::is_smart_pointer<IN_T>::value, bool> = true>
  explicit gui_cache(const std::string &in_name, const IN_T &in_data)
      : base_type(),
        name_id(fmt::format("{}##{}", in_name, fmt::ptr(this))),
        data(in_data) {
    std::string_view k_v{name_id};
    name = k_v.substr(0, in_name.size());
  };

  template <class IN_T, std::enable_if_t<doodle::details::is_smart_pointer<IN_T>::value, bool> = true>
  explicit gui_cache(const std::string &in_name, IN_T &in_data)
      : base_type(),
        name_id(fmt::format("{}##{}", in_name, fmt::ptr(this))),
        data(std::move(in_data)) {
    std::string_view k_v{name_id};
    name = k_v.substr(0, in_name.size());
  };

  template <class IN_T, std::enable_if_t<doodle::details::is_smart_pointer<IN_T>::value, bool> = true>
  explicit gui_cache(IN_T &in_data)
      : gui_cache(fmt::to_string(*in_data), in_data) {}

  template <class IN_T, std::enable_if_t<!doodle::details::is_smart_pointer<IN_T>::value, bool> = true>
  explicit gui_cache(const IN_T &in_data)
      : gui_cache(fmt::to_string(in_data), in_data) {}

  bool operator<(const gui_cache &in_rhs) const {
    return data < in_rhs.data;
  }
  bool operator>(const gui_cache &in_rhs) const {
    return in_rhs < *this;
  }
  bool operator<=(const gui_cache &in_rhs) const {
    return !(in_rhs < *this);
  }
  bool operator>=(const gui_cache &in_rhs) const {
    return !(*this < in_rhs);
  }
  bool operator==(const gui_cache &in_rhs) const {
    return data == in_rhs.data;
  }
  bool operator!=(const gui_cache &in_rhs) const {
    return !(in_rhs == *this);
  }

  constexpr operator T &() {
    return data;
  }
  constexpr operator const T &() const {
    return data;
  }
};
}  // namespace details

}  // namespace doodle::gui
