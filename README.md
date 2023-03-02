<h1 align="center">
    人脸识别考勤系统服务端
</h1>

## 🚀 快速开始

> **Note**
>
> 为了使用 format 格式化 SQL 语句，C++ 版本设置为 20

### 环境

- Visual Studio 2019
- Windows 10
- Cmake 3.20.21032501-MSVC_2
- Ninja 1.10.2
- MySQL 5.7.40
- Redis 7.0.5

### 拉取代码

```bash
git clone https://github.com/Face-recognition-punching-system/QFaceServer.git
```

### 构建

通过 Visual Studio 构建

## 🌲 目录树

```text
.
└───src
    │   main.cpp
    │
    ├───arcface
    │       detector.cpp
    │       detector.h
    │       detector_handler.cpp
    │       detector_handler.h
    │       face.h
    │       Facenet.cpp
    │       Facenet.h
    │       facenet_handler.cpp
    │       facenet_handler.h
    │       onet.cpp
    │       onet.h
    │       pnet.cpp
    │       pnet.h
    │       rnet.cpp
    │       rnet.h
    │       utils.h
    │
    ├───config
    │       config.h
    │    
    ├───model
    │       det1.caffemodel
    │       det1.prototxt
    │       det2.caffemodel
    │       det2.prototxt
    │       det3.caffemodel
    │       det3.prototxt
    │       facenet.pb
    │       facenet.pbtxt
    │
    ├───server
    │       admin_http_handler.cpp
    │       admin_http_handler.h
    │       database.cpp
    │       database.h
    │       database_pool.cpp
    │       database_pool.h
    │       http_handler_factory.cpp
    │       http_handler_factory.h
    │       http_server.cpp
    │       http_server.h
    │       intercept_http_handler.cpp
    │       intercept_http_handler.h
    │       logger.cpp
    │       logger.h
    │       redis_pool.cpp
    │       redis_pool.h
    │       utils.cpp
    │       utils.h
    │       video_http_handler.cpp
    │       video_http_handler.h
    │       worker_http_handler.cpp
    │       worker_http_handler.h
    │
```

## 依赖安装

### OpenCV

```bash
wget https://github.com/opencv/opencv/archive/refs/tags/4.5.5.zip
unzip 4.5.5.zip -d 4.5.5
wget https://www.raoyunsoft.com/opencv/opencv_contrib/opencv_contrib-4.5.5.zip
unzip opencv_contrib-4.5.5.zip -d opencv_contrib-4.5.5
cd 4.5.5
mkdir build
cd build
cmake \
  -D CMAKE_BUILD_TYPE=Release \
  -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-4.5.5/modules \
  ..
cmake --build . --config Release -j 2
cmake --build . --config Release --target install
```

### Poco

```bash
git clone -b master https://github.com/pocoproject/poco.git
cd poco
mkdir cmake-build
cd cmake-build
cmake ..
cmake --build . --config Release -j 2
cmake --build . --config Release --target install
```

### hiredis

```bash
git clone https://github.com/redis/hiredis.git
cd hiredis
mkdir build
cd build
cmake ..
cmake --build . --config Release -j 2
cmake --build . --config Release --target install
```

### redis-plus-plus

```bash
git clone https://github.com/sewenew/redis-plus-plus.git
cd redis-plus-plus
mkdir build
cd build
cmake ..
cmake --build . --config Release -j 2
cmake --build . --config Release --target install
```
