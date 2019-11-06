# set(BUILD_STATIC_LIBS YES CACHE BOOL "")
# set(BUILD_SHARED_LIBS NO CACHE BOOL "")

# 加载依赖库
add_subdirectory(smhasher)
add_subdirectory(cppjieba)
add_subdirectory(glog)