#include<iostream>
#include<cstdio>
#include<stdlib.h>
#define CAPACITY 1000
using namespace std;
class Mystack{
    private:
        int data[CAPACITY];
        int size;
    public:
        void push(int x)
        {
            if(full()){
                cout << "the stack is full!" << endl;
                return;
            }
            data[++size] = x;
        }
        void pop()
        {
            if(empty()){
                cout << "the stack is empty!" << endl;
                return;
            }
            size--;
        }
        int empty()
        {
            return size == -1 ? 1 : 0;
        }
        int full()
        {
            return size + 1 == CAPACITY ? 1 : 0;
        }
        int front()
        {
            if(empty()){
                cout << "\nthe stack is empty!" << endl;
                return 0;
            }
            return data[size];
        }
        Mystack()
        {
            size = -1;
        }
};
int main()
{
    Mystack s;
    char cmd;
    cout << "选择一种方法操作栈:\np [space] num--push t--pop\ne--empty f--full h--front\n输入其他字符退出" << endl;
    while(1)
    {
        cin >> cmd;
        switch (cmd)
        {
        case 'p':
            int in;
            cin >> in;
            s.push(in);
            break;
        
        case 't':
            s.pop();
            break;

        case 'e':
            cout << (s.empty() ? "YES" : "NO") << endl;
            break;

        case 'f':
            cout << (s.full() ? "YES" : "NO") << endl;
            break;

        case 'h':
            cout << s.front() << endl;
            break;

        default:
            return 0;
        }
        cout << "next" << endl;
    }
    return 0;
}