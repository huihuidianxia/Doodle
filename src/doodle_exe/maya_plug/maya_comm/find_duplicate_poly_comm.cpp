//
// Created by TD on 2022/3/8.
//

#include "find_duplicate_poly_comm.h"
#include <data/find_duplicate_poly.h>
#include <maya/MArgParser.h>
#include <maya/MNamespace.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <data/maya_tool.h>

namespace doodle::maya_plug {
namespace {
constexpr char name_s[]      = "-ns";
constexpr char name_s_long[] = "-namespace";

}  // namespace
MSyntax find_duplicate_poly_comm_syntax() {
  MSyntax l_syntax{};
  l_syntax.addFlag(name_s, name_s_long, MSyntax::MArgType::kString);
  return l_syntax;
}
MStatus find_duplicate_poly_comm::doIt(const MArgList& in_list) {
  MStatus l_status{};
  MArgParser k_prase{syntax(), in_list, &l_status};
  MString l_name_s{};
  if (k_prase.isFlagSet(name_s, &l_status)) {
    DOODLE_CHICK(l_status);
    k_prase.getFlagArgument(name_s, 0, l_name_s);
  }

  auto l_list_p = find_duplicate_poly{}(MNamespace::getNamespaceObjects(l_name_s, false, &l_status));
  //  MFnDagNode l_dag_node{};
  MSelectionList l_list{};
  for (auto&& i : l_list_p) {
    DOODLE_CHICK(l_list.add(get_transform(i.first)));
    DOODLE_CHICK(l_list.add(get_transform(i.second)));
  }
  MGlobal::setActiveSelectionList(l_list);

  return l_status;
}

}  // namespace doodle::maya_plug
