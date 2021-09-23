#pragma once
#include <DoodleLib/DoodleLib_fwd.h>
#include <DoodleLib/threadPool/long_term.h>

namespace doodle {
/**
 * @brief 连接视频的opencv的包装类
 *
 */
class DOODLELIB_API VideoSequence
    : public std::enable_shared_from_this<VideoSequence> {
 private:
  std::vector<FSys::path> p_paths;

  std::string p_name;

 public:
  VideoSequence(std::vector<FSys::path> paths);

  void connectVideo(const FSys::path& path, const long_term_ptr& in_ptr) const;
  /**
   * @brief 使用这个可以将镜头和和集数以及输出名称一起设置完成
   *
   * @param in_shot 要使用的镜头元数据
   * @param in_episodes 要使用的集数元数据
   * @return std::string 生成的水印
   */
  std::string set_shot_and_eps(const ShotPtr& in_shot, const EpisodesPtr& in_episodes);
};

class DOODLELIB_API video_sequence_async : public details::no_copy {
  std::shared_ptr<VideoSequence> p_video;
  FSys::path p_backup_out_path;

 public:
  video_sequence_async();
  std::shared_ptr<VideoSequence> set_video_list(const std::vector<FSys::path>& paths);
  long_term_ptr connect_video(const FSys::path& path = {}) const;
};

}  // namespace doodle
