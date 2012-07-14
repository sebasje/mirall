project(owncloudsyncd)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${owncloudsyncdaemon_BINARY_DIR}
  ../..
  )

set(owncloudsyncd_SRCS
  owncloudsync.cpp
  owncloudsyncdaemon.cpp
  ../../mirall/owncloudinfo.cpp
  main.cpp
  )

qt4_add_dbus_adaptor(owncloudsyncd_SRCS
  org.kde.owncloudsync.xml
  owncloudsyncdaemon.h
  OwncloudSyncDaemon)

kde4_add_kdeinit_executable(owncloudsyncd ${owncloudsyncd_SRCS})

ADD_LIBRARY(owncloudsyncd_SRCS STATIC IMPORTED)

target_link_libraries(kdeinit_owncloudsyncd
  #${KDE4_KDEUI_LIBS}
  ${QT_QTDBUS_LIBRARY}
  ${QT_QTCORE_LIBRARY}
  owncloudsync
)

target_link_libraries(owncloudsyncd
  kdeinit_owncloudsyncd
)

install(
  FILES owncloudsync.desktop
  DESTINATION ${AUTOSTART_INSTALL_DIR})

install(
  TARGETS kdeinit_owncloudsyncd
  DESTINATION ${LIB_INSTALL_DIR})

install(
  TARGETS owncloudsyncd
  ${INSTALL_TARGETS_DEFAULT_ARGS})
