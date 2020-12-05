#include "my_list.h"

using std::cout;
using std::endl;

MyList::MyList() {

}

MyList::~MyList() {
    delete option_;
}

MyList::MyList(char * option, const int & option_number) {
    option_numbers_ = option_number;
    option_ = new char [option_numbers_];
    for (int i = 0; i < option_numbers_; ++i) { // 获取option信息
        option_[i] = option[i]; 
        if (option_[i] != 0) {
            option_exist_[option_[i]] = true;  // 建立option映射
        }
    }
}

void MyList::ShowOption() {
	// 显示option
    cout << "avaliable options are as followed: \n \t";
    for (int i = 0; i < option_numbers_; ++i) {
        if (option_[i] != 0) {
            cout << option_[i] << '\t';
        }
    }
    cout << endl << endl;
}

int  MyList::ListAsList() {
    int (*fp)(const char *path, struct stat *buff); // #@-@# 函数指针
	//根据命令行参数进行lstat与stat的切换调用.
	if (option_exist_.count('L')) {
		fp = stat;
	}
	else { // 默认
		fp = lstat;
	}

	char *path = new char[FILE_PATH_MAX_SIZE];  // 创建存储区
	if (getcwd(path, FILE_PATH_MAX_SIZE) != nullptr) {
		DIR *currentdiretory = opendir(path); // 打开文件路径
		if (currentdiretory != nullptr) {
			struct dirent *current_sub_diretory;
			while ((current_sub_diretory = readdir(currentdiretory)) != nullptr) { // 遍历目录
				struct stat buf; // 文件信息缓冲区
				if (fp(current_sub_diretory->d_name, &buf) < 0) {  // 获取文件信息
					cout << ("Error : stat error") << endl;
					continue;
				}
				else if (current_sub_diretory->d_name[0] == '.') { // 隐藏文件
					if (option_exist_.count('a')) {  // 显示隐藏文件信息
						PrintStat(buf, current_sub_diretory->d_name);
					}
					else if (option_exist_.count('A')) { // 显示除了"."、".."以外隐藏文件信息
						if (!(strlen(current_sub_diretory->d_name) == 1 
						|| (strlen(current_sub_diretory->d_name) == 2 && current_sub_diretory->d_name[1] == '.'))) {
							PrintStat(buf, current_sub_diretory->d_name);
						}
					}
				}
				else {  // 显示非隐藏文件信息
					PrintStat(buf, current_sub_diretory->d_name);
				}
			}
			closedir(currentdiretory);  // 关闭目录
		}
		else {
			cout << "Open failed!" << endl;
		}
	}
	else {
		cout << "Error: getcwd fail!";
	}

	delete[] path;
	path = NULL;
}

int  MyList::List() {
    char *path  = new char [FILE_PATH_MAX_SIZE]; // 创建存储区

    if (getcwd(path, FILE_PATH_MAX_SIZE) != nullptr) {  // 获取当前目录名称
        DIR *current_diretory = opendir(path); // 打开当前目录

        if (current_diretory != nullptr) {   // 成功打开文件，且不为空
            struct dirent *current_sub_diretory;
            
            while ((current_sub_diretory = readdir(current_diretory)) != nullptr) {//  读取每条目录

                if (current_sub_diretory->d_name[0] == '.') {  // 出现隐藏文件

                    if (option_exist_.count('a')) {  // 输出所有文件
                        cout << current_sub_diretory->d_name << "  ";
                    } 
                    else if (option_exist_.count('A')) {  // 除了"."、".."以外，输出隐藏文件 

                        if (!(strlen(current_sub_diretory->d_name) == 1 || 
                        (strlen(current_sub_diretory->d_name) == 2 && (current_sub_diretory->d_name[1] == '.')))) {
                            cout << current_sub_diretory->d_name << "  ";
                        }
                    }
                }  
                else {  // 输出非隐藏文件
                    cout<<current_sub_diretory->d_name<<"  ";
                }

            }
            cout << endl;
            closedir(current_diretory);
        }
        else {  // 打开目录失败
            cout << "Error: opendir failed" << endl; 
        }
    }
    else { // 获取当前目录失败
        cout << "Error: getcwd failed" << endl;
    }
    delete [] path;  // 释放内存
    path = nullptr;  // 指针指向空
}

