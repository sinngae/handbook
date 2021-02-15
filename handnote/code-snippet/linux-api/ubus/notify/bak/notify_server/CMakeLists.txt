cmake_minimum_required(VERSION 2.8.7)

project(server)

set(CMAKE_C_FLAGS "-Wno-error=format-security -DONECLOUD -D_FILE_OFFSET_BITS=64 -DLIMIT_TAG -DUBUS -DIOS11 -fPIC -pipe -I/opt/local/dep/include -I/opt/local/dep/include/utils ${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-L/opt/local/dep/lib ${CMAKE_EXE_LINKER_FLAGS}")

file(GLOB SOURCES *.c)
add_executable(${PROJECT_NAME} ${SOURCES})
#target_link_libraries(${PROJECT_NAME} heifapi ev json-c cJSON vorbis ogg id3tag exif FLAC jpeg iconv pthread dl sqlite3
#  m z xlog volume devid zlog png crypto ssl ubus config ubus_app ubox md5sum_calc
#  curl xpinyin pcre heifreader common avcparser h265parser json  png16 gmodule-2.0
#  expat pangoft2-1.0 pango-1.0 fontconfig gobject-2.0 glib-2.0 freetype vips  vips-cpp vipsCC stdc++ ffi gthread-2.0 harfbuzz
#  ${AVFORMAT_LIB} ${AVCODEC_LIB} ${AVFILTER_LIB} ${AVUTIL_LIB} ${AVDEVICE_LIB} ${SWRESAMPLE_LIB} ${SWSCALE_LIB})
target_link_libraries(${PROJECT_NAME} json-c ubus ubox pthread )
