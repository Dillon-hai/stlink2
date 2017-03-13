set (STLINK2_CMD "stlink2")

set (STLINK2_CMD_SOURCE
	"main.c"
)

if (STLINK2_HAS_LUA)
	add_definitions(-DSTLINK2_HAS_LUA=1)
endif()

add_executable(${STLINK2_CMD} ${STLINK2_CMD_SOURCE})
add_dependencies(${STLINK2_CMD} ${STLINK2_LIBRARY_STATIC} liblua)
target_link_libraries(${STLINK2_CMD} ${STLINK2_LIBRARY_STATIC})

if (STLINK2_HAS_LUA)
	add_dependencies(${STLINK2_CMD} liblua)
	target_link_libraries(${STLINK2_CMD} ${LUA_LIBRARY_STATIC} ${STLINK2_LUA_LIBRARY_STATIC} m)
endif()

install (TARGETS
	${STLINK2_CMD}
	DESTINATION bin
)
