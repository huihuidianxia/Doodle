//
// Created by teXiao on 2021/4/27.
//

#pragma once

#include <doodle_lib/doodle_lib_fwd.h>

#include <boost/signals2.hpp>
#include <optional>

namespace doodle {
class DOODLELIB_API metadata_database {
 public:
  std::string user_data;
  std::uint64_t id;
  std::optional<std::uint32_t> parent;
  std::int32_t m_type;
  std::string uuid_path;
  std::optional<std::int32_t> season;
  std::optional<std::int32_t> episode;
  std::optional<std::int32_t> shot;
  std::optional<std::string> assets;
};
enum class metadata_type : std::int32_t {
  unknown_file       = 0,
  project_root       = 1,
  file               = 2,
  folder             = 3,
  derive_file        = 4,
  animation_lib_root = 5,
  maya_file          = 6,
  maya_rig           = 7,
  fbx                = 8,
  abc                = 9,
  movie              = 10,
  ue4_prj            = 11

};

class DOODLELIB_API root_ref {
 public:
  entt::entity p_root;

  inline entt::handle root_handle() {
    return make_handle(p_root);
  }

  inline void set_root(entt::entity in_root) {
    p_root = in_root;
  }
};

class DOODLELIB_API database_info {
 public:
  FSys::path path_;
 

};

class DOODLELIB_API database {
 private:
  class impl;
  std::unique_ptr<impl> p_i;

 public:
  class DOODLELIB_API ref_root {
   public:
    const std::uint64_t id;
    const boost::uuids::uuid uuid;
  };

  database();
  //  explicit database(const metadata_database &in_metadata_database);
  ~database();

  bool has_components() const;
  void set_id(std::uint64_t in_id) const;

  database(database &&) noexcept;
  database &operator            =(database &&) noexcept;

  database(database &) noexcept = delete;
  database &operator=(database &) noexcept = delete;

  static void set_enum(entt::registry &in_reg, entt::entity in_ent);

  const FSys::path &get_url_uuid() const;
  std::int32_t get_meta_type_int() const;
  bool is_install() const;
  const string &get_id_str() const;
  ref_root get_ref() const;

  const boost::uuids::uuid &uuid() const;
  /**
   * @brief 获得数据库id
   *
   * @return std::int32_t
   */
  std::uint64_t get_id() const;
  /**
   * @brief 设置数据库中的类型
   *
   * @param in_meta 类型
   */
  void set_meta_type(const metadata_type &in_meta);
  void set_meta_type(const std::string &in_meta);
  void set_meta_type(std::int32_t in_);

  database &operator=(const metadata_database &in);
  operator metadata_database() const;
  bool operator==(const database &in_rhs) const;
  bool operator==(const boost::uuids::uuid &in_rhs) const;
  bool operator!=(const database &in_rhs) const;
  bool operator!=(const boost::uuids::uuid &in_rhs) const;

  enum class status : std::uint8_t {
    none        = 0,
    is_sync     = 1,
    need_load   = 2,
    need_save   = 3,
    need_delete = 4,
  };

  std::atomic<status> status_;

  friend void to_json(nlohmann::json &j, const database &p);
  friend void from_json(const nlohmann::json &j, database &p);

  class DOODLELIB_API fun_save_ {
   public:
    void operator()(database &in) const {
      in.status_ = status::need_save;
    }
  };
  class DOODLELIB_API fun_delete_ {
   public:
    void operator()(database &in) const {
      in.status_ = status::need_delete;
    }
  };
  class DOODLELIB_API fun_load_ {
   public:
    void operator()(database &in) const {
      in.status_ = status::need_load;
    }
  };
  class DOODLELIB_API fun_sync_ {
   public:
    void operator()(database &in) const {
      in.status_ = status::is_sync;
    }
  };

  constexpr const static fun_save_ save{};
  constexpr const static fun_delete_ delete_{};
  constexpr const static fun_load_ load{};
  constexpr const static fun_sync_ sync{};
};

void from_json(const nlohmann::json &j, database &p);
void to_json(nlohmann::json &j, const database &p);

class DOODLELIB_API to_str {
 private:
  mutable std::string p_str;

 public:
  to_str() = default;
  DOODLE_MOVE(to_str);

  const string &get() const;
  operator string() const;
};

template <class in_class>
class DOODLELIB_API handle_warp {
 public:
  entt::handle handle_;
};

}  // namespace doodle

// CEREAL_REGISTER_TYPE(doodle::metadata)
// CEREAL_REGISTER_POLYMORPHIC_RELATION(std::enable_shared_from_this<doodle::metadata>, doodle::metadata)
