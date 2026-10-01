---
generator: doxide
---


# Object

**class  Object**

The core object class to execute functionality during the application lifecycle


## Functions

| Name | Description |
| ---- | ----------- |
| [on_create](#on_create) | Called either when the app starts running or when created via ``` create<...>(...) ```  |
| [on_tick](#on_tick) | Called every application tick :material-location-enter: `deltatime` :    the time between the last frame and the current frame  |
| [on_render](#on_render) | Called when the renderer is accepting commands  |
| [on_event](#on_event) | Called when the window dispatches an event :material-location-enter: `event` :    the window event :material-keyboard-return: **Return** :    true to stop propagating the event to other objects  |
| [on_destroy](#on_destroy) | Called before the app shutsdown  |
| [uuid](#uuid) | Get the uuid of this object  |

## Function Details

### on_create<a name="on_create"></a>
!!! function "virtual auto on_create() -&gt; void"

    Called either when the app starts running or when created via 
    ``` create<...>(...) 
    ```
    

### on_destroy<a name="on_destroy"></a>
!!! function "virtual auto on_destroy() -&gt; void"

    Called before the app shutsdown
    

### on_event<a name="on_event"></a>
!!! function "virtual auto on_event(win::Event&amp; event) -&gt; bool"

    Called when the window dispatches an event
        
    :material-location-enter: `event`
    :    the window event
        
    :material-keyboard-return: **Return**
    :    true to stop propagating the event to other objects
    

### on_render<a name="on_render"></a>
!!! function "virtual auto on_render() -&gt; void"

    Called when the renderer is accepting commands
    

### on_tick<a name="on_tick"></a>
!!! function "virtual auto on_tick(const Time&amp; deltatime) -&gt; void"

    Called every application tick
        
    :material-location-enter: `deltatime`
    :    the time between the last frame and the current frame
    

### uuid<a name="uuid"></a>
!!! function "auto uuid() const -&gt; UUID"

    Get the uuid of this object
    

