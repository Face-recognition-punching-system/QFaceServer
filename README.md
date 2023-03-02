# 人脸识别考勤系统服务端

## 如何开始

> **Note**
>
> 为了使用 format 格式化 SQL 语句，C++ 版本设置为 20

## 环境

- Visual Studio 2019
- Windows 10
- Cmake 3.25.2

## 拉取代码

```bash
git clone https://github.com/Face-recognition-punching-system/QFaceServer.git
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
cmake --build . --config Release
```

### hiredis

```bash
git clone https://github.com/redis/hiredis.git
cd hiredis
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### redis-plus-plus

```bash
git clone https://github.com/sewenew/redis-plus-plus.git
cd redis-plus-plus
mkdir build
cd build
cmake ..
cmake --build . --config Release
```
