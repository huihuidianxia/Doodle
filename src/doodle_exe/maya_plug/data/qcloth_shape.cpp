//
// Created by TD on 2021/12/6.
//

#include "qcloth_shape.h"
#include "doodle_lib/metadata/metadata.h"

#include <doodle_lib/metadata/project.h>

#include <maya/MDagPath.h>
#include <maya/MFileIO.h>
#include <maya/MFnMesh.h>
#include <maya/MPlug.h>
#include <maya/MNamespace.h>
#include <maya/MItDag.h>
#include <maya/MDagModifier.h>
#include <maya/MFnSet.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MFnBlendShapeDeformer.h>

#include <maya_plug/data/reference_file.h>
#include <maya_plug/data/maya_file_io.h>
#include <maya_plug/maya_plug_fwd.h>

#include <magic_enum.hpp>

namespace doodle::maya_plug {

namespace qcloth_shape_n {
maya_obj::maya_obj() = default;
maya_obj::maya_obj(const MObject& in_object) {
  obj = in_object;
  MStatus k_s;
  MFnDependencyNode k_node{in_object, &k_s};
  DOODLE_CHICK(k_s);
  p_name = d_str{k_node.name(&k_s)};
  DOODLE_CHICK(k_s);
}
}  // namespace qcloth_shape_n

qcloth_shape::qcloth_shape() = default;

qcloth_shape::qcloth_shape(const entt::handle& in_ref_file, const MObject& in_object)
    : qcloth_shape() {
  p_ref_file = in_ref_file;
  obj        = in_object;
  chick_component<reference_file>(p_ref_file);
}
bool qcloth_shape::set_cache_folder() const {
  MStatus k_s{};
  /// \brief 获得解算节点fn
  MFnDependencyNode k_node{obj, &k_s};
  DOODLE_CHICK(k_s);
  string k_name{d_str{k_node.name(&k_s)}};
  DOODLE_CHICK(k_s);
  string k_namespace = p_ref_file.get<reference_file>().get_namespace();
  auto& k_cfg        = p_ref_file.get<reference_file>().get_prj().get<project::cloth_config>();

  DOODLE_CHICK(k_s);
  string k_node_name = d_str{MNamespace::stripNamespaceFromName(k_node.name(), &k_s)};
  DOODLE_CHICK(k_s);
  {
    auto k_cache = k_node.findPlug(d_str{"cacheFolder"}, false, &k_s);
    DOODLE_CHICK(k_s);
    auto k_file_name       = maya_file_io::get_current_path();
    /// \brief 使用各种信息确认缓存相对路径
    const string& l_string = fmt::format("cache/{}/{}/{}",
                                         k_file_name.stem().generic_string(),
                                         k_namespace,
                                         k_node_name);
    DOODLE_LOG_INFO("设置缓存路径 {}", l_string);
    /// \brief 删除已经缓存的目录
    auto k_path = maya_file_io::work_path(l_string);
    if (FSys::exists(k_path)) {
      DOODLE_LOG_INFO("发现缓存目录, 主动删除 {}", k_path);
      FSys::remove_all(k_path);
    }
    FSys::create_directories(k_path);
    k_s = k_cache.setString(d_str{l_string});
    DOODLE_CHICK(k_s);
  }
  {
    auto k_cache = k_node.findPlug(d_str{"cacheName"}, true, &k_s);
    DOODLE_CHICK(k_s);
    k_cache.setString(d_str{k_node_name});
  }
  return true;
}

bool qcloth_shape::create_cache() const {
  if (obj.isNull())
    throw doodle_error{"空组件"};
  MStatus k_s{};
  /// 直接使用 MItDependencyGraph 搜素 kMesh 类型并同步
  MFnMesh k_shape{get_first_mesh(obj), &k_s};
  DOODLE_CHICK(k_s);
  k_s = k_shape.updateSurface();
  DOODLE_CHICK(k_s);
  k_s = k_shape.syncObject();
  DOODLE_CHICK(k_s);

  return true;
}

void qcloth_shape::create_sim_cloth(const entt::handle& in_handle) {
  chick_component<qcloth_shape_n::maya_obj, qcloth_shape_n::shape_list>(in_handle);
  chick_ctx<root_ref>();
  auto& k_ref  = g_reg()->ctx<root_ref>().root_handle().get<project::cloth_config>();
  auto l_group = get_cloth_group();

  MStatus k_s{};
  MFnDagNode l_node{};
  MDagModifier l_modifier{};

  /// \brief 主要的动画输出节点(需要输入到解算输入端)
  auto& k_anim_mesh = in_handle.get<qcloth_shape_n::maya_obj>();
  k_s               = l_node.setObject(k_anim_mesh.obj);
  DOODLE_CHICK(k_s);
  string k_anim_mesh_name = d_str{l_node.name(&k_s)};
  DOODLE_CHICK(k_s);
  /// \brief 主要的输入节点
  auto k_proxy_node = l_node.duplicate(false, false, &k_s);
  {  ///重命名节点
    k_s = l_node.setObject(k_proxy_node);
    DOODLE_CHICK(k_s);
    l_node.setName(d_str{fmt::format("{}{}", k_anim_mesh_name, k_ref.cloth_proxy)}, false, &k_s);
    DOODLE_CHICK(k_s);
    /// 设置材质
    MFnSet l_mat{get_shading_engine(k_anim_mesh.obj), &k_s};
    DOODLE_CHICK(k_s);
    k_s = l_mat.addMember(k_proxy_node);
    DOODLE_CHICK(k_s);
  }
  DOODLE_CHICK(k_s);
  auto& k_maya_high_mesh = in_handle.get<qcloth_shape_n::shape_list>();
  std::vector<MObject> l_high_mesh{};
  {
    /// 复制高模节点作为输出
    std::transform(k_maya_high_mesh.begin(), k_maya_high_mesh.end(),
                   std::back_inserter(l_high_mesh),
                   [&](const qcloth_shape_n::maya_obj& in_object) -> MObject {
                     k_s = l_node.setObject(in_object.obj);
                     DOODLE_CHICK(k_s);
                     auto l_r = l_node.duplicate(false, false, &k_s);
                     DOODLE_CHICK(k_s);
                     /// 设置材质
                     MFnSet l_mat{get_shading_engine(in_object.obj), &k_s};
                     DOODLE_CHICK(k_s);
                     k_s = l_mat.addMember(l_r);
                     DOODLE_CHICK(k_s);
                     return l_r;
                   });
  }
  MDagPath l_path{};
  {  /// 创建动画网格和解算网络的输入
    /// 连接两个属性的输入和输出
    k_s = l_modifier.connect(get_plug(k_anim_mesh.obj, "outMesh"),
                             get_plug(k_proxy_node, "inMesh"));
    DOODLE_CHICK(k_s);
    k_s = l_modifier.doIt();
    DOODLE_CHICK(k_s);
  }
  {
    /// 创建解算网络
    MSelectionList l_selection_list{};
    k_s = l_selection_list.add(k_proxy_node);
    DOODLE_CHICK(k_s);
    k_s = MGlobal::setActiveSelectionList(l_selection_list);
    DOODLE_CHICK(k_s);
    k_s = MGlobal::executeCommand(d_str{"qlCreateCloth;"});
    DOODLE_CHICK(k_s);
  }
  {
    /// 创建包裹变形(maya的包裹变形需要先选择高模, 可以多个, 然后选中低模) 包裹时需要添加独占式绑定参数
    MSelectionList k_select{};
    /// 添加高模
    std::for_each(l_high_mesh.begin(), l_high_mesh.end(),
                  [&](const MObject& in_obj) -> void {
                    k_s = k_select.add(in_obj, false);
                    DOODLE_CHICK(k_s);
                  });
    /// 低模需要遍历寻找
    MObject k_low{};
    auto l_shape = get_shape(k_proxy_node);
    for (MItDependencyGraph i{l_shape, MFn::kMesh};
         !i.isDone();
         i.next()) {
      k_low = i.currentItem(&k_s);
      DOODLE_CHICK(k_s);
    }
    if (k_low.isNull())
      throw doodle_error{"没有找到低模"};

    k_s = k_select.add(get_transform(k_low), false);
    DOODLE_CHICK(k_s);
    /// 设置选择
    k_s = MGlobal::setActiveSelectionList(k_select);
    DOODLE_CHICK(k_s);
    k_s = MGlobal::executeCommand(d_str{R"(doWrapArgList "4" {"1","0","0.1", "1","1"};)"});
    DOODLE_CHICK(k_s);
  }
  {
    /// 创建解算网络的输出 这个可以用融合变形(其中先选择主动变形物体, 再选择被变形物体)
    chick_true<maya_error>(l_high_mesh.size() == k_maya_high_mesh.size(), DOODLE_SOURCE_LOC, "节点数量不一致");
    for (int l_i = 0; l_i < l_high_mesh.size(); ++l_i) {
      auto& l_aim = k_maya_high_mesh[l_i].obj;
      auto& l_sim = l_high_mesh[l_i];
      /// 先将高模权重重置为0;
      MFnBlendShapeDeformer k_b{l_aim, &k_s};
      MIntArray l_array{};
      k_s = k_b.weightIndexList(l_array);
      DOODLE_CHICK(k_s);
      for (int l_j = 0; l_j < l_array.length(); ++l_j) {
        k_b.setWeight(l_array[l_j], 0);
      }
      /// 创建选择列表
      MSelectionList l_list{};
      k_s = l_list.add(l_sim);
      DOODLE_CHICK(k_s);
      k_s = l_list.add(l_aim);
      DOODLE_CHICK(k_s);
      k_s = MGlobal::setActiveSelectionList(l_list);
      DOODLE_CHICK(k_s);
      /// 这个设置包裹
      k_s = MGlobal::executeCommand(d_str{R"(blendShape -automatic;)"});
      DOODLE_CHICK(k_s);
      /// 开始设置权重
      MFnDependencyNode l_node1{l_aim, &k_s};
      DOODLE_CHICK(k_s);
      string l_aim_name = d_str{l_node.name(&k_s)};
      DOODLE_CHICK(k_s);

      k_s = l_node.setObject(l_sim);
      DOODLE_CHICK(k_s);

      string l_sim_name = d_str{l_node.name(&k_s)};
      DOODLE_CHICK(k_s);
      MGlobal::executeCommand(d_str{fmt::format(R"(setAttr "{}.{}" 1;)",
                                                l_aim_name, l_sim_name)});
    }
  }
}

qcloth_shape::cloth_group qcloth_shape::get_cloth_group() {
  MStatus k_s{};
  qcloth_shape::cloth_group k_r{};
  MFnDagNode k_node{};

  for (MItDag i{MItDag::kDepthFirst, MFn::Type::kTransform, &k_s}; !i.isDone(); i.next()) {
    DOODLE_CHICK(k_s);
    k_s = k_node.setObject(i.currentItem());
    DOODLE_CHICK(k_s);
    if (k_node.name(&k_s) == "cfx_grp") {
      DOODLE_CHICK(k_s);
      k_r.cfx_grp = i.currentItem(&k_s);
      DOODLE_CHICK(k_s);
    } else if (k_node.name(&k_s) == "anim_grp") {
      DOODLE_CHICK(k_s);
      k_r.anim_grp = i.currentItem(&k_s);
      DOODLE_CHICK(k_s);
    } else if (k_node.name(&k_s) == "constraint_grp") {
      DOODLE_CHICK(k_s);
      k_r.constraint_grp = i.currentItem(&k_s);
      DOODLE_CHICK(k_s);
    } else if (k_node.name(&k_s) == "collider_grp") {
      DOODLE_CHICK(k_s);
      k_r.collider_grp = i.currentItem(&k_s);
      DOODLE_CHICK(k_s);
    } else if (k_node.name(&k_s) == "deform_grp") {
      DOODLE_CHICK(k_s);
      k_r.deform_grp = i.currentItem(&k_s);
      DOODLE_CHICK(k_s);
    } else if (k_node.name(&k_s) == "export_grp") {
      DOODLE_CHICK(k_s);
      k_r.export_grp = i.currentItem(&k_s);
      DOODLE_CHICK(k_s);
    }
  }
  MDagModifier k_m{};
  if (k_r.cfx_grp.isNull()) {
    k_r.cfx_grp = k_m.createNode(d_str{"transform"}, MObject::kNullObj, &k_s);
    DOODLE_CHICK(k_s);
    k_s = k_m.renameNode(k_r.cfx_grp, "cfx_grp");
    DOODLE_CHICK(k_s);
    k_s = k_m.doIt();
    DOODLE_CHICK(k_s);
  }
  if (k_r.anim_grp.isNull()) {
    k_r.anim_grp = k_m.createNode(d_str{"transform"}, k_r.cfx_grp, &k_s);
    DOODLE_CHICK(k_s);
    k_s = k_m.renameNode(k_r.anim_grp, "anim_grp");
    DOODLE_CHICK(k_s);
    k_s = k_m.doIt();
    DOODLE_CHICK(k_s);
  }
  if (k_r.constraint_grp.isNull()) {
    k_r.constraint_grp = k_m.createNode(d_str{"transform"}, k_r.cfx_grp, &k_s);
    DOODLE_CHICK(k_s);
    k_s = k_m.renameNode(k_r.constraint_grp, "constraint_grp");
    DOODLE_CHICK(k_s);
    k_s = k_m.doIt();
    DOODLE_CHICK(k_s);
  }
  if (k_r.collider_grp.isNull()) {
    k_r.collider_grp = k_m.createNode(d_str{"transform"}, k_r.cfx_grp, &k_s);
    DOODLE_CHICK(k_s);
    k_s = k_m.renameNode(k_r.collider_grp, "collider_grp");
    DOODLE_CHICK(k_s);
    k_s = k_m.doIt();
    DOODLE_CHICK(k_s);
  }
  if (k_r.deform_grp.isNull()) {
    k_r.deform_grp = k_m.createNode(d_str{"transform"}, k_r.cfx_grp, &k_s);
    DOODLE_CHICK(k_s);
    k_s = k_m.renameNode(k_r.deform_grp, "deform_grp");
    DOODLE_CHICK(k_s);
    k_s = k_m.doIt();
    DOODLE_CHICK(k_s);
  }
  if (k_r.export_grp.isNull()) {
    k_r.export_grp = k_m.createNode(d_str{"transform"}, k_r.cfx_grp, &k_s);
    DOODLE_CHICK(k_s);
    k_s = k_m.renameNode(k_r.export_grp, "export_grp");
    DOODLE_CHICK(k_s);
    k_s = k_m.doIt();
    DOODLE_CHICK(k_s);
  }

  return k_r;
}

void qcloth_shape::add_child(const MObject& in_praent, MObject& in_child) {
  MStatus k_s{};
  MFnDagNode k_node{in_praent, &k_s};
  DOODLE_CHICK(k_s);
  k_s = k_node.addChild(in_child);
  DOODLE_CHICK(k_s);
}

}  // namespace doodle::maya_plug
