#ifndef _UIALLOC_H
#define _UIALLOC_H
#include "UIObject.h"
class UIAlloc
{
    UIObject** objs;
    int* freeKey, * pendingErase;
    int lidx, eidx;
    int capacity;
    public:
    UIAlloc(int size);
    UIAlloc() {}
    void clear();
    int peek();
    int pop();
    void push(int i);
    int alloc(UIObject* obj, int sceneId);
    int alloc(UIObject* obj, initializer_list<int> sceneIdList);
    UIObject* operator [] (int i) const;
    UIObject*& operator [] (int i);
    void erase(int idx);
    void update();
};
#endif