bool MyList::Start() {
    // this -> ShowOption();
    if (option_exist_.count('l')) {
		// 输出文件信息
        this -> ListAsList();
    } else { 
		// 输出文件名称
        this -> List();  
    }
}

void PrintUsage()
{
	// 打印使用语法
	cout << "Usage: ./myls [OPTION]" << endl;
	cout << "Parimeters:" << endl;
	cout << "-l"
		 << "\t"
		 << "Show detailed information of files in the diretory." << endl;
	cout << "-L"
		 << "\t"
		 << "Dereference:  When showing file information for a symbolic link, show information for the file the link references rather than for the link itself" << endl;
	cout << "-a"
		 << "\t"
		 << "Show all hidden files." << endl;
	cout << "-A"
		 << "\t"
		 << "Show almost all hidden files, ignoring \".\" and \"..\"." << endl;
}

void MyList::PrintStat(struct stat buf, char *filename)
{
	// 按照 ls -l 打印相关信息
	cout << GetType(buf.st_mode) << GetPerm(buf.st_mode) << "\t " << buf.st_nlink << "\t ";
	cout << GetUserName(buf.st_uid) << "\t " << GetGroupName(buf.st_gid) << "\t " << buf.st_size << "\t " << GetTime(&buf.st_mtime) << "\t " << filename << endl;
}

char MyList::GetType(mode_t term){
	// 判断文件类型
	if (S_ISREG(term)) {    // 普通文件
		return '-';
	}
	else if (S_ISDIR(term)) { // 目录文件
		return 'd';
	}
	else if (S_ISCHR(term)) { // 字符设备文件
		return 'c';
	}
	else if (S_ISBLK(term)) { // 块设备文件
		return 'b';
	}
	else if (S_ISFIFO(term)) { // 管道文件
		return 'p';
	}
	else if (S_ISLNK(term)) { // 符号链接文件
		return 'l';
	}
	else if (S_ISSOCK(term)) { // socket文件
		return 's';
	}
	else { 
		return 'U'; // Unknown type.
	}
}

char *MyList::GetPerm(mode_t term) {
	static char perm[9]; //#@-@# 使用静态变量，可以使其不会因为函数的销毁而销毁，从而返回相应字符串给调用的地方
	string mode = "rwxrwxrwx";  // 对应位置上的权限位
	for (int i = 8; i >= 0; i--) { // 遍历文件的所有权限位置
		if (term & 1) {
			perm[i] = mode[i]; // 有权限，则对应给出相应位置上的符号
		}
		else {
			perm[i] = '-'; // 没有权限，给出'-'
		}
		term = term >> 1;  // 右移 给出的时数字
	}
	return perm;
}

char *MyList::GetUserName(uid_t uid) {
	return getpwuid(uid)->pw_name; // 根据uid获取用户名
}

char *MyList::GetGroupName(gid_t gid) {
	return getgrgid(gid)->gr_name;  // 根据gid获取组用户名
}

string MyList::GetTime(const time_t *time) {
	string time_str = ctime(time); // C库函数，用于转换时间格式
	return time_str.substr(0, time_str.length()-6);  // 获取时间，省略年份
}

bool ExtractCommandLine(const int &argc, char **& argv, char *option) { // #@-@# argv指针数组
    if (argc > 1 && argv[1][0] != '-') { // 若可选项没有添加'-'，则报错
        PrintUsage();
        return false;
    }
    for (int i = 1; i < argc; ++i) { // 依序提取可能的可选项
        for (int j = 0; j < strlen(argv[i]); ++j) {
            switch (argv[i][j])
            {
                case 'l' :  
                    option[0] = 'l'; // 以列表的形式展示文件
                    break;
                case 'L' :
                    option[1] = 'L'; // 符号链接显示原文件
                    break;
                case 'a' :
                    option[2] = 'a'; // 显示所有文件
                    break;
                case 'A' :
                    option[3] = 'A'; // 显示除"."、"."以外的文件
                    break;
                default:
                    break;
            }
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    char option [4] = {0};
	ExtractCommandLine(argc, argv, option);  // 提取myls命令输入的可选项
    MyList * my_list = new MyList(option, sizeof(option)); // #@-@# 注意不要使用strlen，会因为0而截断，因为时字符数组，可以使用sizeof()
    my_list -> Start(); // 开始打印目录下的文件信息
}