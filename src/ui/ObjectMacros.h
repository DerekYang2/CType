#ifndef _OBJECTMACROS_
#define _OBJECTMACROS_
//button creation macros
#define new_Button(scene, ...) ui_objects.alloc(new Button(__VA_ARGS__), scene)
#define new_Toggle(scene, ...) ui_objects.alloc(new Toggle(__VA_ARGS__), scene)
#define new_Slider(scene, ...) ui_objects.alloc(new Slider(__VA_ARGS__), scene)
#define new_Panel(scene, ...) ui_objects.alloc(new Panel(__VA_ARGS__), scene)
#define new_Graph(scene, ...) ui_objects.alloc(new Graph(__VA_ARGS__), scene)

#define get_Button(idx) static_cast<Button*>(ui_objects[idx])
#define get_Panel(idx) static_cast<Panel*>(ui_objects[idx])
#define get_Toggle(idx) static_cast<Toggle*>(ui_objects[idx])
#define get_Slider(idx) static_cast<Slider*>(ui_objects[idx])
#define get_Graph(idx) static_cast<Graph*>(ui_objects[idx])

#endif