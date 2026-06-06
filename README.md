# get_next_line

A C function that reads a file **one line at a time** from a file descriptor, no matter how large or small the read buffer is. Each call returns the next line; repeated calls walk through the whole file.

```c
char *get_next_line(int fd);
```

Returns the next line **including the trailing `\n`** (if there is one), or `NULL` at end of file or on error. The caller is responsible for freeing each returned line.

## The challenge

`read()` doesn't know anything about lines — it just hands you a fixed-size block of bytes. The job here is to build a clean line-by-line reader on top of it, which means:

- buffering bytes that were read past the end of the current line,
- **remembering that leftover between calls**, and
- working correctly for *any* read size, from `BUFFER_SIZE=1` to a buffer larger than the whole file.

## How it works

A `static` pointer (the **stash**) holds whatever was read beyond the previous line.

1. **Read loop** — read `BUFFER_SIZE` bytes at a time and append each chunk to the stash, stopping as soon as a `\n` is in the stash or `read()` hits EOF.
2. **Extract** — copy from the start of the stash up to and including the first `\n` (or to the end if there's no newline). That's the returned line.
3. **Update** — keep everything *after* that `\n` in the stash for the next call; if nothing is left, the stash is freed and reset.

Supporting helpers (`ft_strlen`, `ft_memcpy`, `ft_strchr`, `gnl_subdup`) keep the core logic small. `BUFFER_SIZE` defaults to `42` and can be overridden at compile time.

## Bonus: multiple file descriptors

The bonus version replaces the single stash with an array indexed by file descriptor (`static char *st[FD_MAX]`). This lets you read from **several files interleaved** — alternating `get_next_line()` calls on different fds — without their leftovers ever getting mixed up.

## Build & use

`get_next_line` is compiled together with the program that uses it. Set the read buffer size with `-D BUFFER_SIZE`:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   your_program.c get_next_line.c get_next_line_utils.c -o your_program
```

Example:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int   fd = open("file.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

## Verified

- Compiles clean with `-Wall -Wextra -Werror`.
- Identical output for `BUFFER_SIZE` 1, 42, and values larger than the file.
- Handles empty files and files with no trailing newline.
- Bonus: reads multiple file descriptors in parallel without mixing their data.
- No memory leaks (checked with Valgrind on both versions).

---

# get_next_line

一个 C 函数，从文件描述符里**逐行读取**文件内容，无论读取缓冲区设多大或多小都能正确工作。每次调用返回下一行，连续调用即可遍历整个文件。

```c
char *get_next_line(int fd);
```

返回下一行（**包含末尾的 `\n`**，如果有的话），到文件末尾或出错时返回 `NULL`。返回的每一行需要调用者自己 `free`。

## 难点

`read()` 本身不认识「行」的概念——它只给你固定大小的一块字节。本项目要在它之上实现一个干净的逐行读取器，这意味着要：

- 缓存「读过头」、超出当前行的那部分字节；
- **在多次调用之间记住这些剩余数据**；
- 对任意读取大小都正确，从 `BUFFER_SIZE=1` 到比整个文件还大的缓冲区都要能跑。

## 实现思路

用一个 `static` 指针（称为 **stash**）保存上一行之后多读出来的内容。

1. **读取循环**：每次读 `BUFFER_SIZE` 字节并拼接到 stash，直到 stash 中出现 `\n` 或 `read()` 到达文件末尾。
2. **提取行**：从 stash 开头复制到第一个 `\n`（含）为止（若无换行则复制到结尾），作为返回的行。
3. **更新 stash**：把该 `\n` 之后的内容留在 stash 里供下次调用；若没有剩余，则释放并重置 stash。

辅助函数（`ft_strlen`、`ft_memcpy`、`ft_strchr`、`gnl_subdup`）让核心逻辑保持精简。`BUFFER_SIZE` 默认为 `42`，可在编译时覆盖。

## Bonus：多文件描述符

bonus 版本把单个 stash 换成按文件描述符索引的数组（`static char *st[FD_MAX]`），这样就能**交替读取多个文件**——在不同 fd 上轮流调用 `get_next_line()`——而各自的剩余数据互不串扰。

## 编译与使用

`get_next_line` 需要和调用它的程序一起编译。用 `-D BUFFER_SIZE` 指定读取缓冲区大小：

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   your_program.c get_next_line.c get_next_line_utils.c -o your_program
```

示例：

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int   fd = open("file.txt", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

## 已验证

- `-Wall -Wextra -Werror` 编译零警告。
- `BUFFER_SIZE` 取 1、42、以及大于文件大小时输出一致。
- 正确处理空文件、以及无换行结尾的文件。
- Bonus：并行读取多个文件描述符且数据互不混淆。
- 无内存泄漏（两个版本均通过 Valgrind 检查）。
