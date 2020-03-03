#include<iostream>
#include<cstdio>
#include<stdlib.h>
#define MAX 1000
using namespace std;
int n;
int arr[MAX];
void MySwap(int * arr, int a, int b)
{
    int tmp;
    tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}
void bubblingSort(int s, int e, int* arr)    //冒泡排序，左闭右开,两重循环时间复杂度O(n^2)
{
    for(int i = s;i < e;i++)
        for(int j = s;j < e - 1 - i;j++){
            if(arr[j] > arr[j + 1]){
                MySwap(arr, j, j + 1);
            }
        }
}

void choosingSort(int s, int e, int* arr)    //选择排序,两重循环时间复杂度O(n^2)
{
    for(int i = s;i < e - 1;i++){
        int min_index = i;
        for(int j = i;j < e;j++){
            if(arr[min_index] > arr[j]){
                min_index = j;
            }
        }
        if(min_index != i){
            int tmp = arr[min_index];arr[min_index] = arr[i];arr[i] = tmp;
        }
    }
}

void quickSort(int s, int e, int* arr)   //快速排序,一次扫描复杂度Cn，总共log(n)次扫描，总复杂度O(nlogn)
{
    if(e <= s) return;
    int l = s, r = e - 1;
    int mid = arr[s];   //选定中间数
    while(l < r){
        while(l < r && arr[r] >= mid) r--;  //自右向左扫描，遇到第一个比mid小的数就将其填到l的位置，r与l相遇则跳出循环
        arr[l] = arr[r];
        while(l < r && arr[l] <= mid) l++;
        arr[r] = arr[l];
    }
    arr[l] = mid;
    quickSort(s, l, arr);
    quickSort(l + 1, e, arr);   //递归的对左右两部分进行排序
}

int tmp[MAX];    //临时数组，用于归并排序
void emergingSort(int s, int e, int* arr)    //归并排序,每次“归并”复杂度为Cn,总共log(n)次扫描，总复杂度O(nlogn)
{
    if(e - s <= 1) return;
    int mid = (s >> 1) + ((e - 1) >> 1);
    emergingSort(s, mid + 1, arr);
    emergingSort(mid + 1, e, arr);   //分治

    int l = s, r = mid + 1, p = -1;
    while(l < mid + 1 && r < e){
        if(arr[l] < arr[r]) tmp[++p] = arr[l++];
        else tmp[++p] = arr[r++];
    }
    while(l < mid + 1) tmp[++p] = arr[l++];
    while(r < e) tmp[++p] = arr[r++];   //比较左右数组按大小填入tmp

    for(int i = s;i < e;i++){
        arr[i] = tmp[i - s];
    }   //tmp结果复制到arr
}

struct smallLeap{
    int data[MAX];
    int size;
} l;    //小顶堆
void sink(int top)   //调整l中顶的序列为top的堆
{
    int stone = l.data[top];
    int parent = top;
    int son;
    while(1){
        if(parent * 2 > l.size) break;
        else if (parent * 2 + 1 > l.size) son = parent * 2;
        else son = (l.data[parent * 2] < l.data[parent * 2 + 1]) ? (parent * 2) : (parent * 2 + 1);  //选择左右儿子中较小的儿子“下沉”
        if(stone > l.data[son]) l.data[parent] = l.data[son], parent = son;
        else break;
    }
    l.data[parent] = stone;
}
void leapSort(int s, int e, int* arr)    //堆排序
{
    l.data[0] = -1000;
    for(int i = s;i < e;i++){
        l.data[i - s + 1] = arr[i];
    }
    l.size = e - s;
    if(l.size == 1) return;
    int top = l.size / 2;
    while(top != 0){
        sink(top);
        top--;
    }   //建立小顶堆

    for(int i = s;i < e;i++){
        arr[i] = l.data[1];
        l.data[1] = l.data[l.size];
        l.size--;
        sink(1);
    }   //排序
}
void Sort(int *arr, char cmd)
{
    switch (cmd)
    {
    case 'b':
        bubblingSort(0, n, arr);
        break;
    case 'c':
        choosingSort(0, n, arr);
        break;
    case 'q':
        quickSort(0, n, arr);
        break;
    case 'e':
        emergingSort(0, n, arr);
        break;
    case 'l':
        leapSort(0, n, arr);
        break;
    default:
        break;
    }
}
int main()
{
    char cmd;   //选择方法
    cout << "指定序列元素数目和排序方法(b,c,q,e,l) （空格隔开）\n";
    cin >> n >> cmd;
    cout << "输入" << n << "个整数\n";
    for(int i = 0;i < n;i++)
    {
        cin >> arr[i];
    }
    Sort(arr, cmd);
    for(int i = 0;i < n;i++)
    {
        cout << arr[i] << ' ';
    }
    return 0;
}