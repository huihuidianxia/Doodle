#include <DoodleLib/mainWidght/tool_windows.h>
//logger是boost库使用者，放到qt上面能好点
#include <DoodleLib/DoodleApp.h>
#include <DoodleLib/Exception/Exception.h>
#include <DoodleLib/FileSys/FileSystem.h>
#include <DoodleLib/FileWarp/ImageSequence.h>
#include <DoodleLib/FileWarp/MayaFile.h>
#include <DoodleLib/FileWarp/Ue4Project.h>
#include <DoodleLib/FileWarp/VideoSequence.h>
#include <DoodleLib/Logger/Logger.h>
#include <DoodleLib/Metadata/Episodes.h>
#include <DoodleLib/Metadata/Project.h>
#include <DoodleLib/Metadata/Shot.h>
#include <DoodleLib/Metadata/View/ShotListWidget.h>
#include <DoodleLib/SettingWidght/SettingWidget.h>
#include <DoodleLib/core/CoreSet.h>
#include <DoodleLib/mainWidght/MklinkWidget.h>
#include <DoodleLib/mainWidght/systemTray.h>
#include <DoodleLib/toolkit/MessageAndProgress.h>



namespace doodle {

tool_windows::tool_windows()
    : wxFrame(nullptr, wxID_ANY, {"doodle"}),
      p_exmaya_id(NewControlId()),
      p_create_image_id(NewControlId()),
      p_create_dir_image_id(NewControlId()),
      p_create_video_id(NewControlId()),
      p_create_ue4File_id(NewControlId()),
      p_mkLink_id(NewControlId()) {
  // SetMenuBar(new wxMenuBar{});
  // CreateStatusBar(1);
  // SetStatusText("doodle tools");
  //创建总布局
  auto layout = new wxBoxSizer{wxVERTICAL};
  //创建按钮
  auto k_exMaya_button    = new wxButton{this, p_exmaya_id, ConvStr<wxString>("从maya导出相机和文件")};
  auto k_create_image     = new wxButton{this, p_create_image_id, ConvStr<wxString>("从图片创建视频")};
  auto k_create_dir_image = new wxButton{this, p_create_dir_image_id, ConvStr<wxString>("从多个文件夹创建视频")};
  auto k_create_video     = new wxButton{this, p_create_video_id, ConvStr<wxString>("连接视频")};
  auto k_create_ue4File   = new wxButton{this, p_create_ue4File_id, ConvStr<wxString>("创建ue4关卡序列")};
  auto k_mkink            = new wxButton{this, p_mkLink_id, ConvStr<wxString>("映射连接文件夹")};
  //布局
  layout->Add(k_exMaya_button, wxSizerFlags{0}.Expand().Border(wxALL, 0))->SetProportion(1);
  layout->Add(k_create_image, wxSizerFlags{0}.Expand().Border(wxALL, 0))->SetProportion(1);
  layout->Add(k_create_dir_image, wxSizerFlags{0}.Expand().Border(wxALL, 0))->SetProportion(1);
  layout->Add(k_create_video, wxSizerFlags{0}.Expand().Border(wxALL, 0))->SetProportion(1);
  layout->Add(k_create_ue4File, wxSizerFlags{0}.Expand().Border(wxALL, 0))->SetProportion(1);
  layout->Add(k_mkink, wxSizerFlags{0}.Expand().Border(wxALL, 0))->SetProportion(1);

  //设置布局调整
  SetSizer(layout);
  //设置最小值
  layout->SetSizeHints(this);

  //可以拖拽文件
  k_exMaya_button->DragAcceptFiles(true);
  k_exMaya_button->Bind(wxEVT_DROP_FILES, [this](wxDropFilesEvent& event) {
    const auto num = event.GetNumberOfFiles();
    std::vector<FSys::path> path{};
    auto wxPath = event.GetFiles();
    if (num > 0) {
      for (auto i = 0; i < num; ++i)
        path.emplace_back(ConvStr<std::string>(wxPath[i]));
    }
    this->exportMayaFile(path);
  });

  k_create_image->DragAcceptFiles(true);
  k_create_image->Bind(wxEVT_DROP_FILES, [this](wxDropFilesEvent& event) {
    std::vector<FSys::path> path = convertPath(event);
    this->createVideoFile(path);
  });

  k_create_dir_image->DragAcceptFiles(true);
  k_create_dir_image->Bind(wxEVT_DROP_FILES, [this](wxDropFilesEvent& event) {
    std::vector<FSys::path> path = convertPath(event);
    this->createVideoFileFormDir(path);
  });

  k_create_video->DragAcceptFiles(true);
  k_create_video->Bind(wxEVT_DROP_FILES, [this](wxDropFilesEvent& event) {
    std::vector<FSys::path> path = convertPath(event);
    this->connectVideo(path);
  });

  k_create_ue4File->DragAcceptFiles(true);
  k_create_ue4File->Bind(wxEVT_DROP_FILES, [this](wxDropFilesEvent& event) {
    std::vector<FSys::path> path = convertPath(event);
    this->createUe4Project(path);
  });

  k_mkink->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
    auto mk  = MklinkWidget{this, wxID_ANY};
    auto k_r = mk.ShowModal();
  });
  k_create_ue4File->Bind(
      wxEVT_BUTTON,
      [](wxCommandEvent& event) {
        wxGetApp().openSettingWindow();
      });

  Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event) {
    this->Hide();
    if (event.CanVeto())
      event.Veto(false);
  });

  //调整大小
  this->Layout();
  this->Center(wxBOTH);
  this->SetSize(wxSize{400, 350});
}

