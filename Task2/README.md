# Task 2 ：仿写cp指令部分功能

## 1. 主要功能：
- (1) 源文件和目标文件通过命令行参数指定。
- (2) 将源文件复制到另外一个文件。
    - 例如：[test@linux test]$ ./mycp /home/test1.txt /home/tmp/test2.txt 
- (3) 将源文件复制到另外一个目录。
    - 例如：[test@linux test]$ ./mycp /home/test1.txt /tmp
    
## 2. 完善：
- ✓目标文件存在时，提示“是否覆盖”or“是否合并”等提示信息，并实现覆盖或者目标文件与源文件合并的功能（目标文件尾部追加写入）
- ✓源文件不存在时给出错误提示信息
- ✓源文件是目录时给出错误提示信息