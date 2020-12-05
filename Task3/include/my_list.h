#ifndef BASICKNOWLEDGE_TASK3_MY_LIST_H
#define BASICKNOWLEDGE_TASK3_MY_LIST_H

#include <vector>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <grp.h>
#include <pwd.h>

using std::vector;
using std::unordered_map;
using std::string;

const int FILE_PATH_MAX_SIZE = 255;

class MyList {
    public:
        MyList();
        ~MyList();
        MyList(char * = nullptr, const int & = 0);
        bool Start();
    private:
        char *option_;  // 存储可能的选项
        int option_numbers_;  // 选项的数量
        unordered_map<char, bool> option_exist_;  // 检索是否存在选项

        // 展示选项
        void ShowOption(); 
        // 显示文件目录下的文件名和文件目录名
        int List();        
        // 以列表的方式显示文件的类别、权限、用户、组用户、时间、文件名等信息
        int ListAsList();  
        // 向标准输出打印文件的各项信息
        void PrintStat(struct stat, char *); 
        // 获取文件的类型
        char GetType(mode_t);  
        // 获取文件的使用权限
        char *GetPerm(mode_t); 
        // 获取文件的用户名
        char *GetUserName(uid_t);  
        // 获取文件的组用户信息 
        char *GetGroupName(gid_t); 
        // 获取文件的最后修改时间
        string GetTime(const time_t *);  
};

#endif