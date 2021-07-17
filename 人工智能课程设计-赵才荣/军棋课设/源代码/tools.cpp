#include"tools.h"
/**
* @file			tools.cpp
* @brief       字符串转化为int
* @author       zzh
* @date			2020.04.13
* @version		0.0.1
*/
int str2int(string str)
{
    int num = 0;
    for (int i = 0; i < str.length(); i++) {
        num *= 10;
        num += str[i] - '0';
    }
    return num;
}

/**
* @file			tools.cpp
* @brief       判断是否是字母，大小写不限
* @author       zzh
* @date			2020.04.13
* @version		0.0.1
*/
bool isalpha(char ch)
{
    return (ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a');
}

/**
* @file			tools.cpp
* @brief        判断是否是数字
* @author       zzh
* @date			2020.04.13
* @version		0.0.1
*/
bool isnumeric(char ch)
{
    return ch <= '9' && ch >= '0';
}

/**
* @file			tools.cpp
* @brief        将（int，int）转化为int
* @author       zzh
* @date			2020.04.13
* @version		0.0.1
*/
int position2int(int x, int y)
{
    return x * 5 + y;
};
void run(int& depth) {

}