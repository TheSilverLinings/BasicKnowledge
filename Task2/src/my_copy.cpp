#include <iostream>
#include "my_copy.h"

using std::cout;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;

MyCopy::MyCopy(){

}

MyCopy::~MyCopy(){
}

MyCopy::MyCopy(char *source_path, char *target_path) {
    source_path_ = source_path;
    target_path_ = target_path;
}

int MyCopy::CheckPath(const char * const & file_path) {
    // 1. 创建保存文件信息的结构体
    struct stat file_info;
    // 2. 获取源路径上的文件信息，并保存到结构体中，返回成功与否状态
    int status = stat(file_path, &file_info);
    // 3. 判断是否获取成功，并判断文件类别
        // 为0则获取成功
    if (status == 0) { 
        // 使用宏定义，判断是否为目录
        if (S_ISDIR(file_info.st_mode)) { 
            return 0;  // 路径源为目录，返回0
        }
        else {
            return 1; // 其他视为文件，返回1
        }
    } 
    else {
        if (errno == ENOENT){ // #@-@# Linux很多api调用失败后，都会把错误信息保存到errno中，ENOENT是C++11的宏常量表示文件或目录不存在
            return 2; // 文件或目录不存在，返回2
        }
        else {
            return 3; // Unexpected Error，返回3
        }
    }
    //#@-@# const char * const & file path :
    // 1. 目的是不更改file_path，然后在加上使用引用，得到 const char *& file_path，无法编译通过
    // 2. 原因：const是底层，虽然不能更改file_path，但是使用引用必须保持引用前后数据类型一致，除非加上使用顶层const，就可以强制转换
}

int MyCopy::GetFileName(const char * const & file_path) {
    // 源文件名称在最后一个"/"之后，或者源路径就是源文件名称
    for (int i = strlen(file_path) - 1; i>= 0; --i) {  // 从后往前遍历
        if (file_path[i] == '/' || i == 0) {  // 遇到第一个"/"，即路径中最后一个"/" 或者 i == 0，不存在"/"，源路径就是源文件名称
            for (i = i + 1; i < strlen(file_path); ++i) {
                source_file_name_ += file_path[i];  // 这里从前往后获取文件名称，存入source_file_name中
            }
            break;
        }
    }
    return 0;
}

int MyCopy::GetDiretoryName() {
    target_directory_name_ = "";
    for (int i = strlen(target_path_) - 1; i >= 0; --i) {
        if (target_path_[i] == '/') { // 最后一个"/"，前面就是目标文件的路径
            for (int j = 0; j < i; ++j) {
                target_directory_name_ += target_path_[j]; //添加目标文件的路径
            }
            break;
        }
    }
}

int MyCopy::CopyFileToDiretory() {
    // 提权源文件的名称，注意不是源路径
    this -> GetFileName(source_path_);
    // 复制目标路径，路径不包含目标文件名
    string *target_file = new string(target_path_);
    // 目标路径加上文件名
    (*target_file).append(source_file_name_);
    // 执行文件复制
    return (this -> CopyFileToFile(target_file));
}

int MyCopy::CopyFileAndRename() {
    // 获取目标文件路径
    this -> GetDiretoryName();  // #@-@# 注意bug，若没有diretory，直接在当前目录下复制新的文件
    // 检查目标文件路径上的目录是否存在
    int target_dirtory_status = this -> CheckPath(target_directory_name_.c_str()); 
    if (target_dirtory_status == 0 || target_directory_name_ == "") { // 存在相应的目录 或者 在当前目录
        string *target_file = new string (target_path_); // 复制目标路径
        return (this -> CopyFileToFile(target_file));  // 按照当前目标路径进行复制即可
    }
    else {
        cout << "No such file or directory : " << target_path_ << endl;
        return false;
    }
}