std::vector<FSys::path> tool_windows::convertPath(const wxDropFilesEvent& event) {
  const auto num = event.GetNumberOfFiles();
  std::vector<FSys::path> path{};
  auto wxPath = event.GetFiles();
  if (num > 0) {
    for (auto i = 0; i < num; ++i)
      path.emplace_back(ConvStr<std::string>(wxPath[i]));
  }
  return path;
}

void tool_windows::exportMayaFile(const std::vector<FSys::path>& paths) {
  auto maya    = std::make_shared<MayaFile>();
  auto process = new MessageAndProgress{this};
  process->createProgress(maya);

  std::thread{
      [maya, paths]() { maya->batchExportFbxFile(paths); }}
      .detach();
}

void tool_windows::createVideoFile(const std::vector<FSys::path>& paths) {
  auto image   = std::make_shared<ImageSequence>(paths);
  auto process = new MessageAndProgress{this};

  process->createProgress(image);
  auto path = paths.at(0).parent_path() / image->getEpisodesAndShot_str().append(".mp4");
  std::thread{
      [image, path]() {
        image->createVideoFile(path);
      }}
      .detach();
}

void tool_windows::createVideoFileFormDir(const std::vector<FSys::path>& paths) {
  auto bath    = std::make_shared<ImageSequenceBatch>(paths);
  auto process = new MessageAndProgress{this};

  process->createProgress(bath);
  std::thread{
      [bath] {
        bath->batchCreateSequence();
      }}
      .detach();
}

void tool_windows::connectVideo(const std::vector<FSys::path>& paths) {
  auto data    = std::make_shared<VideoSequence>(paths);
  auto process = new MessageAndProgress{this};

  process->createProgress(data);
  std::thread{
      [data] {
        data->connectVideo();
      }}
      .detach();
}

void tool_windows::createUe4Project(const std::vector<FSys::path>& paths) {
  try {
    auto ue              = std::make_shared<Ue4Project>(paths[0]);
    auto [k_eps, k_shot] = ShotListDialog::getShotList();
    if (k_shot.empty()) {
      wxMessageDialog{this, ConvStr<wxString>("用户取消创建")}.ShowModal();
      return;
    }
    ue->createShotFolder(k_shot);
  } catch (const std::exception& error) {
    DOODLE_LOG_INFO(error.what());
    return;
  }
  wxMessageDialog{this, ConvStr<wxString>("成功创建")}.ShowModal();
}

}
