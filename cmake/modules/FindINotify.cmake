# This module defines
#  INOTIFY_INCLUDE_DIR, where to find inotify.h, etc.
#  INOTIFY_FOUND, If false, do not try to use inotify.
# also defined, but not for general use are
#  INOTIFY_LIBRARY, where to find the inotify library.

#<<<<<<< HEAD
##find_path(INOTIFY_INCLUDE_DIR sys/inotify.h ${CMAKE_ARCH_TRIPLET}/sys/inotify.h)
#find_path(INOTIFY_INCLUDE_DIR NAMES sys/inotify.h ${CMAKE_LIBRARY_ARCHITECTURE}/sys/inotify.h)
##find_path(INOTIFY_INCLUDE_DIR NAMES sys/inotify.h ${CMAKE_ARCH_TRIPLET}/sys/inotify.h x86_64-linux-gnu/sys/inotify.h)
#mark_as_advanced(INOTIFY_INCLUDE_DIR)

##set(INOTIFY_INCLUDE_DIR "/usr/include/x86_64-linux-gnu/")
##set(INOTIFY_FOUND TRUE)

## handle the QUIETLY and REQUIRED arguments and set INOTIFY_FOUND to TRUE if
#=======
find_path(INOTIFY_INCLUDE_DIR sys/inotify.h 
          HINTS /usr/include/${CMAKE_LIBRARY_ARCHITECTURE})
mark_as_advanced(INOTIFY_INCLUDE_DIR)

#>>>>>>> master
# all listed variables are TRUE
# handle the QUIETLY and REQUIRED arguments and set INOTIFY_FOUND to TRUE if
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(INOTIFY DEFAULT_MSG INOTIFY_INCLUDE_DIR)


IF(INOTIFY_FOUND)
  SET(INotify_INCLUDE_DIRS ${INOTIFY_INCLUDE_DIR})
  MESSAGE("-- INotify Found.")
ELSE()
  set(INOTIFY_INCLUDE_DIR "/usr/include/x86_64-linux-gnu/")
  set(INOTIFY_FOUND TRUE)
  SET(INotify_INCLUDE_DIRS ${INOTIFY_INCLUDE_DIR})
  MESSAGE("-- INotify FORCED in !!!")
ENDIF(INOTIFY_FOUND)

