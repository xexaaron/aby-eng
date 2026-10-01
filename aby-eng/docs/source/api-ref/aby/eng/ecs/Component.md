---
generator: doxide
---


# Component

**struct Component**

Base component class that every component should inherit from.
Descries the static interface that all components should implement.


## Functions

| Name | Description |
| ---- | ----------- |
| [name](#name) | Get the class name of a component !!! note Every component must have this static function defined.  |
| [hidden](#hidden) | Check if the component should be hidden from tree and property views !!! note Every component must have this static function defined  |

## Function Details

### hidden<a name="hidden"></a>
!!! function "static auto hidden() -&gt; bool"

    Check if the component should be hidden from tree and property views
    
    !!! note
    
    
    	 Every component must have this static function defined
    

### name<a name="name"></a>
!!! function "static auto name() -&gt; std::string_view"

    Get the class name of a component
    
    !!! note
    
    
    	 Every component must have this static function defined.
    

