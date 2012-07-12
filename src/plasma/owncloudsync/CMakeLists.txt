project(owncloudsync)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${owncloudsync_BINARY_DIR}
  )

set(owncloudsync_SRCS
  owncloudsync.cpp
  owncloudsyncdaemon.cpp
  main.cpp
  )

qt4_add_dbus_adaptor(owncloudsync_SRCS
  org.kde.owncloudsync.xml
  owncloudsyncdaemon.h
  OwncloudSyncDaemon)

kde4_add_kdeinit_executable(owncloudsync ${owncloudsync_SRCS})

target_link_libraries(kdeinit_owncloudsync
  #${KDE4_KDEUI_LIBS}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTCORE_LIBRARY}

)

target_link_libraries(owncloudsync
  kdeinit_owncloudsync
)

install(
  FILES owncloudsync.desktop
  DESTINATION ${AUTOSTART_INSTALL_DIR})

install(
  TARGETS kdeinit_owncloudsync
  DESTINATION ${LIB_INSTALL_DIR})

install(
  TARGETS owncloudsync
  ${INSTALL_TARGETS_DEFAULT_ARGS})
