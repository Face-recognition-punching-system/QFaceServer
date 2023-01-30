# CMakeList.txt: QFaceServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("QFaceServer")

find_package(OpenCV 4 REQUIRED)
find_package(Poco  REQUIRED COMPONENTS MongoDB Redis Util Net NetSSL JSON)
find_package(Boost REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB ARCFACE_FILES "src/arcface/*")
file(GLOB CONFIG_FILES "src/config/*")
file(GLOB SERVER_FILES "src/server/*")

add_executable (
	QFaceServer
	"src/main.cpp" 
	${ARCFACE_FILES}
	${CONFIG_FILES}
	${SERVER_FILES}
)

target_link_libraries(QFaceServer PRIVATE ${OpenCV_LIBS})
target_link_libraries(QFaceServer PRIVATE
	Poco::MongoDB
	Poco::Redis
	Poco::Net
	Poco::NetSSL
	Poco::JSON)

link_directories(${OpenCV_LIBRARY_DIRS})

if(CMAKE_BUILD_TYPE MATCHES "Release")
else()
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux ")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
	MESSAGE(STATUS "current platform: Windows")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	MESSAGE(STATUS "current platform: FreeBSD")
else()
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
