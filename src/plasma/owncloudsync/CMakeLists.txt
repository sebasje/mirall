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
  kfilesync.cpp
  #../../mirall/owncloudinfo.cpp # FIXME: Ouch. :/
  main.cpp
  )

qt5_add_dbus_adaptor(owncloudsyncd_SRCS
  org.kde.owncloudsync.xml
  owncloudsync.h
  OwncloudSync)

kf5_add_kdeinit_executable(owncloudsyncd ${owncloudsyncd_SRCS})

ADD_LIBRARY(owncloudsyncd_SRCS STATIC IMPORTED)

target_link_libraries(kdeinit_owncloudsyncd
    Qt5::Core
    Qt5::DBus
    Qt5::Qml
    KF5::KIO
    KF5::KDELibs4Support
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
