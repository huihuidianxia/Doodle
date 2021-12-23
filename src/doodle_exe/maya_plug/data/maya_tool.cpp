//
// Created by TD on 2021/12/16.
//

#include "maya_tool.h"
#include <maya/MPlug.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MItDependencyGraph.h>

#include <maya_plug/maya_plug_fwd.h>
namespace doodle::maya_plug {

MPlug get_plug(const MObject& in_node, const std::string& in_name) {
  MStatus k_s{};
  MFnDependencyNode l_node{in_node, &k_s};
  MPlug l_plug{};

  try {
    l_plug = l_node.findPlug(d_str{in_name}, false, &k_s);
    DOODLE_CHICK(k_s);
  } catch (const maya_InvalidParameter& error) {
    DOODLE_LOG_INFO("没有在这个节点中找到属性")
  }

  if (in_node.hasFn(MFn::kDagNode)) {
    MFnDagNode l_dag_node{in_node, &k_s};
    DOODLE_CHICK(k_s);
    MDagPath l_path{};
    k_s = l_dag_node.getPath(l_path);
    DOODLE_CHICK(k_s);
    try {
      k_s = l_path.extendToShape();
      DOODLE_CHICK(k_s);
      MFnDagNode l_dag_node_shape{l_path, &k_s};
      l_plug = l_dag_node_shape.findPlug(d_str{in_name}, false, &k_s);
      DOODLE_CHICK(k_s)
    } catch (const maya_InvalidParameter& error) {
      DOODLE_LOG_INFO("节点下方没有 shape 形状节点, 不需要寻找形状节点")
    }
  }
  chick_true<doodle_error>(!l_plug.isNull(), DOODLE_SOURCE_LOC, "无法找到属性");
  return l_plug;
}
MObject get_shading_engine(const MObject& in_node) {
  MStatus k_s{};
  MObject k_obj = in_node;
  if (in_node.hasFn(MFn::kDagNode)) {
    MFnDagNode l_dag_node{in_node, &k_s};
    DOODLE_CHICK(k_s);
    MDagPath l_path{};
    k_s = l_dag_node.getPath(l_path);
    DOODLE_CHICK(k_s);
    k_s = l_path.extendToShape();
    DOODLE_CHICK(k_s);
    k_obj = l_path.node(&k_s);
    DOODLE_CHICK(k_s);
  }
  MObject obj{};
  for (MItDependencyGraph i{k_obj,
                            MFn::Type::kShadingEngine,
                            MItDependencyGraph::Direction::kDownstream,
                            MItDependencyGraph::Traversal::kDepthFirst,
                            MItDependencyGraph::Level::kNodeLevel,
                            &k_s};
       !i.isDone();
       i.next()) {
    DOODLE_CHICK(k_s);
    obj = i.currentItem(&k_s);
    //    DOODLE_CHICK(k_s);
    //    MFnDependencyNode k_node{};
    //    k_node.setObject(obj);
    //    DOODLE_LOG_INFO(fmt::format("找到节点 {}", d_str{k_node.name()}.str()));
    break;
  }
  chick_true<doodle_error>(!obj.isNull(), DOODLE_SOURCE_LOC, "没有找到节点");
  return obj;
}
MObject get_first_mesh(const MObject& in_node) {
  MStatus k_s{};
  MObject k_obj = in_node;
  MObject l_r{};

  for (MItDependencyGraph i{k_obj,
                            MFn::Type::kMesh,
                            MItDependencyGraph::Direction::kDownstream,
                            MItDependencyGraph::Traversal::kDepthFirst,
                            MItDependencyGraph::Level::kNodeLevel,
                            &k_s};
       !i.isDone();
       i.next()) {
    DOODLE_CHICK(k_s);
    l_r = i.currentItem(&k_s);
    break;
  }
  chick_true<maya_error>(!l_r.isNull(), DOODLE_SOURCE_LOC, "没有在依赖网格中寻找到mesh节点");
  return l_r;
}
MObject get_shape(const MObject& in_object) {
  MStatus k_s{};
  MObject k_obj = in_object;
  MObject k_r{};
  if (k_obj.hasFn(MFn::kDagNode)) {
    MFnDagNode l_dag_node{k_obj, &k_s};
    DOODLE_CHICK(k_s);
    MDagPath l_path{};
    k_s = l_dag_node.getPath(l_path);
    DOODLE_CHICK(k_s);
    k_s = l_path.extendToShape();
    DOODLE_CHICK(k_s);
    k_r = l_path.node(&k_s);
    DOODLE_CHICK(k_s);
  }
  chick_true<maya_error>(!k_r.isNull(), DOODLE_SOURCE_LOC, "没有找到形状");
  return k_r;
}
MObject get_transform(const MObject& in_object) {
  MStatus k_s{};
  MObject k_obj = in_object;
  MObject k_r{};
  if (k_obj.hasFn(MFn::kDagNode)) {
    MFnDagNode l_dag_node{k_obj, &k_s};
    DOODLE_CHICK(k_s);
    MDagPath l_path{};
    k_s = l_dag_node.getPath(l_path);
    DOODLE_CHICK(k_s);
    k_r = l_path.transform(&k_s);
    DOODLE_CHICK(k_s);
  }
  chick_true<maya_error>(!k_r.isNull(), DOODLE_SOURCE_LOC, "没有找到变换");
  return k_r;
}
}  // namespace doodle::maya_plug
