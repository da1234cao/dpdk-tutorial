# 项目的构建

首先是准备python环境。

```shell
## 安装下面依赖
python3 -m pip install -i https://pypi.tuna.tsinghua.edu.cn/simple --upgrade pip
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
pip install msgpack scapy PrettyTable pyelftools Nuitka zstandard ptf
```

然后是安装一个些必要的依赖。

```shell
dependency: gcc, g++, clang, ....
```

接着开始构建这个项目。这个项目会自动拉取和构建DPDK。

```shell
meson setup build
# meson setup --reconfigure  build
# meson configure -Denable_subs='' -Dbuildtype=debug build
cd build
meson compile
```

## dpdk的使用 - step by step

示例代码基本来自：[Sample Applications User Guides — Data Plane Development Kit](https://doc.dpdk.org/guides/sample_app_ug/index.html)

- [hello-world](./hello-world/)
- [env-prepare](./env-prepare/)
- [skeleton](./skeleton/)
- [commands](./cmdline/)
- [rx-tx-callback](./rx-tx-callback/)
- [multi-process-msg](./multi-process-msg/)
- [flow](./flow/)
- [rcu](./rcu/)
- [trace](./trace/)
- [ip_fragmentation](./ip_fragmentation/)
