#  IFS

## :black_nib: 基于Ext2的虚拟化分布式文件系统:black_nib:

### ![brightgreen](https://img.shields.io/badge/-轻量级-brightgreen)![important](https://img.shields.io/badge/-Linux-important)![blueviolet](https://img.shields.io/badge/-c_only-blueviolet)![informational](https://img.shields.io/badge/-异步IO-informational)![red](https://img.shields.io/badge/-中文-red)

![Language](https://img.shields.io/badge/language-c-brightgreen)

![Documentation](https://img.shields.io/badge/documentation-yes-brightgreen)

ITrans是基于linux内核的Ext2文件系统实现的虚拟系统，内部数据结构和文件组织形式与Ext2保持一致

作为一个类似于Ext2 的文件系统，应该提供诸如下相关操作。

| 操作命令 | 参数             | 功能                       |
| -------- | ---------------- | -------------------------- |
| format   | 无               | 格式化文件系统             |
| password | 无               | 修改文件系统的密码         |
| ls       | 无               | 列举当前目录的文件（目录） |
| create   | 文件类型，文件名 | 在当前目录创建文件(目录)   |
| delete   | 文件类型，文件名 | 在当前目录删除文件(目录)   |
| cd       | 目录名           | 切换目录                   |
| close    | 目录深度         | 按照目录深度关闭目录       |
| read     | 文件名           | 从文件中读取数据           |
| write    | 文件名           | 向文件中写入数据           |


## :whale:下载须知 :feet:

```shell
git clone ${url}
```

## :articulated_lorry: 环境准备

无论在任何环境上编译，请确保你的环境拥有以下工具链：

```shell
gcc/clang gcc建议8.3以上
```


## :alarm_clock: 如何编译
同时编译客户端和服务端：
```shell
make
```
只编译客户端：
```shell
make client
```
其中客户端与服务端完全不耦合，客户端源码只有`src/client.c`文件，因此也可以单独使用`gcc -o ifsClient src/client.c`来编译客户端

清除.o文件：
```shell
make clean
```

## :alarm_clock: 如何运行
### 本地运行
不使用任何参数即可启用本地命令行
```shell
./ifsServer
```

### 服务端配置
加入-i参数指定部署服务器(默认8080端口)
```shell
./ifsServer -i
```
更加详细的配置，指定端口号、通信缓冲区大小，最大连接数
```shell
./ifsServer -i -p 8080 -b 1024 -c 10
```
### 客户端配置
指定目标服务器ip和端口即可
```shell
./ifsClient -t 127.0.0.1 -p 8080
```


## :memo:实现备注

- socket: [网络编程实现细节](./docs/socket.md)


## :sparkling_heart:License

IFS 采用 [木兰宽松许可证，第2版](https://license.coscl.org.cn/MulanPSL2), 可以自由拷贝和使用源码, 当做修改或分发时, 请遵守 [木兰宽松许可证，第2版](https://license.coscl.org.cn/MulanPSL2).

![License](https://img.shields.io/badge/license-MuLan-yellow)



Copyright :copyright:2023 wuhuua

