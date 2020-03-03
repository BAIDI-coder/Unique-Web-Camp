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
void bubblingSort(int s, int e, int* arr)    //ð����������ҿ�,����ѭ��ʱ�临�Ӷ�O(n^2)
{
    for(int i = s;i < e;i++)
        for(int j = s;j < e - 1 - i;j++){
            if(arr[j] > arr[j + 1]){
                MySwap(arr, j, j + 1);
            }
        }
}

void choosingSort(int s, int e, int* arr)    //ѡ������,����ѭ��ʱ�临�Ӷ�O(n^2)
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

void quickSort(int s, int e, int* arr)   //��������,һ��ɨ�踴�Ӷ�Cn���ܹ�log(n)��ɨ�裬�ܸ��Ӷ�O(nlogn)
{
    if(e <= s) return;
    int l = s, r = e - 1;
    int mid = arr[s];   //ѡ���м���
    while(l < r){
        while(l < r && arr[r] >= mid) r--;  //��������ɨ�裬������һ����midС�����ͽ����l��λ�ã�r��l����������ѭ��
        arr[l] = arr[r];
        while(l < r && arr[l] <= mid) l++;
        arr[r] = arr[l];
    }
    arr[l] = mid;
    quickSort(s, l, arr);
    quickSort(l + 1, e, arr);   //�ݹ�Ķ����������ֽ�������
}

int tmp[MAX];    //��ʱ���飬���ڹ鲢����
void emergingSort(int s, int e, int* arr)    //�鲢����,ÿ�Ρ��鲢�����Ӷ�ΪCn,�ܹ�log(n)��ɨ�裬�ܸ��Ӷ�O(nlogn)
{
    if(e - s <= 1) return;
    int mid = (s >> 1) + ((e - 1) >> 1);
    emergingSort(s, mid + 1, arr);
    emergingSort(mid + 1, e, arr);   //����

    int l = s, r = mid + 1, p = -1;
    while(l < mid + 1 && r < e){
        if(arr[l] < arr[r]) tmp[++p] = arr[l++];
        else tmp[++p] = arr[r++];
    }
    while(l < mid + 1) tmp[++p] = arr[l++];
    while(r < e) tmp[++p] = arr[r++];   //�Ƚ��������鰴��С����tmp

    for(int i = s;i < e;i++){
        arr[i] = tmp[i - s];
    }   //tmp������Ƶ�arr
}

struct smallLeap{
    int data[MAX];
    int size;
} l;    //С����
void sink(int top)   //����l�ж�������Ϊtop�Ķ�
{
    int stone = l.data[top];
    int parent = top;
    int son;
    while(1){
        if(parent * 2 > l.size) break;
        else if (parent * 2 + 1 > l.size) son = parent * 2;
        else son = (l.data[parent * 2] < l.data[parent * 2 + 1]) ? (parent * 2) : (parent * 2 + 1);  //ѡ�����Ҷ����н�С�Ķ��ӡ��³���
        if(stone > l.data[son]) l.data[parent] = l.data[son], parent = son;
        else break;
    }
    l.data[parent] = stone;
}
void leapSort(int s, int e, int* arr)    //������
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
    }   //����С����

    for(int i = s;i < e;i++){
        arr[i] = l.data[1];
        l.data[1] = l.data[l.size];
        l.size--;
        sink(1);
    }   //����
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
    char cmd;   //ѡ�񷽷�
    cout << "ָ������Ԫ����Ŀ�����򷽷�(b,c,q,e,l) ���ո������\n";
    cin >> n >> cmd;
    cout << "����" << n << "������\n";
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