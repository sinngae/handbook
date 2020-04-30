#include <iostream>
#include <map>
#include <list>
#include <string>

using namespace std;

typedef struct {
    void *data;
    MItem *prev;
    MItem *next;
} MItem;

typedef map<string, MItem*> Cache;

typedef struct {
    Cache cache;
    MItem *head;
    MItem *end;
    int count;
    int capacity;
}LRUCache;

LRUCache cache; // ³õÊ¼»¯cache{ ?£¬ NULL, NULL, 0, 100 }

void set(const string &key, const void *data) {
    auto iter = cache.cache.find(key);
    if (iter == NULL) {
        MItem *mitem = new MItem();
        mitem->data = data;
        if (cache.head == NULL) {
            mitem->prev = NULL;
            mitem->next = NULL;
            cache.end = cache.head = mitem;
        } else {
            mitem->prev = NULL;
            mitem->next = cache->head;
            cache.head = mitem;
            if (cache.count == cache.capacity) {
                MItem *end_prev = cache.end->prev;
                
                // free cache.end->data?
                delete cache.end;
                
                cache.end = end_prev;
            }
        }
        if (cache.count < cache.capacity) {
            cache.count++;
        }        
        cache.cache.insert(make_pair<string,MItem*>(key, mitem));
    } else {
        if (item->prev != NULL) {
            if (item->next != NULL) {
                item->prev->next = item->next;
                item->next->prev = item->prev;
                
                item->prev = NULL;
                item->next = cache.head;
                cache.head = item;
            } else {
                item->prev->next = NULL;
                
                item->prev = NULL;
                item->next = cache.head;
                cache.head = item;
            }
        } else {
            // do nothing
        }
    }
    
}

int main() {
    //int a;
    //cin >> a;
    cout << "Hello World!" << endl;
}