//
// Created by TD on 2021/12/27.
//

#include "image_to_move.h"
#include <doodle_lib/thread_pool/long_term.h>
#include <doodle_lib/metadata/episodes.h>
#include <doodle_lib/metadata/shot.h>
#include <doodle_lib/core/core_set.h>
#include <doodle_lib/thread_pool/thread_pool.h>

#include <opencv2/opencv.hpp>
namespace doodle {
namespace details {
namespace {
void watermark_add_image(cv::Mat &in_image, const image_watermark &in_watermark) {
  auto l_image     = in_image;
  int fontFace     = cv::HersheyFonts::FONT_HERSHEY_COMPLEX;
  double fontScale = 1;
  int thickness    = 2;
  int baseline     = 0;
  auto textSize    = cv::getTextSize(in_watermark.p_text, fontFace,
                                     fontScale, thickness, &baseline);
  baseline += thickness;
  textSize.width += baseline;
  textSize.height += baseline;
  // center the text
  cv::Point textOrg((in_image.cols - textSize.width) * in_watermark.p_width_proportion,
                    (in_image.rows + textSize.height) * in_watermark.p_height_proportion);

  // draw the box
  cv::rectangle(l_image, textOrg + cv::Point(0, baseline),
                textOrg + cv::Point(textSize.width, -textSize.height),
                cv::Scalar(0, 0, 0), -1);

  cv::addWeighted(l_image, 0.7, in_image, 0.3, 0, in_image);
  // then put the text itself
  cv::putText(in_image, in_watermark.p_text, textOrg, fontFace, fontScale,
              in_watermark.rgba, thickness, cv::LineTypes::LINE_AA);
}
}  // namespace

class image_to_move::impl {
 public:
  impl() = default;
  std::vector<image_file_attribute> p_image;
  FSys::path p_out_path;
  entt::handle p_h;
  std::future<void> result;

