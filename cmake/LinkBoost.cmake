###################
# LinkBoost.cmake #
###################
message(${APP_NAME})
message(${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(${APP_NAME} ${Boost_LIBRARIES})

IF("${CMAKE_SYSTEM}" MATCHES "Linux")
  TARGET_LINK_LIBRARIES(${APP_NAME} pthread rt)
ENDIF()
