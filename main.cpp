cmake_minimum_required(VERSION 3.16)
project(untitled2)

set(CMAKE_CXX_STANDARD 14)

add_executable(untitled2 task3.cpp task2.cpp task1.cpp)
#include <iostream>
#include <vector>
#include<string>
#include <functional>
#include <algorithm>
using namespace std;

void BubbleSort(vector<string>myVec, bool cmp(string a, string b))
{
    cout << "Before BubbleSort:\n";
    for (auto i : myVec)
        cout << i << " ";
    cout << endl;
    for(int i=0;i<myVec.size();i++)
        for (int j = 0; j < myVec.size()-1; j++)
        {
            if (cmp(myVec[j], myVec[j+1]))
                swap(myVec[j], myVec[j+1]);
        }
    cout << "After BubbleSort:\n";
    for (auto i : myVec)
        cout << i << " ";
    cout << endl;
}

int main()
{
    vector<string>forSort = { "Zyxwp","Abc","LMNBHJ","18uhgsp","0008","qwertyyy" };

    cout << "Sort by first element (greater)\n";
    BubbleSort(forSort, [](string a, string b) {	return a[0] > b[0]; });
    cout << endl;

    cout << "Sort by last element (greater)\n";
    BubbleSort(forSort, [](string a, string b) {	return a[a.length() - 1] > b[b.length() - 1]; });
    cout << endl;

    cout << "Sort by last element(smaller)\n";
    BubbleSort(forSort, [](string a, string b) {	return a[a.length()-1] < b[b.length()-1]; });
    cout << endl;

    cout << "Sort by length(smaller)\n";
    BubbleSort(forSort, [](string a, string b) {	return a.length() < b.length(); });
    cout << endl;

    cout << "Sort by length(greater)\n";
    BubbleSort(forSort, [](string a, string b){	return a.length() > b.length(); });
    cout << endl;

    return 0;

}

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>

using namespace std;
condition_variable cond_var;
mutex mu;
bool print_done=false;
bool sort_done=true;
bool getSort = false;
void BubbleSort(vector<string>&myVec, bool cmp(string a, string b))
{

    for (int i = 0; i < myVec.size(); i++)
    {
        unique_lock<mutex> lock(mu);
        cond_var.wait(lock, []() {return print_done; });
        for (int j = 0; j < myVec.size() - 1; j++)
        {

            if (cmp(myVec[j], myVec[j + 1]))
                swap(myVec[j], myVec[j + 1]);

        }
        sort_done = true;
        print_done = false;
        lock.unlock();
        cond_var.notify_all();
    }
    getSort = true;
}

int main()
{
    vector<string>forSort = { "Zyxwp","Abc","LMNBHJ","18uhgsp","0008","qwertyyy" };
    thread t(BubbleSort , ref(forSort), [](string a, string b) {return a.length() > b.length(); });
    while (!getSort)
    {
        unique_lock<mutex> lock(mu);
        cond_var.wait(lock, []() {return sort_done; });
        for (auto i :forSort)
            cout << i << " ";
        cout << endl;
        sort_done = false;
        print_done = true;
        lock.unlock();
        cond_var.notify_all();
    }
    t.join();
    return 0;

}

#include <iostream>
#include <vector>
#include <ctime>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>

using namespace std;

const int total_prod = 50;
mutex mu;

void kassa(queue < vector<int>> consumer) {
    for (int i = 1; i <= consumer.size(); i++) {
        mu.lock();
        cout << "work kassa number " << this_thread::get_id() << endl;
        mu.unlock();
        for (int j = 0; j < consumer.front().size(); j++) {
            this_thread::sleep_for(chrono::milliseconds(10));
        }

    }
}

int main(){
    int total_cons; //всего купателей

    srand(time(nullptr));
    total_cons = rand() % 100;

    vector<int> prod; //кол-во продуктов
    queue < vector<int>> consumer; //очередь из покупателей с продуктами
    vector <thread*> kas; // набор касс


    cout << "Total people " << total_cons << endl;
    for (int i = 0; i < total_cons; i++) {
        int num_prod= rand() % total_prod; //кол-во продуктов покупателя
        for (int j = 0; j < num_prod; j++) { //заполнили продуктами
            int id_prod = rand() % total_prod;
            prod.push_back(id_prod);
        }
        consumer.push(prod);
        prod.clear();

        int id = i;
        if ((consumer.size()==5)||(i+1==total_cons)) { //на 5 покупателе создаем кассу и на последних
            kas.push_back(new thread(kassa,consumer));
            while (!consumer.empty()) {
                consumer.pop();
            }
        }
    }
    for (auto &t : kas)
        t->join();
}
