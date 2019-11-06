AUX_SOURCE_DIRECTORY(. APP_SRC)
ADD_EXECUTABLE(simhash ${APP_SRC})

INCLUDE_DIRECTORIES(
    ${SOURCE_DIR}/external/smhasher
    ${SOURCE_DIR}/external/cppjieba/include
    ${SOURCE_DIR}/external/cppjieba/deps
)

target_link_libraries(simhash SMHasherSupport glog)
