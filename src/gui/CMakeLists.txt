qt_add_executable(playarchive_gui
    main.cpp
    ArchiveModel.cpp ArchiveModel.h
)

qt_add_qml_module(playarchive_gui
    URI PlayArchive
    VERSION 1.0
    QML_FILES Main.qml
)

target_link_libraries(playarchive_gui
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Concurrent
    playarchive_core
)

set_target_properties(playarchive_gui PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)
