[toc]

# 关于本工程

旨在创建一个通用的C工程模板，支持C语言的C90标准，且使用CMAKE工具进行代码管理和编译。工程中包含一些常用的高级组件，比如用于快捷生成命令行处理的组件，用于日志管理的日志组件等。

组件分为第三方组件和内嵌组件，分别在3rd和libs中，区别在于3rd中的组件为单独的git子工程，本仓库中并不给出这些子工程的源代码，需要使用者在编译前执行更新子模块功能来单独下载这些第三方组件。

对于那些我们自己编写的针对某个功能的代码，可以封装成库以屏蔽内部实现，这种代码可以放在libs下的单独目录进行管理。