  std::atomic_bool stop{false};
};

image_to_move::image_to_move(const entt::handle &in_handle, const std::vector<entt::handle> &in_vector)
    : p_i(std::make_unique<impl>()) {
  chick_true<doodle_error>(in_handle.any_of<process_message>(), DOODLE_LOC, "缺失进度指示结构");
  chick_true<doodle_error>(in_handle.any_of<FSys::path>(), DOODLE_LOC, "缺失输出文件路径");

  p_i->p_out_path = in_handle.get<FSys::path>();
  //  p_i->p_out_path = in_handle.get_or_emplace<FSys::path>(core_set::getSet().get_cache_root("image"));

  std::for_each(in_vector.begin(), in_vector.end(), [](const entt::handle &in) {
    chick_true<doodle_error>(in.any_of<image_file_attribute>(), DOODLE_LOC, "缺失文件属性");
  });

  p_i->p_h = in_handle;

  std::transform(in_vector.begin(), in_vector.end(), std::back_inserter(p_i->p_image),
                 [](const entt::handle &in) -> image_file_attribute {
                   return in.get<image_file_attribute>();
                 });
  std::for_each(p_i->p_image.begin(), p_i->p_image.end(), [](const image_file_attribute &in) {
    chick_true<doodle_error>(exists(in.file_path), DOODLE_LOC, "找不到路径指向的文件");
  });
  chick_true<doodle_error>(!p_i->p_image.empty(), DOODLE_LOC, "没有传入任何的图片");

  /// \brief 这里排序组件
  std::sort(p_i->p_image.begin(), p_i->p_image.end());

  DOODLE_LOG_INFO("获得图片路径 {}", p_i->p_image.front().file_path.parent_path());
}
image_to_move::image_to_move(const entt::handle &in_handle,
                             const std::vector<image_file_attribute> &in_vector)
    : p_i(std::make_unique<impl>()){
  chick_true<doodle_error>(in_handle.any_of<process_message>(), DOODLE_LOC, "缺失进度指示结构");
  chick_true<doodle_error>(in_handle.any_of<FSys::path>(), DOODLE_LOC, "缺失输出文件路径");

  p_i->p_out_path = in_handle.get<FSys::path>();
  //  p_i->p_out_path = in_handle.get_or_emplace<FSys::path>(core_set::getSet().get_cache_root("image"));

  //  std::for_each(in_vector.begin(), in_vector.end(), [](const image_file_attribute &in) {
  //    chick_true<doodle_error>(exists(in.file_path), DOODLE_LOC, "找不到路径指向的文件");
  //  });
  for (auto &k_i : in_vector) {
    chick_true<doodle_error>(exists(k_i.file_path), DOODLE_LOC, "找不到路径指向的文件");
  }
  p_i->p_image = in_vector;
  p_i->p_h     = in_handle;

  chick_true<doodle_error>(!p_i->p_image.empty(), DOODLE_LOC, "没有传入任何的图片");

  /// \brief 这里排序组件
  std::sort(p_i->p_image.begin(), p_i->p_image.end());

  DOODLE_LOG_INFO("获得图片路径 {}", p_i->p_image.front().file_path.parent_path());
}
image_to_move::~image_to_move() = default;

void image_to_move::init() {
  auto &l_mag = p_i->p_h.patch<process_message>();
  l_mag.set_state(l_mag.run);
  l_mag.aborted_function = [self = this]() {if(self) self->abort(); };

  /// \brief 这里我们检查 shot，episode 进行路径的组合
  if (!p_i->p_out_path.has_extension())
    p_i->p_out_path /= fmt::format(
        "{}_{}.mp4",
        p_i->p_h.any_of<episodes>() ? fmt::to_string(p_i->p_h.get<episodes>()) : "eps_none"s,
        p_i->p_h.any_of<shot>() ? fmt::to_string(p_i->p_h.get<shot>()) : "sh_none"s);
  else
    chick_true<doodle_error>(p_i->p_out_path.extension() == ".mp4", DOODLE_LOC, "扩展名称不是MP4");

  if (exists(p_i->p_out_path.parent_path()))
    create_directories(p_i->p_out_path.parent_path());

  DOODLE_LOG_INFO("开始创建视频 {}", p_i->p_out_path);

  auto k_fun = [&]() -> void {
    const static cv::Size k_size{1920, 1080};
    auto video             = cv::VideoWriter{p_i->p_out_path.generic_string(),
                                 cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                                 25,
                                 k_size};
    auto k_image           = cv::Mat{};
    const auto &k_size_len = p_i->p_image.size();
    for (auto &l_image : p_i->p_image) {
      if (p_i->stop)
        return;
      k_image = cv::imread(l_image.file_path.generic_string());
      chick_true<doodle_error>(!k_image.empty(), DOODLE_LOC, "open cv not read image");
      if (k_image.cols != k_size.width || k_image.rows != k_size.height)
        cv::resize(k_image, k_image, k_size);

      for (auto &k_w : l_image.watermarks) {
        watermark_add_image(k_image, k_w);
      }
      p_i->p_h.patch<process_message>([&](process_message &in_message) {
        in_message.progress_step(rational_int{1, k_size_len});
      });
      video << k_image;
    }
  };
  p_i->result = std::move(g_thread_pool().enqueue(k_fun));
}
void image_to_move::update(chrono::duration<chrono::system_clock::rep, chrono::system_clock::period>, void *data) {
  chick_true<doodle_error>(p_i->result.valid(), DOODLE_LOC, "无效的数据");
  switch (p_i->result.wait_for(0ns)) {
    case std::future_status::ready: {
      try {
        p_i->result.get();
        this->succeed();
      } catch (const doodle_error &error) {
        DOODLE_LOG_ERROR(error.what());
        this->fail();
        throw;
      }
    } break;
    default:
      break;
  }
}
void image_to_move::succeeded() {
  p_i->p_h.patch<process_message>([&](process_message &in) {
    in.set_state(in.success);
    auto k_str = fmt::format("成功完成任务\n");
    in.message(k_str, in.warning);
  });
}
void image_to_move::failed() {
  p_i->p_h.patch<process_message>([&](process_message &in) {
    in.set_state(in.fail);
    auto k_str = fmt::format("转换失败 \n");
    in.message(k_str, in.warning);
  });
}
void image_to_move::aborted() {
  p_i->stop = true;
  p_i->p_h.patch<process_message>([&](process_message &in) {
    in.set_state(in.fail);
    auto k_str = fmt::format("合成视频被主动结束 合成视频文件将被主动删除\n");
    in.message(k_str, in.warning);
  });
  try {
    remove(p_i->p_out_path);
  } catch (const FSys::filesystem_error &err) {
    p_i->p_h.patch<process_message>([&](process_message &in) {
      auto k_str = fmt::format("合成视频主动删除失败 {}\n", err.what());
      in.message(k_str, in.warning);
    });
    throw;
  }
}

}  // namespace details
image_watermark::image_watermark(const string &in_p_text,
                                 double_t in_p_width_proportion,
                                 double_t in_p_height_proportion,
                                 const cv::Scalar &in_rgba)
    : p_text(in_p_text),
      p_width_proportion(in_p_width_proportion),
      p_height_proportion(in_p_height_proportion),
      rgba(in_rgba) {}
bool image_file_attribute::operator<(const image_file_attribute &in_rhs) const {
  return file_path < in_rhs.file_path;
}
bool image_file_attribute::operator>(const image_file_attribute &in_rhs) const {
  return in_rhs < *this;
}
bool image_file_attribute::operator<=(const image_file_attribute &in_rhs) const {
  return !(in_rhs < *this);
}
bool image_file_attribute::operator>=(const image_file_attribute &in_rhs) const {
  return !(*this < in_rhs);
}
}  // namespace doodle
