#ifndef _OBJECTMACROS_
#define _OBJECTMACROS_
//button creation macros
#define new_Button(scene, ...) UIObjects.alloc(new Button(__VA_ARGS__), scene)
#define new_Toggle(scene, ...) UIObjects.alloc(new Toggle(__VA_ARGS__), scene)
#define new_Slider(scene, ...) UIObjects.alloc(new Slider(__VA_ARGS__), scene)
#define new_Panel(scene, ...) UIObjects.alloc(new Panel(__VA_ARGS__), scene)

#define get_Button(idx) static_cast<Button*>(UIObjects[idx])
#define get_Panel(idx) static_cast<Panel*>(UIObjects[idx])
#define get_Toggle(idx) static_cast<Toggle*>(UIObjects[idx])
#define get_Slider(idx) static_cast<Slider*>(UIObjects[idx])
#endif