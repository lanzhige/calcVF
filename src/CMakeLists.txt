
file(GLOB hdrs "*.h"
	"./fisheye/*.h")
file(GLOB srcs "*.cpp")

add_executable(calcVF ${hdrs} ${srcs})
#target_link_libraries(calcVF ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})

target_link_libraries(calcVF OpenGL32.lib Glu32.lib libglew32.lib glfw3.lib soil2.lib zlib.lib)
install(TARGETS calcVF DESTINATION .)