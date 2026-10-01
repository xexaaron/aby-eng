---
generator: doxide
---


# Abyss Engine

API reference for Abyss Engine

:material-package: [aby](aby/index.md)
:   

## Macros

| Name | Description |
| ---- | ----------- |
| [ABY_ENG_COMPONENT_HIDE](#ABY_ENG_COMPONENT_HIDE) | When defining a component this can make it more clearer than [true|false] that a component should be hidden from the tree view  |
| [ABY_ENG_COMPONENT_SHOW](#ABY_ENG_COMPONENT_SHOW) | When defining a component this can make it more clearer than [true|false] that a component should be shown in the tree view  |
| [ABY_ENG_COMPONENT](#ABY_ENG_COMPONENT) | Define common component functions :material-location-enter: `Name` :    the class name of the component :material-location-enter: `Hidden` :    the tree/property visibility of the component  |
| [ABY_ENG_PROPERTIES](#ABY_ENG_PROPERTIES) | Defines the meta structure for a component containing properties :material-location-enter: `The` :    member variable names. ie. !!! note Usage: ``` struct foo { property<...> x = ...; ABY_ENG_PROPERTIES(x); }; ```  |
| [ABY_OBJECT_CLASS](#ABY_OBJECT_CLASS) | Define the object class common properties :material-location-enter: `Class` :    the name of the class  |
| [ABY_OBJECT_DEFAULT_CREATE](#ABY_OBJECT_DEFAULT_CREATE) | Mark the object as default constructible and automatically make the required create function :material-location-enter: `Class` :    the name of the class  |

## Macro Details

### ABY_ENG_COMPONENT<a name="ABY_ENG_COMPONENT"></a>

!!! macro "#define ABY_ENG_COMPONENT(Name, Hidden)"

    Define common component functions
    
    :material-location-enter: `Name`
    :    the class name of the component
        
    :material-location-enter: `Hidden`
    :    the tree/property visibility of the component
    

### ABY_ENG_COMPONENT_HIDE<a name="ABY_ENG_COMPONENT_HIDE"></a>

!!! macro "#define ABY_ENG_COMPONENT_HIDE"

    When defining a component this can make it more clearer than [true|false] that
    a component should be hidden from the tree view
    

### ABY_ENG_COMPONENT_SHOW<a name="ABY_ENG_COMPONENT_SHOW"></a>

!!! macro "#define ABY_ENG_COMPONENT_SHOW"

    When defining a component this can make it more clearer than [true|false] that
    a component should be shown in the tree view
    

### ABY_ENG_PROPERTIES<a name="ABY_ENG_PROPERTIES"></a>

!!! macro "#define ABY_ENG_PROPERTIES(...)"

    Defines the meta structure for a component containing properties
    
    :material-location-enter: `The`
    :    member variable names. ie.
        
    !!! note
    
    
    	 Usage: ``` struct foo { property<...> x = ...; ABY_ENG_PROPERTIES(x); }; ```
    

### ABY_OBJECT_CLASS<a name="ABY_OBJECT_CLASS"></a>

!!! macro "#define ABY_OBJECT_CLASS(Class)"

    Define the object class common properties
    
    :material-location-enter: `Class`
    :    the name of the class
    

### ABY_OBJECT_DEFAULT_CREATE<a name="ABY_OBJECT_DEFAULT_CREATE"></a>

!!! macro "#define ABY_OBJECT_DEFAULT_CREATE(Class)"

    Mark the object as default constructible and automatically make the required
        create function
        
    :material-location-enter: `Class`
    :    the name of the class
    

