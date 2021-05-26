#pragma once
#include <DoodleLib/DoodleLib_fwd.h>
#include <DoodleLib/Metadata/Metadata.h>

#include <cereal/cereal.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/string.hpp>
namespace doodle {
/**
 * 项目信息类
 */
class DOODLELIB_API Project : public Metadata {
  std::string p_name;
  FSys::path p_path;

 public:
  Project();
  explicit Project(FSys::path in_path, std::string in_name = {});

  const std::string& getName() const;
  void setName(const std::string& Name) noexcept;

  [[nodiscard]] const FSys::path& getPath() const noexcept;
  void setPath(const FSys::path& Path);

  [[nodiscard]] std::string str() const override;
  [[nodiscard]] std::string showStr() const override;

  [[nodiscard]] std::string shortStr() const;

  void load(const MetadataFactoryPtr& in_factory) override;
  void save(const MetadataFactoryPtr& in_factory) override;

  static std::string getConfigFileName();
  static std::string getConfigFileFolder();
  bool operator<(const Project& in_rhs) const;
  bool operator>(const Project& in_rhs) const;
  bool operator<=(const Project& in_rhs) const;
  bool operator>=(const Project& in_rhs) const;



  virtual void createMenu(ContextMenu* in_contextMenu) override;
  virtual void deleteData(const MetadataFactoryPtr& in_factory) override;

 protected:
  virtual bool sort(const Metadata& in_rhs) const override;
  void modifyParent(const std::shared_ptr<Metadata>& in_old_parent) override;
  void save() const override;
 private:
  [[nodiscard]] FSys::path DBRoot() const;


  friend class cereal::access;
  template <class Archive>
  void serialize(Archive& ar, std::uint32_t const version);
};

template <class Archive>
void Project::serialize(Archive& ar, std::uint32_t const version) {
  if (version == 1)
    ar(
        cereal::make_nvp("name", p_name),
        cereal::make_nvp("path", p_path));
  if (version == 2)
    ar(
        cereal::make_nvp("Metadata", cereal::base_class<Metadata>(this)),
        cereal::make_nvp("name", p_name),
        cereal::make_nvp("path", p_path));
}

}  // namespace doodle
CEREAL_REGISTER_TYPE(doodle::Project)
CEREAL_CLASS_VERSION(doodle::Project, 2);
