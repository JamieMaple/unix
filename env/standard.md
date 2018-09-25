# Unix 标准及其实现

### 一些专业名词

### 限制

1. 编译时限制

2. 运行时限制

`sysconf` `pathconf` `fpathconf`

### 选项 -- 针对可移植的程序

1.  编译时选项定义在 `<unistd.h>`

2. 与文件或目录无关的运行时选项用 `sysconf` 函数来判断

3. 与文件或目录有关的运行时选项通过调用 `pathconf` 或 `fpathconf` 函数来判断

