#include "globals.h"
#include "UIAlloc.h"

UIAlloc::UIAlloc(int size)
{
    capacity = size;
    objs = new UIObject* [capacity];
    freeKey = new int[capacity], pendingErase = new int[capacity];
    lidx = capacity - 1, eidx = 0;
    for (int i = 0; i < capacity; i++)
    {
        objs[i] = nullptr;
        freeKey[i] = capacity - 1 - i;
    }
}

void UIAlloc::clear()  //reset and clear
{
    lidx = capacity - 1, eidx = 0;
    for (int i = 0; i < capacity; i++)
    {
        if (objs[i] != nullptr) { delete objs[i]; objs[i] = nullptr; } //if not null
        freeKey[i] = capacity - 1 - i;
    }
}

int UIAlloc::peek() { return freeKey[lidx]; }  //get the next free

int UIAlloc::pop() // get and pop the next free
{
    int ret = freeKey[lidx--];
    if (lidx < 0) exit(0);
    return ret;
}

void UIAlloc::push(int i) { freeKey[++lidx] = i; }  //add a new idx to free

int UIAlloc::alloc(UIObject* obj, int sceneId) // add a new UIObject and return its idx
{
    int idx = pop();
    objs[idx] = obj;
    scene_ids[sceneId].insert(idx); 
    return idx;
}

UIObject* UIAlloc::operator [](int i) const
{
    if (i < 0 || i >= capacity || objs[i] == nullptr)
    {
        exit(0);
    }
    return objs[i];
}

UIObject*& UIAlloc::operator [](int i)
{
    if (i < 0 || i >= capacity || objs[i] == nullptr)
    {
        exit(0);
    }
    return objs[i];
}

void UIAlloc::erase(int idx)  //erase object at index
{
    pendingErase[eidx++] = idx;
}

void UIAlloc::update()  //remove all memory waiting to be deleted
{
    int idx;
    for (int i = 0; i < eidx; i++)
    {
        idx = pendingErase[i];
        if (objs[idx] != nullptr)  //if not null
        {
            push(idx);  //return free key
            delete objs[idx];
            objs[idx] = nullptr;
        }
    }
    eidx = 0;
}