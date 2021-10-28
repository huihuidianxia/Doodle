//
// Created by TD on 2021/10/25.
//

#pragma once

#include <doodle_lib/doodle_lib_fwd.h>
#include <doodle_lib/gui/action/command.h>

namespace doodle {
class DOODLELIB_API comm_video : public command_base {
 private:
  entt::entity p_root;

 public:
  comm_video();
  virtual bool render() override;
  virtual bool set_data(const entt::handle& in_any) override;
};
}  // namespace doodle
