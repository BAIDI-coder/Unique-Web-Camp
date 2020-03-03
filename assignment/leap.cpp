#include<iostream>
#include<cstdio>
#include<stdlib.h>
#define CAPACITY 1000
#define INI -(1 << 30)
using namespace std;
//小顶堆
class Myleap{
    private:
        int data[CAPACITY];
        int size;
        void sink(int top);
    public:
        int pop();
        void insert(int x);
        int empty()
        {
            return !size ? 1 : 0;
        }
        int full()
        {
            return size == CAPACITY ? 1 : 0;
        }
        Myleap()
        {
            size = 0;
            data[0] = INI;
        }
};

int main()
{
    Myleap l;
    char cmd;
    cout << "选择一种方法操作最小堆:\ni [space] num--insert t--pop\ne--empty f--full\n输入其他字符退出" << endl;
    while(1)
    {
        cin >> cmd;
        switch (cmd)
        {
        case 'i':
            int in;
            cin >> in;
            l.insert(in);
            break;
        
        case 't':
            cout << l.pop() << endl;
            break;

        case 'e':
            cout << (l.empty() ? "YES" : "NO") << endl;
            break;

        case 'f':
            cout << (l.full() ? "YES" : "NO") << endl;
            break;

        default:
            return 0;
        }
        cout << "next" << endl;
    }
    return 0;
}

void Myleap::sink(int top)   //调整堆中顶的序列为top的堆
{
    int stone = data[top];
    int parent = top;
    int son;
    while(1){
        if(parent * 2 > size) break;
        else if (parent * 2 + 1 > size) son = parent * 2;
        else son = (data[parent * 2] < data[parent * 2 + 1]) ? (parent * 2) : (parent * 2 + 1);  //选择左右儿子中较小的儿子“下沉”
        if(stone > data[son]) data[parent] = data[son], parent = son;
        else break;
    }
    data[parent] = stone;
}
int Myleap::pop()
{
    if(empty()){
        cout << "The leap is empty!" << endl;
        return 0;
    }
    int item = data[1];
    data[1] = data[size--];
    sink(1);
    return item;
}
void Myleap::insert(int x)
{
    int son = ++size;
    int parent = son / 2;
    while(data[parent] > data[son]){
        data[son] = data[parent];
        son = parent;
        parent /= 2;
    }
    data[son] = x;
}