int MyCopy::CopyFileToFile(const string *target_file) {
    int select; // 用户选择
    ifstream fin;  //定义输入流
    ofstream fout;  //定义输出流

    if (target_file == nullptr) { // 已存在文件
        cout << "Target file exist. You can" << endl;
        cout << "1. Overwrite" << endl;
        cout << "2. Merge" << endl;
        cout << "3. Cancel" << endl;
        cin >> select;
        if (select != 1 && select != 2) { // 用户选择3或者其他，结束复制
            return false;
        }
        target_file = new string(target_path_); // 并复制目标文件路径
    }
    
    // 打开源文件
    if (select == 2) { // 合并文件
        fout.open((*target_file).c_str(), ios::app);  // 使用app模式
    } else {
        fout.open((*target_file).c_str());  // 否则直接打开文件
    }
    if (!fout.is_open()) {
        cout << "Target file open failed" << endl;
        return false;
    }

    fin.open(source_path_);  // 打开源文件
    if (!fin.is_open()) {
        cout << "Source file open failed" << endl;
        return false;
    }

    string temp;
    while (getline(fin, temp)) {
        fout << temp << endl;
    }
        
    fin.close();
    fout.close();
    delete target_file;
    return true;
}

bool MyCopy::Start() {
    // 0. 查看是否是同一文件
    if (strcmp(source_path_, target_path_) == 0) {
        cout << '"' << source_path_ << "\" and \"" << target_path_ << "\" are the same file" << endl;
        return false;
    }
    // 1. 检查源路径
    int source_status = this -> CheckPath(source_path_);  // #@-@# 直接使用CheckPath()也可以调用
    // 2. 根据源路径结果执行下一步命令
    switch (source_status) {
        case 0:  // 源路径是目录，暂不支持
            cout << "Source Error: Still can not support that the file is a folder" << endl;
            return false;

        case 1: {  // 源目录是文件，检查目标路径
            int target_status = this -> CheckPath(target_path_);
            switch(target_status) {
                case 0:  
                    return (this -> CopyFileToDiretory());  // 目标路径是目录，复制到该目录下

                case 1:
                    return (this -> CopyFileToFile());  // 目标路径是文件，且目标文件已存在，是否覆盖

                case 2:
                    return (this -> CopyFileAndRename()); // 文件不存在，查看是否存在路径上的目录，若存在创建新的目标文件(复制 + 命名)，否则提示错误

                case 3:  
                    cout << "Target Unexpected Error: Error occur when getting the file information" << endl; // 预料外的错误：检查checkPath()
                    return false;

                default:  
                    cout << "Target Unexpected Error: Error occur after check path to check the status" << endl; // 预料外的错误：检查switch()
                    return false;
            }
        }
        break;

        case 2: 
            cout << "Source Error: No such file or directory" << endl;  // 源路径不存在
            return false;

        case 3:  
            cout << "Source Unexpected Error: Error occur when getting the file information" << endl; // 预料外的错误：检查checkPath()
            return false;

        default: 
            cout << "Source Unexpected Error: Error occur after check path to check the status" << endl; // 预料外的错误：检查switch()
            return false;
    }
}

int main (int argc, char *agrv[]) {
    MyCopy *my_copy = new MyCopy(agrv[1], agrv[2]);
    if (!my_copy->Start()) {
        cout << "Copy file falied." << endl;
    }
    delete my_copy;
    return 0;
}



/*
    测试用例：
    ./mycp test test       (相同文件，不予复制)✔
    ./mycp test xxx
    ./mycp test dir/test   (dir不存在test时)  ✔
    ./mycp test dir/xxx    (dir不存在xxx时)   ✔
    ./mycp test dir/test   (dir存在test时)    ✔
        1. Overwrite  ✔
        2. Merge      ✔
        3. Cancel     ✔
    ./mycp dir/aaa aaa                        ✔
*/

/*
    测试第一个用例：
        调试到心态崩溃，错误原因我猜是因为不能使用ifstream 和 ofstream 指向同一个文件。
*/