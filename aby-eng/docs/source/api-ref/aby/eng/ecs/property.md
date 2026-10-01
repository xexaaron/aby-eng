---
generator: doxide
---


# property

**template &lt;meta::fixed_string DisplayName, typename T, EProperty Access&gt; struct property : detail::component_property&lt;std::remove_cvref_t&lt;decltype(std::declval&lt;T&gt;())&gt;&gt;**

A component property structure that wraps a type
    
:material-code-tags: `DisplayName`
:    a string literal name used to display the property in the editor
    
:material-code-tags: `T`
:    the type of object to wrap
    
:material-code-tags: `Access`
:    determined if the property can be changed in the editor


## Types

| Name | Description |
| ---- | ----------- |
| [meta](property/meta.md) | The property's meta information  |

## Functions

| Name | Description |
| ---- | ----------- |
| [value](#value) | Get the property value  |
| [value](#value) | Get the property value  |

## Function Details

### value<a name="value"></a>
!!! function "auto value() const -&gt; const base&amp;"

    Get the property value
    

!!! function "auto value() -&gt; base&amp;"

    Get the property value
    

