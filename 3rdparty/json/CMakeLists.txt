if( TARGET nlohmann_json )
  return()
endif()

if(NOT NLOHMANNJSON_ROOT)
  set(NLOHMANNJSON_ROOT "${PROJECT_SOURCE_DIR}/lib/json")
endif()

add_library( nlohmann_json INTERFACE )
target_include_directories(
  nlohmann_json
  INTERFACE
    ${NLOHMANNJSON_ROOT}/include
)