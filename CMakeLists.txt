cmake_minimum_required (VERSION 3.8)

project ("QFaceServer")

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV 4 REQUIRED)
find_package(Poco REQUIRED COMPONENTS Util Net NetSSL JSON)
find_package(Boost REQUIRED)
find_path(MYSQL_ROOT_DIR mysql)
find_path(HIREDIS_HEADER hiredis)
find_library(HIREDIS_LIB hiredis)
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)
 
include_directories(${MYSQL_ROOT_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

file(GLOB ARCFACE_FILES "src/arcface/*")
file(GLOB CONFIG_FILES "src/config/*")
file(GLOB SERVER_FILES "src/server/*")

configure_file("src/model/det1.caffemodel" "model/det1.caffemodel" COPYONLY)
configure_file("src/model/det1.prototxt" "model/det1.prototxt" COPYONLY)
configure_file("src/model/det2.caffemodel" "model/det2.caffemodel" COPYONLY)
configure_file("src/model/det2.prototxt" "model/det2.prototxt" COPYONLY)
configure_file("src/model/det3.caffemodel" "model/det3.caffemodel" COPYONLY)
configure_file("src/model/det3.prototxt" "model/det3.prototxt" COPYONLY)
configure_file("src/model/facenet.pb" "model/facenet.pb" COPYONLY)
configure_file("src/model/facenet.pbtxt" "model/facenet.pbtxt" COPYONLY)
 
add_executable ( 
	${PROJECT_NAME}
	"src/main.cpp"  
	${SERVER_FILES}
	${ARCFACE_FILES}
	${CONFIG_FILES} 
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} PRIVATE
	Poco::Util
	Poco::Net  
	Poco::NetSSL 
	Poco::JSON)
target_link_libraries(${PROJECT_NAME} PRIVATE ${MYSQL_ROOT_DIR}/../lib/libmysql.lib)
target_include_directories(${PROJECT_NAME} PUBLIC ${HIREDIS_HEADER})
target_link_libraries(${PROJECT_NAME} PRIVATE ${HIREDIS_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC ${REDIS_PLUS_PLUS_HEADER})
target_link_libraries(${PROJECT_NAME} PRIVATE ${REDIS_PLUS_PLUS_LIB})
link_directories(${OpenCV_LIBRARY_DIRS})

if(CMAKE_BUILD_TYPE MATCHES "Release")  
else() 
endif()

find_package(OpenSSL)

if(OPENSSL_FOUND)
  message(STATUS "OPENSSL_VERSION = ${OPENSSL_VERSION}")
  message(STATUS "OPENSSL_SSL_LIBRARY = ${OPENSSL_SSL_LIBRARY}")
  message(STATUS "OPENSSL_CRYPTO_LIBRARY = ${OPENSSL_CRYPTO_LIBRARY}")
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
