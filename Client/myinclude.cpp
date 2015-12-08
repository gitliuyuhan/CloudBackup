/*======================================================
    > File Name: myinclude.cpp
    > Author: lyh
    > E-mail:  
    > Other :  
    > Created Time: 2015年12月08日 星期二 14时07分21秒
 =======================================================*/

#include"myinclude.h"

string getRoadFolder(string s)
{
    int  len = s.size() - 1;
    int  i = 0;
    for(i = len;i>=0;i--)
    {
        if(s[i] == '/')
            break;
    }
    return s.substr(i);
}



