# dpdk-tutorial

```shell
# 准备一个pyenv环境，安装下面依赖
python3 -m pip install -i https://pypi.tuna.tsinghua.edu.cn/simple --upgrade pip
pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple
pip install msgpack scapy PrettyTable pyelftools Nuitka zstandard ptf

meson setup build && cd build
meson compile
```

## dpdk的使用 - step by step

- [hello-world](./hello-world/)
