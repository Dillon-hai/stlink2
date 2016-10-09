###
# Library source
###
set (STLINK2_LIBRARY_SOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/stlink2.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/cortexm.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/log.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/usb.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/stm32.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/semihosting.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/utils/hexstr.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/utils/msleep.c"
	CACHE INTERNAL "stlink2 library source files"
)

###
# libusb
###
find_package(LibUSB REQUIRED)
include_directories(SYSTEM ${LIBUSB_INCLUDE_DIR})
set(LIBUSB_INCLUDE_DIR ${LIBUSB_INCLUDE_DIR} CACHE INTERNAL "libusb library headers")

###
# static library
###
set(STLINK2_LIBRARY_STATIC ${PROJECT_NAME}-static CACHE INTERNAL "stlink2 static library")

add_library(${STLINK2_LIBRARY_STATIC} STATIC ${STLINK2_LIBRARY_SOURCE})
set_target_properties(${STLINK2_LIBRARY_STATIC} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} PREFIX "")
target_link_libraries(${STLINK2_LIBRARY_STATIC} ${LIBUSB_LIBRARY})

###
# shared library
###
set(STLINK2_LIBRARY_SHARED ${PROJECT_NAME}-shared CACHE INTERNAL "stlink2 shared library")

add_library(${STLINK2_LIBRARY_SHARED} SHARED ${STLINK2_LIBRARY_SOURCE})
set_target_properties(${STLINK2_LIBRARY_SHARED} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} PREFIX "")
target_link_libraries(${STLINK2_LIBRARY_SHARED} ${LIBUSB_LIBRARY})

###
# library dependency linking
###
if (WIN32)
	target_link_libraries(${STLINK2_LIBRARY_STATIC} wsock32 ws2_32)
	target_link_libraries(${STLINK2_LIBRARY_SHARED} wsock32 ws2_32)
elseif (APPLE)
	find_library(ObjC objc)
	find_library(CoreFoundation CoreFoundation)
	find_library(IOKit IOKit)

	target_link_libraries(${STLINK2_LIBRARY_STATIC} ${CoreFoundation} ${IOKit} ${ObjC})
	target_link_libraries(${STLINK2_LIBRARY_SHARED} ${CoreFoundation} ${IOKit} ${ObjC})
endif ()

###
# Library version
###
if (WIN32 OR MSYS OR MINGW)
	set(STLINK2_LIBRARY_VERSION ${STLINK2_VERSION_MAJOR}.${STLINK2_VERSION_MINOR})
else ()
	set(STLINK2_LIBRARY_VERSION ${STLINK2_VERSION_MAJOR}.${STLINK2_VERSION_MINOR}.${STLINK2_VERSION_PATCH})
endif()

set_target_properties (${STLINK2_LIBRARY_SHARED}
	PROPERTIES SOVERSION ${STLINK2_VERSION_MAJOR}
	VERSION ${STLINK2_LIBRARY_VERSION}
)

###
# Install
###
install (TARGETS
	${STLINK2_LIBRARY_STATIC}
	${STLINK2_LIBRARY_SHARED}
	DESTINATION lib
)
