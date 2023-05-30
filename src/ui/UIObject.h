#ifndef UI_OBJECT_H
#define UI_OBJECT_H
class UIObject {
public:
    float x, y;
    UIObject() {}
    virtual ~UIObject() {}
    virtual void draw() {}
    virtual void draw_hint() {}
    virtual void update() {}
    virtual void setPos(float x2, float y2) {
        x = x2;  y = y2;
    }
};
#endif