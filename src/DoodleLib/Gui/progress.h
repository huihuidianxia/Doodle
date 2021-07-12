//
// Created by TD on 2021/7/12.
//

#pragma once

#include <DoodleLib/DoodleLib_fwd.h>

#include <nana/gui/widgets/form.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/progress.hpp>

namespace doodle {

class DOODLELIB_API progress : public nana::form {
  nana::progress _pro;

 public:
  progress();
  explicit progress(nana::window in_w, long_term_ptr in_, std::string in_title);
};
}  // namespace doodle
