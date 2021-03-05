#pragma once

#include <MotionGlobal.h>

#include <QtWidgets/QListView>

namespace doodle::motion::ui {
class MotionView : public QListView {
  Q_OBJECT

  TreeDirItemPtr p_TreeDirItem;

 public:
  MotionView(QWidget* parent = nullptr);

  void setTreeNode(const decltype(p_TreeDirItem)& item);

 protected:
  //上下文菜单
  void contextMenuEvent(QContextMenuEvent* event) override;

  // //拖拽函数
  // void dragMoveEvent(QDragMoveEvent* event) override;
  // //拖拽函数
  // void dragLeaveEvent(QDragLeaveEvent* event) override;
  // //拖拽函数
  // void dragEnterEvent(QDragEnterEvent* event) override;
  // //拖拽函数
  // void dropEvent(QDropEvent* event) override;

 private:
  void createFbxAction(const FSys::path& path);
};

}  // namespace doodle::motion::ui