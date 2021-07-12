#pragma once

#include <DoodleLib/DoodleLib_fwd.h>

#include <boost/signals2.hpp>

namespace doodle {
/**
 * @brief 长时间任务时， 使用这个类进行通知；
 * 
 */
class long_term {
 public:
  long_term() : sig_progress(),
                sig_message_result(),
                sig_finished(){};
  virtual ~long_term() = default;
  /**
   * @brief 这个是步进信号, 到一百是完成
   * 
   */
  boost::signals2::signal<void(int)> sig_progress;
  /**
   * @brief 结果信号
   * 
   */

  boost::signals2::signal<void(const std::string& message)> sig_message_result;
  /**
   * @brief 完成信号
   * 
   */
  boost::signals2::signal<void()> sig_finished;
};

}  // namespace doodle
