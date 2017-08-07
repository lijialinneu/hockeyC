project( ReadVideo )
find_package( OpenCV REQUIRED )
add_executable( ReadVideo ReadVideo )
target_link_libraries( ReadVideo ${OpenCV_LIBS} )