//
// Created by TD on 2022/3/4.
//
#pragma once

#include <maya_plug/maya_plug_fwd.h>
namespace doodle::maya_plug {

class maya_poly_info {
 private:
 public:
  std::int32_t numVertices;
  std::int32_t numEdges;
  std::int32_t numPolygons;
  std::int32_t numFaceVertices;
  //  std::int32_t polygonVertexCount;
  std::int32_t numUVs;
  std::int32_t numUVSets;
  MObject maya_obj;
  bool has_skin;
  bool is_intermediate_obj;
  bool has_cloth;
  //  std::int32_t numColors;
  //  std::int32_t numColorSets;
  maya_poly_info();
  explicit maya_poly_info(const MObject& in_mesh_object);
  bool operator==(const maya_poly_info& in_rhs) const;
  bool operator!=(const maya_poly_info& in_rhs) const;
  void set_mesh_info(const MObject& in_mesh_object);
  bool has_skin_cluster(const MObject& in_object);
  bool has_cloth_link(const MObject& in_object);
};

}  // namespace doodle::maya_plug
