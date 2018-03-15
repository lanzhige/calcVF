file(GLOB hdrs "*.h")
file(GLOB srcs "*.cpp")

add_executable(calcSVF ${hdrs} ${srcs})
#target_link_libraries(calcSVF ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
target_link_libraries(calcSVF OpenGL32.lib soil2.lib)
install(TARGETS calcSVF DESTINATION .)