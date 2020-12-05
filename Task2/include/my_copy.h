/*
    为什么类定义要放在.h文件，类实现放在cpp文件?
    一、前置知识：
        1. 声明与定义：
            1.1 声明：
                    告诉编译器相关的符号的存在，这个符号是在其他文件中出现过的，链接的时候由编译器在别的目标文件中寻找；
                定义：
                    定义把一个符号完完整整地描述出来，它是变量还是函数，需要哪些参数、返回值是什么等信息。
            1.2 仅仅是声明的情况：
                - void func(int, int);
                - extern int a;
                - class A;
                - typedef;
                - 静态数据成员定义: public: static int a;
            1.3 仅仅是定义的情况：
                - 静态数据成员定义：A::a
                - 函数的实现
            1.4 既是声明也是定义：
                - int a;
                - extern int b = 1;
            1.5 区别：
                - 声明可以有多次，但是定义只能有一次。
                - except：类内，成员函数 和 静态数据成员 声明 只能有一次。
        2. C++ 编译方式：
            2.1 分别编译：
                .cpp中的内容都是相对独立的，编译时，并不要求文件都是互通的(但是要有声明)，只要在编译成目标文件后，将相关的目标文件链接到一起即可运行。
            例如：a.cpp 由一个函数定义了void A(){}，b.cpp 若要使用，只需要声明该函数 void A();，即可使用A这个函数。因为，编译器在编译b.cpp时，会产
            生一个符号表，符号表包含了b.cpp中声明而没定义的符号，如A()，再进行链接的时候，编译器会到别的目标文件中寻找相关符号的定义。
            2.2 头文件：
                我们把声明别的.cpp中函数的行为称为 引用。
                如果一个.cpp要引用的函数特别多，引用时需要知道函数的形式，而想要记住所有函数的形式，读于程序员来说是一件很困难的事情。
                这时，如果知道我们的程序a.cpp想要引用的函数A()，A()所在的x.cpp是哪一个(现实中程序员也必须知道知道想要使用的函数在哪个.cpp文件中，准确说是.h文件)
            那么在引用这个函数A()时，若x.cpp提供了相应的头文件x.h，那我们就不需要再去引用声明A()，而只需要在a.cpp中添加一行 #include<x.h>，就可以声明相应的A()。
            更具体地说，因为#include<x.h>会将该.h文件内的所有内容都复制到我们的文件a.cpp中，而x.h中包含了x.cpp文件中所有函数的声明。因此,相当于我们的a.cpp
            声明了A()函数。
                这样子，我们就可以通过 #include<x.h> 来声明需要的函数，一个简单的问题就是，x.h中的函数我们不一定都会使用，若x.h中包含了大量其他的函数声明，将导致
            我们的a.cpp出现大量多余的声明。
            2.3 头文件内容：
                在一个项目中，若出现了两个.cpp引用相同的.h文件，若.h文件中，出现过定义A的话，那么这个定义A也将随着.cpp文件的编译而被编译，这样将导致A被定义了两次，
            编译器就不知道该使用哪个定义，即便两个定义是完全相同的。(多cpp的重定义情况)
                由此可知，头文件应该存放：
                    - 变量或者函数的声明
                    - except： // https://www.runoob.com/w3cnote/cpp-header.html
                        - const全局对象
                        - 内联函数
                        - 类定义
                    - 需要知道，上述的例外情况，只是防止多个.cpp间发生重定义，下面看看单个.cpp的重定义情况：
                        - 引用头文件的定义与本文件的定义发生冲突，即便const全局对象是非extern的 (只能靠自己的命名解决冲突)
                        - 引用的头文件不止一个，且头文件内容导致了重定义 (参见2.4)
            2.4 头文件的预编译问题：
                在a.cpp中，需要使用B和C类，而B类和C类，分别在b.h与c.h中定义，其中B类依赖C类，所以，b.h引用了c.h，这样当a.cpp在编译时，C的定义将会出现两次，导致
            编译出错。(单cpp的重定义情况)
                解决方法：
                    - 使用条件编译#define，使用方法，见下代码
    二、 https://blog.csdn.net/lwb102063/article/details/52068212
*/


#ifndef BASICKNOWLEDGE_TASK2_MY_COPY_H
#define BASICKNOWLEDGE_TASK2_MY_COPY_H

#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>

using std::string;

class MyCopy {
    private:
        char *source_path_;
        char *target_path_;
        string source_file_name_;
        string target_directory_name_;

        // 检查源路径是否存在，是文件还是目录等功能
        int CheckPath(const char * const &);
        // 获取文件的名称
        int GetFileName(const char * const &);
        // 获取目录的名称
        int GetDiretoryName();
        // 复制文件到指定的文件名字，或覆盖指定的文件
        int CopyFileToFile(const string * = nullptr); // #@-@# 不能引用空对象，即const string & = nullptr，若对象为空，使用指针，同理空指针也不行，const string *& = nullptr
        // 复制文件并重命名
        int CopyFileAndRename();
        // 复制文件到指定的目录
        int CopyFileToDiretory(); 
    public:
        MyCopy();
        MyCopy(char * = nullptr, char * = nullptr);  // #@-@# 默认的缺省参数只能在声明(此处)，或者定义(.cpp中)两者中出现一次
        ~MyCopy();
        // 开启复制过程
        bool Start(); 
};

#endif