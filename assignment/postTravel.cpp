#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<stack>
using namespace std;
struct node{
    int data;
    node *left, *right;
};
typedef node* Btree;

struct path{
    Btree ptr;
    int flag;   //flag == 1表示左右子树已经被遍历, flag == 0 表示左右子树尚且未被全部遍历
};
void postTravel(Btree t)
{
    stack< path > s;
    path tmp = {t, 0};
    s.push(tmp);
    Btree cur = s.top().ptr;
    while(!s.empty()){
        if(s.top().flag){
            cout << s.top().ptr->data;
            s.pop();
            continue;
        }   //左右子树都已经遍历过,直接输出并出栈
        cur = s.top().ptr->left;
        while(cur){
            tmp.ptr = cur;
            tmp.flag = 0;
            s.push(tmp);
            cur = cur->left;
        }   //否则遍历左子树
        s.top().flag = 1;   //左子树遍历完成，接下来遍历右子树，提前标记好其左右子树遍历完成
        if(s.top().ptr->right){
            tmp.flag = 0;
            tmp.ptr = s.top().ptr->right;
            s.push(tmp);
        }  //有右子树则遍历右子树
    }
}
int main()
{
    
    return 0;
}