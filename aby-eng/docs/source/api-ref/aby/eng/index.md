---
generator: doxide
---


# eng



:material-package: [ecs](ecs/index.md)
:   

:material-package: [meta](meta/index.md)
:   

:material-package: [utf8](utf8/index.md)
:   

## Types

| Name | Description |
| ---- | ----------- |
| [App](App.md) | Core application class !!! note The entry point will call `App::run`  |
| [EntryPoint](EntryPoint.md) | Define your entry point class and then in your main function call EntryPoint::exec<MyEntryClass>(...);  |
| [Font](Font.md) | Font class for loading local or system fonts and storing them as renderable glyphs with caching. |
| [Object](Object.md) | The core object class to execute functionality during the application lifecycle  |
| [Renderer2D](Renderer2D.md) | Two dimensional rendering interface  |
| [Text](Text.md) | UTF-8 aware text class  |
| [Time](Time.md) | Time class mainly used for deltatime with easy conversion functions !!! note formatter {:ms} (milli) {:s} (sec) {:ns} (nano) {:us} (micro) calls function then uses suffix ms, s, ns, μs  |
| [UUID](UUID.md) | Unique identifier generator  |

## Functions

| Name | Description |
| ---- | ----------- |
| [create](#create) | Create an object and add it to the application :material-code-tags: `T` :    the object type (derived from Object base class) :material-code-tags: `Args` :    T::create argument types :material-location-enter: `args` :    T::create arguments :material-keyboard-return: **Return** :    ref<T> !!! note defined in "core/app.hpp"  |

## Function Details

### create<a name="create"></a>
!!! function "template &lt;typename T, typename... Args&gt; requires(CObject&lt;T&gt;) static auto create(Args&amp;&amp;... args) -&gt; ref&lt;T&gt;"

    Create an object and add it to the application
        
    :material-code-tags: `T`
    :    the object type (derived from Object base class)
        
    :material-code-tags: `Args`
    :    T::create argument types
        
    :material-location-enter: `args`
    :    T::create arguments
        
    :material-keyboard-return: **Return**
    :    ref<T>
        
    !!! note
    
    
    	defined in "core/app.hpp"
    

