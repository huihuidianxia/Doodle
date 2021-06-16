﻿//
// Created by TD on 2021/5/7.
//

#include <DoodleLib/Metadata/AssetsFile.h>
#include <DoodleLib/Metadata/AssetsPath.h>
#include <DoodleLib/Metadata/ContextMenu.h>
///这个工厂类必须在所有导入的后面
#include <DoodleLib/Metadata/MetadataFactory.h>
#include <Metadata/TimeDuration.h>
#include <PinYin/convert.h>

#include <boost/format.hpp>
#include <utility>

namespace doodle {

AssetsFile::AssetsFile()
    : Metadata(),
      p_name(),
      p_ShowName(),
      p_path_file(),
      p_time(std::make_shared<TimeDuration>(std::chrono::system_clock::now())),
      p_user(),
      p_department(),
      p_comment(),
      p_version(1) {
}

AssetsFile::AssetsFile(std::weak_ptr<Metadata> in_metadata, const FSys::path& in_path, std::string name, std::string showName)
    : Metadata(),
      p_name(std::move(name)),
      p_ShowName(std::move(showName)),
      p_path_file(std::make_shared<AssetsPath>(in_path)),
      p_time(std::make_shared<TimeDuration>(std::chrono::system_clock::now())),
      p_user(),
      p_department(),
      p_comment(),
      p_version(1) {
  p_parent = std::move(in_metadata);
  if (p_ShowName.empty())
    p_ShowName = convert::Get().toEn(p_name);
}

// AssetsFile::~AssetsFile() {
//   if (p_metadata_flctory_ptr_)
//     updata_db(p_metadata_flctory_ptr_);
// }

std::string AssetsFile::str() const {
  return p_name;
}
std::string AssetsFile::showStr() const {
  return p_ShowName;
}

bool AssetsFile::operator<(const AssetsFile& in_rhs) const {
  return std::tie(p_name, p_ShowName, p_path_file) < std::tie(in_rhs.p_name, in_rhs.p_ShowName, in_rhs.p_path_file);
  //  return std::tie(static_cast<const doodle::Metadata&>(*this), p_name, p_ShowName, p_path_file) < std::tie(static_cast<const doodle::Metadata&>(in_rhs), in_rhs.p_name, in_rhs.p_ShowName, in_rhs.p_path_file);
}
bool AssetsFile::operator>(const AssetsFile& in_rhs) const {
  return in_rhs < *this;
}
bool AssetsFile::operator<=(const AssetsFile& in_rhs) const {
  return !(in_rhs < *this);
}
bool AssetsFile::operator>=(const AssetsFile& in_rhs) const {
  return !(*this < in_rhs);
}

bool AssetsFile::sort(const Metadata& in_rhs) const {
  if (typeid(in_rhs) == typeid(*this)) {
    return *this < (dynamic_cast<const AssetsFile&>(in_rhs));
  } else {
    return str() < in_rhs.str();
  }
}
void AssetsFile::createMenu(ContextMenu* in_contextMenu) {
  in_contextMenu->createMenu(std::dynamic_pointer_cast<AssetsFile>(shared_from_this()));
}
std::chrono::time_point<std::chrono::system_clock> AssetsFile::getStdTime() const {
  return p_time->getUTCTime();
}
void AssetsFile::setStdTime(const std::chrono::time_point<std::chrono::system_clock>& in_time) {
  p_time = std::make_shared<TimeDuration>(in_time);
  saved(true);
}
const std::string& AssetsFile::getUser() const {
  return p_user;
}
void AssetsFile::setUser(const std::string& in_user) {
  p_user = in_user;
  saved(true);
}

const std::vector<CommentPtr>& AssetsFile::getComment() const {
  return p_comment;
}
void AssetsFile::setComment(const std::vector<CommentPtr>& in_comment) {
  p_comment = in_comment;
  saved(true);
}
void AssetsFile::addComment(const CommentPtr& in_comment) {
  p_comment.emplace_back(in_comment);
  saved(true);
}

const std::uint64_t& AssetsFile::getVersion() const noexcept {
  return p_version;
}

std::string AssetsFile::getVersionStr() const {
  boost::format str{"v%04i"};
  str % p_version;
  return str.str();
}

void AssetsFile::setVersion(const std::uint64_t& in_Version) noexcept {
  p_version = in_Version;
}
const AssetsPathPtr& AssetsFile::getPathFile() const {
  return p_path_file;
}
void AssetsFile::setPathFile(const AssetsPathPtr& in_pathFile) {
  p_path_file = in_pathFile;
  saved(true);
}
Department AssetsFile::getDepartment() const {
  return p_department;
}
void AssetsFile::setDepartment(Department in_department) {
  p_department = in_department;
  saved(true);
}
void AssetsFile::_select_indb(const MetadataFactoryPtr& in_factory) {
  p_metadata_flctory_ptr_->select_indb(this);
}

void AssetsFile::_updata_db(const MetadataFactoryPtr& in_factory) {
  if (isInstall())
    p_metadata_flctory_ptr_->updata_db(this);
  else
    p_metadata_flctory_ptr_->insert_into(this);
}
void AssetsFile::_deleteData(const MetadataFactoryPtr& in_factory) {
  in_factory->deleteData(this);
}
void AssetsFile::_insert_into(const MetadataFactoryPtr& in_factory) {
  in_factory->insert_into(this);
}
const TimeDurationPtr& AssetsFile::getTime() const {
  return p_time;
}
void AssetsFile::setTime(const TimeDurationPtr& in_time) {
  p_time = in_time;
  saved(true);
}
}  // namespace doodle
