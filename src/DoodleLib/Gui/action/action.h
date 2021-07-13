﻿//
// Created by TD on 2021/6/17.
//

#pragma once

#include <DoodleLib/DoodleLib_fwd.h>

#include <any>
#include <boost/signals2.hpp>

namespace doodle {

/**
 * @brief 动作，或者说是命令， 是围绕着数据的包装， 我们最终是需要运行的
 * 在创建动作时，需要喂入所需要的数据， 并在之后调用他
 * 或者我们喂入一些函子也是可以的
 */

class DOODLELIB_API action : public details::no_copy {
  friend DragFilesFactory;

 protected:
  std::string p_name;

 public:
  /**
   * @brief 这是一个动作参数对象， 使用时根据动作来确定参数
   * 
   */
  class DOODLELIB_API _arg {
   public:
    _arg()          = default;
    virtual ~_arg() = default;

    bool is_cancel;

    // _arg(_arg&& other) = default;
    // _arg && operator=(_arg&& other) = default;
  };
  class DOODLELIB_API arg_null : public _arg {
   public:
    arg_null() = default;
  };
  class DOODLELIB_API arg_int : public _arg {
   public:
    arg_int() = default;
    explicit arg_int(std::int32_t in_) : date(in_){};
    std::int32_t date;
  };

  class DOODLELIB_API arg_str : public _arg {
   public:
    arg_str() = default;
    explicit arg_str(std::string in_) : date(std::move(in_)){};
    std::string date;
  };

  class DOODLELIB_API arg_path : public _arg {
   public:
    arg_path() = default;
    explicit arg_path(FSys::path& in_) : date(in_){};
    FSys::path date;
  };

  class DOODLELIB_API arg_paths : public _arg {
   public:
    arg_paths() = default;
    explicit arg_paths(std::vector<FSys::path>& in_) : date(in_){};
    std::vector<FSys::path> date;
  };

  action();

  /**
   * @brief 这个函数基本上可以作为显示为菜单的函数
   * @return 返回动作名称
   */
  virtual std::string class_name();

  virtual long_term_ptr run(const MetadataPtr& in_data, const MetadataPtr& in_parent) = 0;
  virtual long_term_ptr operator()(const MetadataPtr& in_data, const MetadataPtr& in_parent);
};

template <class arg_type>
class DOODLELIB_API action_indirect : public action {
 public:
  action_indirect() = default;

  virtual bool is_accept(const arg_type& in_any) { return false; };
  boost::signals2::signal<arg_type()> sig_get_arg;
};

// template <class arg_type>
class DOODLELIB_API action_composited final : public action_indirect<action::arg_null> {
 protected:
 public:
  action_composited() = default;
  std::vector<action_ptr> p_action_list;

  virtual void set_class_name(const std::string& in_name);
  long_term_ptr run(const MetadataPtr& in_data, const MetadataPtr& in_parent) override;
};

class DOODLELIB_API actn_null : public action_indirect<action::arg_null> {
 public:
  actn_null();

  inline long_term_ptr run(const MetadataPtr& in_data, const MetadataPtr& in_parent) override{ return {};};
};
}  // namespace doodle
