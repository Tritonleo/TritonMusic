include("C:/Users/25764/Desktop/TritonMusic/build/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/TritonMusic-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase;qtmultimedia")

qt6_deploy_runtime_dependencies(
    EXECUTABLE C:/Users/25764/Desktop/TritonMusic/build/TritonMusic.exe
    GENERATE_QT_CONF
)
