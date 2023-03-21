#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

// #include "CreateCharacterMianUI.generated.h"  // 头生成

class SCharacterEditorViewport;
struct FDoodleCreateCharacterConfigNode;
class UDoodleCreateCharacterConfig;
class SCreateCharacterCurveEditor;

class ITableRow;
class STableViewBase;
class SCreateCharacterTree;
class FCreateCharacterSliderController;

// 创建编辑器自定义界面
class FCreateCharacterMianUI : public FAssetEditorToolkit, public FGCObject {
 public:
  // IToolkit 接口
  virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& In_TabManager) override;
  virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& In_TabManager) override;
  // 结束 of IToolkit 接口

  // FAssetEditorToolkit 接口
  virtual FName GetToolkitFName() const override;
  virtual FText GetBaseToolkitName() const override;
  virtual FText GetToolkitName() const override;
  virtual FText GetToolkitToolTipText() const override;
  virtual FLinearColor GetWorldCentricTabColorScale() const override;
  virtual FString GetWorldCentricTabPrefix() const override;
  virtual FString GetDocumentationLink() const override;
  // 结束 of FAssetEditorToolkit 接口

  // GC接口
  virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
  // 结束 GC接口

  void InitCreateCharacterMianUI(
      EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& In_InitToolkitHost,
      UDoodleCreateCharacterConfig* In_Config
  );

 private:
  // 预览场景视口
  TSharedRef<SDockTab> SpawnTab_Viewport(const FSpawnTabArgs& Args);
  // 调整人物视口
  TSharedRef<SDockTab> SpawnTab_Tree(const FSpawnTabArgs& Args);

  TSharedRef<SDockTab> SpawnTab_CurveEditor(const FSpawnTabArgs& Args);

  // 预览视口
  TSharedPtr<SCharacterEditorViewport> CharacterEditorViewport;
  // 滑块调整视口
  TSharedPtr<SCreateCharacterTree> CreateCharacterTree;
  // 曲线
  TSharedPtr<SCreateCharacterCurveEditor> CreateCharacterCurveEditor;
  // 时间
  TSharedPtr<FCreateCharacterSliderController> CreateCharacterSliderController;

  // 树 id
  const static FName TreeID;
  // 视口id
  const static FName ViewportID;
  // 当模式为独立时，这是应用的标识符，该应用应该承载这个工具包。
  const static FName AppIdentifier;

  const static FName CurveEditor;
  // 创建角色配置
  TObjectPtr<UDoodleCreateCharacterConfig> CreateCharacterConfig;
};