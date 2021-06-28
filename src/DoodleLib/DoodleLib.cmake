set(
	DOODLELIB_HEADER
	DoodleApp.h
	DoodleLib.h
	DoodleLib_fwd.h
	DoodleLib_pch.h
	DoodleMacro.h
	Exception/Exception.h
	FileSys/FileSystem.h
	FileWarp/ImageSequence.h
	FileWarp/MayaFile.h
	FileWarp/Ue4Project.h
	FileWarp/VideoSequence.h
	Gui/Metadata/project_widget.h
	Gui/action/action.h
	Gui/action/assets_action.h
	Gui/action/assets_file_action.h
	Gui/action/project_action.h
	Gui/action/upload_dir_action.h
	Gui/action/upload_dir_and_file_action.h
	Gui/action/upload_file_action.h
	Gui/factory/menu_factory.h
	Gui/main_windows.h
	Gui/setting_windows.h
	Logger/Logger.h
	Logger/LoggerTemplate.h
	Metadata/Assets.h
	Metadata/AssetsFile.h
	Metadata/AssetsPath.h
	Metadata/Comment.h
	Metadata/ContextMenu.h
	Metadata/DragFilesFactory.h
	Metadata/Episodes.h
	Metadata/Label/LabelNode.h
	Metadata/Metadata.h
	Metadata/MetadataFactory.h
	Metadata/MetadataWidget.h
	Metadata/Metadata_cpp.h
	Metadata/Model/AssetsTree.h
	Metadata/Model/ListAttributeModel.h
	Metadata/Model/ProjectManage.h
	Metadata/Project.h
	Metadata/Shot.h
	Metadata/TimeDuration.h
	Metadata/View/ProjectManageWidget.h
	Metadata/View/ShotListWidget.h
	Metadata/View/TimeWidget.h
	PinYin/convert.h
	ScreenshotWidght/ScreenshotAction.h
	ScreenshotWidght/ScreenshotWidght.h
	Server/ServerWidget.h
	SettingWidght/settingWidget.h
	core/ContainerDevice.h
	core/CoreSet.h
	core/CoreSql.h
	core/DoodleLib.h
	core/MetadataSet.h
	core/Name.h
	core/ToolsSetting.h
	core/Ue4Setting.h
	core/Util.h
	core/static_value.h
	libWarp/BoostUuidWarp.h
	libWarp/CerealWarp.h
	libWarp/WinReg.hpp
	libWarp/cache.hpp
	libWarp/cache_policy.hpp
	libWarp/cmrcWarp.h
	libWarp/fifo_cache_policy.hpp
	libWarp/json_warp.h
	libWarp/lfu_cache_policy.hpp
	libWarp/lru_cache_policy.hpp
	libWarp/observable_container.h
	libWarp/sqlppWarp.h
	libWarp/wxWidgetWarp.h
	mainWidght/MklinkWidget.h
	mainWidght/systemTray.h
	mainWidght/tool_windows.h
	rpc/RpcFileSystemClient.h
	rpc/RpcFileSystemServer.h
	rpc/RpcMetadaataServer.h
	rpc/RpcMetadataClient.h
	rpc/RpcServerHandle.h
	threadPool/LongTerm.h
	threadPool/ThreadPool.h
	toolkit/MessageAndProgress.h
	toolkit/toolkit.h)
set(
	DOODLELIB_SOURCE
	DoodleApp.cpp
	DoodleLib.cpp
	FileSys/FileSystem.cpp
	FileWarp/ImageSequence.cpp
	FileWarp/MayaFile.cpp
	FileWarp/Ue4Project.cpp
	FileWarp/VideoSequence.cpp
	Gui/Metadata/project_widget.cpp
	Gui/action/action.cpp
	Gui/action/assets_action.cpp
	Gui/action/assets_file_action.cpp
	Gui/action/project_action.cpp
	Gui/action/upload_dir_action.cpp
	Gui/action/upload_dir_and_file_action.cpp
	Gui/action/upload_file_action.cpp
	Gui/factory/menu_factory.cpp
	Gui/main_windows.cpp
	Gui/setting_windows.cpp
	Logger/Logger.cpp
	Metadata/Assets.cpp
	Metadata/AssetsFile.cpp
	Metadata/AssetsPath.cpp
	Metadata/Comment.cpp
	Metadata/ContextMenu.cpp
	Metadata/DragFilesFactory.cpp
	Metadata/Episodes.cpp
	Metadata/Label/LabelNode.cpp
	Metadata/Metadata.cpp
	Metadata/MetadataFactory.cpp
	Metadata/MetadataWidget.cpp
	Metadata/Model/AssetsTree.cpp
	Metadata/Model/ListAttributeModel.cpp
	Metadata/Model/ProjectManage.cpp
	Metadata/Project.cpp
	Metadata/Shot.cpp
	Metadata/TimeDuration.cpp
	Metadata/View/ProjectManageWidget.cpp
	Metadata/View/ShotListWidget.cpp
	Metadata/View/TimeWidget.cpp
	PinYin/convert.cpp
	ScreenshotWidght/ScreenshotAction.cpp
	ScreenshotWidght/ScreenshotWidght.cpp
	Server/ServerWidget.cpp
	SettingWidght/settingWidget.cpp
	core/CoreSet.cpp
	core/CoreSql.cpp
	core/DoodleLib.cpp
	core/MetadataSet.cpp
	core/Name.cpp
	core/Ue4Setting.cpp
	core/Util.cpp
	core/static_value.cpp
	mainWidght/MklinkWidget.cpp
	mainWidght/systemTray.cpp
	mainWidght/tool_windows.cpp
	rpc/RpcFileSystemClient.cpp
	rpc/RpcFileSystemServer.cpp
	rpc/RpcMetadaataServer.cpp
	rpc/RpcMetadataClient.cpp
	rpc/RpcServerHandle.cpp
	toolkit/MessageAndProgress.cpp
	toolkit/toolkit.cpp
)