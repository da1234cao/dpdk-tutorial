

[toc]

# 使用

相关链接：
- [3. Command Line Sample Application — Data Plane Development Kit 25.07.0-rc1 documentation](https://doc.dpdk.org/guides/sample_app_ug/cmd_line.html) 。
- [2. 命令行库 — Data Plane Development Kit 25.07.0-rc1 文档 --- 2. Command-line Library — Data Plane Development Kit 25.07.0-rc1 documentation](https://doc.dpdk.org/guides/prog_guide/cmdline.html)

```shell
example> help
Demo example of command line interface in RTE

This is a readline-like interface that can be used to
debug your RTE application. It supports some features
of GNU readline like completion, cut/paste, and some
other special bindings.

This demo shows how rte_cmdline library can be
extended to handle a list of objects. There are
3 commands:
- add obj_name IP
- del obj_name
- show obj_name

example> add 
 [Any STRING]: Add an object (name, val)
example> add 
Bad arguments
example> add addr1 1.1.1.1
Object addr1 added, ip=1.1.1.1
example> show 
 addr1 [Obj-List]: Show/del an object
example> show 
Bad arguments
example> show addr1
Object addr1, ip=1.1.1.1
example> del 
 addr1 [Obj-List]: Show/del an object
example> clear
Command not found
example> show 
 addr1 [Obj-List]: Show/del an object
example> show 
```

总的来说，看起来还行，但是真写起来可能会有点费劲。

dpdk 的 cmdline 中的 token 解析，似乎是顺序解析。每个 token ,可以自定义解析方式。库里面已经支持的 token 解析方式，包含 Strings、Signed/unsigned 16/32/64-bit integers、IP Addresses、Ethernet Addresses。

大概是这么回事，具体的使用，可能还得看更多的示例。

## 基本使用

1. Define the result structure for the command, specifying the command parameters

```c
struct cmd_obj_add_result {
  cmdline_fixed_string_t action;
  cmdline_fixed_string_t name;
  cmdline_ipaddr_t ip;
};
```

2. Provide an initializer for each field in the result

```c
cmdline_parse_token_string_t cmd_obj_action_add =
    TOKEN_STRING_INITIALIZER(struct cmd_obj_add_result, action, "add");
cmdline_parse_token_string_t cmd_obj_name =
    TOKEN_STRING_INITIALIZER(struct cmd_obj_add_result, name, NULL);
cmdline_parse_token_ipaddr_t cmd_obj_ip =
    TOKEN_IPADDR_INITIALIZER(struct cmd_obj_add_result, ip);
```

3. Define the callback function for the command

```c
static void cmd_obj_add_parsed(void *parsed_result, struct cmdline *cl,
                               __rte_unused void *data) {
                               ....
                               }
```

4. Provide a parse result structure instance for the command, linking the callback to the command

```c
cmdline_parse_inst_t cmd_obj_add = {
    .f = cmd_obj_add_parsed, /* function to call */
    .data = NULL,            /* 2nd arg of func */
    .help_str = "Add an object (name, val)",
    .tokens =
        {
            /* token list, NULL terminated */
            (void *)&cmd_obj_action_add,
            (void *)&cmd_obj_name,
            (void *)&cmd_obj_ip,
            NULL,
        },
};
```

5. Add the parse result structure to a command-line context

```c
cmdline_parse_ctx_t main_ctx[] = {
    (cmdline_parse_inst_t *)&cmd_obj_del_show,
    (cmdline_parse_inst_t *)&cmd_obj_add,
    (cmdline_parse_inst_t *)&cmd_help,
    NULL,
};
```

6. Within your main application code, create a new command-line instance passing in the context.

```c
cl = cmdline_stdin_new(main_ctx, "example> ");
	if (cl == NULL)
		rte_panic("Cannot create cmdline instance\n");
	cmdline_interact(cl);
	cmdline_stdin_exit(cl);
```

这个定义过程可以借助 `dpdk-cmdline-gen.py` 。但是我要是需要定义cmdline的话，我肯定找个类似的改改，省的看脚本使用方式。

## 自定义结构

示例中有自定义的解析函数。感觉基本的已经够用了，似乎不必自定义解析函数。

不管啥输入，都可以考虑放入字符串，然后在回调函数里面解析就是了，单独写一套解析函数怪麻烦了。

但是示例有了，我还是看了下。它大致是填充了这个结构。

```c
struct cmdline_token_ops {
	/** parse(token ptr, buf, res pts, buf len) */
	int (*parse)(cmdline_parse_token_hdr_t *, const char *, void *,
		unsigned int);
	/** return the num of possible choices for this token */
	int (*complete_get_nb)(cmdline_parse_token_hdr_t *);
	/** return the elt x for this token (token, idx, dstbuf, size) */
	int (*complete_get_elt)(cmdline_parse_token_hdr_t *, int, char *,
		unsigned int);
	/** get help for this token (token, dstbuf, size) */
	int (*get_help)(cmdline_parse_token_hdr_t *, char *, unsigned int);
};
```

- parse 是从当前token中解析内容，并填入的结果中。成功时返回解析的字符数；失败时返回负值。
- complete_get_nb 获取当前 token 可能的补全选项数量。
- complete_get_elt 将第 i 个补全选项复制到 dstbuf 中。
- get_help 将当前 token 的帮助信息写入 dstbuf。


# 最后

1. 项目集成cmdline是很好的，它比配置更加灵活。
2. 但是，不应该让项目本身在源码层面集成cmdline。cmdline 的代码不好读，不好维护。通过外部cmdline + RPC 的方式，可以解耦项目本身和cmdline。
    * 外部 cmdline 提供CLI。
    * RPC提供接口函数，供cmdline调用。但是，在C中，知名的RPC库很少。可以尝试下 [apache/thrift: Apache Thrift --- apache/thrift: Apache Thrift](https://github.com/apache/thrift?tab=readme-ov-file) 
    * 找个C语言的RPC库非常费劲。可以尝试在程序内部集成一个RESTful服务,用来对外提供接口。但是一旦使RESTful服务，就得考虑鉴权问题，避免外部恶意的利用接口。(127.0.0.1也会有问题。比如部署在容器中，有网络隔离。)
    * 可以考虑部署的时候，带一个数据库。动态的从数据库中读取配置，但是数据库又太重了。
    * 算了，还是不要cmdline了，直接改完配置，重启得了。