#include <DoodleLib/Exception/Exception.h>
#include <DoodleLib/FileWarp/MayaFile.h>
#include <DoodleLib/core/CoreSet.h>
#include <DoodleLib/core/DoodleLib.h>
#include <DoodleLib/threadPool/ThreadPool.h>
#include <DoodleLib/threadPool/long_term.h>
#include <Logger/Logger.h>

#include <boost/locale.hpp>
#include <boost/process.hpp>

namespace doodle {
MayaFile::MayaFile(FSys::path mayaPath)
    : p_path(std::move(mayaPath)),
      p_term(std::make_shared<long_term>()),
      p_term_list(),
      p_futurn_list() {
  if (!FSys::exists(p_path) && CoreSet::getSet().hasMaya())
    p_path = CoreSet::getSet().MayaPath();
  else
    throw DoodleError{"无法找到maya启动器"};
}

FSys::path MayaFile::createTmpFile(const std::string& in_resource_path) {
  //开始写入临时文件

  const static auto tmp_path = CoreSet::getSet().getCacheRoot("maya");
  auto k_tmp_path            = tmp_path / (boost::uuids::to_string(CoreSet::getSet().getUUID()) + ".py");
  auto k_file_py             = cmrc::DoodleLibResource::get_filesystem().open(in_resource_path);

  {  //写入文件后直接关闭
    FSys::fstream file{k_tmp_path, std::ios::out | std::ios::binary};
    file.write(k_file_py.begin(), boost::numeric_cast<std::int64_t>(k_file_py.size()));
  }
  return k_tmp_path;
}

void MayaFile::write_maya_tool_file() {
  const static auto tmp_path = CoreSet::getSet().getCacheRoot(
      fmt::format("maya/v{}{}{}",
                  Doodle_VERSION_MAJOR,
                  Doodle_VERSION_MINOR,
                  Doodle_VERSION_PATCH));
  auto k_tmp_path = tmp_path / "maya_fun_tool.py";
  if (FSys::exists(k_tmp_path))
    return;

  auto k_file_py = cmrc::DoodleLibResource::get_filesystem().open("resource/maya_fun_tool.py");
  {  //写入文件后直接关闭
    FSys::fstream file{k_tmp_path, std::ios::out | std::ios::binary};
    file.write(k_file_py.begin(), boost::numeric_cast<std::int64_t>(k_file_py.size()));
  }
}

FSys::path MayaFile::warit_tmp_file(const std::string& in_string) {
  const static auto tmp_path = CoreSet::getSet().getCacheRoot(
      fmt::format("maya/v{}{}{}",
                  Doodle_VERSION_MAJOR,
                  Doodle_VERSION_MINOR,
                  Doodle_VERSION_PATCH));
  auto k_tmp_path = tmp_path / (boost::uuids::to_string(CoreSet::getSet().getUUID()) + ".py");
  {  //写入文件后直接关闭
    FSys::fstream file{k_tmp_path, std::ios::out};
    file << in_string;
  }
  return k_tmp_path;
}

bool MayaFile::checkFile() {
  return true;
}

bool MayaFile::run_comm(const std::wstring& in_com) const {
  STARTUPINFO si{};
  PROCESS_INFORMATION pi{};
  ZeroMemory(&si, sizeof(si));
  ZeroMemory(&pi, sizeof(pi));

  try {
    //使用windowsIPA创建子进程
    CreateProcess(
        nullptr,
        (wchar_t*)in_com.c_str(),
        nullptr,
        nullptr,
        false,
        CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,  // CREATE_NEW_CONSOLE
        nullptr,
        p_path.generic_wstring().c_str(),  // R"(C:\Program Files\Autodesk\Maya2018\bin\)"
        &si,
        &pi);
    p_term->sig_progress(0.1);
    // boost::process::system(command.c_str(), env);
  } catch (const std::runtime_error& err) {
    DOODLE_LOG_WARN(err.what())
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    p_term->sig_finished();
    p_term->sig_message_result("导出失败");
    return false;
  }

  // boost::process::ipstream k_in{};
  // boost::process::child k_c{str.str(), boost::process::windows::hide};
  // boost::process::child k_c{boost::process::cmd = str.str(), boost::process::std_out > k_in, boost::process::windows::hide};

  // auto str_r = std::string{};
  // while (k_c.running() && std::getline(k_in, str_r) && !str_r.empty()) {
  //   // std::cout << str_r << std::endl;
  //   DOODLE_LOG_INFO(str_r);
  // }
  // k_c.wait();

  WaitForSingleObject(pi.hProcess, INFINITE);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  return true;
}

bool MayaFile::exportFbxFile(const FSys::path& file_path, const FSys::path& export_path) const {
  auto k_export_path = file_path.parent_path() / file_path.stem();
  if (!export_path.empty())
    k_export_path = export_path;

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  if (!FSys::exists(k_export_path))
    FSys::create_directories(k_export_path);

  if (!FSys::exists(file_path)) {
    p_term->sig_finished();
    p_term->sig_message_result("不存在文件");
    return false;
  }

  auto k_tmp_path = this->createTmpFile("resource/mayaExport.py");
  //生成命令
  auto str_ = fmt::format(
      LR"("{}/mayapy.exe" {} --path {} --exportpath {})",
      p_path.generic_wstring(),
      k_tmp_path.generic_wstring(),
      file_path.generic_wstring(),
      k_export_path.generic_wstring());

  DOODLE_LOG_INFO(fmt::format(" {} ", boost::locale::conv::utf_to_utf<char>(str_)))

  run_comm(str_);

  p_term->sig_progress(0.9);
  FSys::remove(k_tmp_path);
  FSys::copy_file(file_path, k_export_path / file_path.filename(), FSys::copy_options::overwrite_existing);
  p_term->sig_finished();
  p_term->sig_message_result("导出完成");
  return true;
}

bool MayaFile::batchExportFbxFile(const std::vector<FSys::path>& file_path) const {
  auto thread_pool = DoodleLib::Get().get_thread_pool();
  auto result      = std::map<FSys::path, std::future<bool>>{};
  for (auto&& file : file_path) {
    if (file.extension() == ".ma" || file.extension() == ".mb") {
      result.insert(
          {file,
           thread_pool
               ->enqueue(
                   [this, file]() -> bool {
                     return this->exportFbxFile(file);
                   })});
    }
  }
  std::future_status status{};
  auto it    = result.begin();
  auto size  = boost::numeric_cast<float>(result.size());
  auto k_pro = float{0};

  while (!result.empty()) {
    status = it->second.wait_for(std::chrono::milliseconds{10});
    if (status == std::future_status::ready) {
      //成功就加一
      ++k_pro;
      //添加进度
      p_term->sig_progress(boost::numeric_cast<int>((1 / size)));
      // try {
      // } catch (const DoodleError& err) {
      //   //添加错误
      //   DOODLE_LOG_WARN(err.what());
      // }

      //发送消息
      p_term->sig_message_result(fmt::format("文件:{} --> {}", it->first, ((it->second.get()) ? "成功" : "失败")));

      //擦除容器内数据
      it = result.erase(it);
    } else {
      //等待超时后继续迭代
      ++it;
    }
    if (it == result.end()) {
      it = result.begin();
    }
  }
  p_term->sig_finished();
  return true;
}

long_term_ptr MayaFile::qcloth_sim_file(const FSys::path& file_path) {
  auto k_term = std::make_shared<long_term>();
  p_term_list.push_back(k_term);

  auto k_export_path = file_path.parent_path() / file_path.stem();
  if (!FSys::exists(k_export_path))
    FSys::create_directories(k_export_path);

  if (!FSys::exists(file_path)) {
    k_term->sig_finished();
    k_term->sig_message_result("不存在文件");
    return k_term;
  }

  auto k_fut = DoodleLib::Get().get_thread_pool()->enqueue(
      [k_term, this, file_path]() {
        // 写入文件
        write_maya_tool_file();
        k_term->sig_progress(0.1);

        auto str_script = fmt::format(
            "import maya.standalone\n"
            "maya.standalone.initialize(name='python')\n"
            "import pymel.core.system\n"
            "import pymel.core\n"
            "pymel.core.system.newFile(force=True)\n"
            "pymel.core.system.loadPlugin(\"AbcExport\")\n"
            "pymel.core.system.loadPlugin(\"AbcImport\")\n"
            "pymel.core.system.loadPlugin(\"qualoth_2019_x64\")"
            "\n\npymel.core.system.openFile(\"{}\",loadReferenceDepth=\"all\")\n"
            "pymel.core.playbackOptions(animationStartTime=\"950\")\n"
            "import maya_fun_tool\n"
            "reload(maya_fun_tool)\n"
            "maya_fun_tool.cloth_export()()",
            file_path.generic_string());
        auto run_path = warit_tmp_file(str_script);
        k_term->sig_progress(0.1);
        //生成命令
        auto run_com = fmt::format(
            LR"("{}/mayapy.exe" {})",
            p_path.generic_wstring(),
            run_path.generic_wstring());
        k_term->sig_progress(0.1);
        run_comm(run_com);
        k_term->sig_progress(0.7);
        k_term->sig_finished();
        k_term->sig_message_result(fmt::format("完成导出 :{}", file_path));
      });
  p_futurn_list.push_back(std::move(k_fut));
  return k_term;
}

bool MayaFile::is_maya_file(const FSys::path& in_path) {
  auto k_e = in_path.extension();
  return k_e == ".ma" || k_e == ".mb";
}
long_term_ptr MayaFile::get_term() const {
  return p_term;
}
MayaFile::~MayaFile() {
  for (auto& rus : p_futurn_list) {
    try {
      rus.get();
    } catch (DoodleError& error) {
      DOODLE_LOG_WARN(error.what());
    }
  }
}

}  // namespace doodle
