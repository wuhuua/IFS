# Socket设计方面
服务采取异步I/O的方式处理客户端请求

## 问题处理
对于重启频繁的场景下可能会出现下面的错误：
```shell
bind failed: Address already in use
server terminated
```
这主要是因为TCP连接的双方当一方主动关闭连接，在接收到对端的 FIN 报文之后，主动关闭连接的一方会在 TIME_WAIT 这个状态里停留一段时间，这个时间大约为 2MSL。
默认情况下我们不能在处于TIME_WAIT状态的连接上重用套接字
因此使用`SO_REUSEADDR`的套接字选项，允许启动绑定在一个端口，即使之前存在一个和该端口一样的连接。

> *这个套接字选项通知内核，如果端口忙，但TCP状态位于 TIME_WAIT ，可以重用端口。如果端口忙，而TCP状态位于其他状态，重用端口时依旧得到一个错误信息，指明"地址已经使用中"。如果你的服务程序停止后想立即重启，而新套接字依旧使用同一端口，此时SO_REUSEADDR 选项非常有用。必须意识到，此时任何非期望数据到达，都可能导致服务程序反应混乱，不过这只是一种可能，事实上很不可能。*

在将套接字绑定到连接之前，使用`setsockopt`进行设置：
```c
int on = 1;
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
```