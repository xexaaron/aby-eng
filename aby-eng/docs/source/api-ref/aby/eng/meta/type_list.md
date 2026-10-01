---
generator: doxide
---


# type_list

**template &lt;typename... Ts&gt; struct type_list : detail::type_list_base**

A list that "holds" types and can then perform transformation operations
    on them or be iterated over.
    
:material-code-tags: `Ts`
:    the types to store.


## Type Aliases

| Name | Description |
| ---- | ----------- |
| [type](#type) |     The tuple of types  |
| [transform](#transform) | Transform a list of types into another `type_list` by applying a template :material-code-tags: `Transformation` :    the transformation to apply such as `std::remove_cvref_t`  |

## Variables

| Name | Description |
| ---- | ----------- |
| [SIZE](#SIZE) |     The amount of types held by this list  |

## Type Alias Details

### transform<a name="transform"></a>

!!! typedef "template &lt;template &lt;typename&gt; typename Transformation&gt; using transform = type_list&lt;Transformation&lt;Ts&gt;...&gt;"

    Transform a list of types into another `type_list` by applying a template
        
    :material-code-tags: `Transformation`
    :    the transformation to apply such as `std::remove_cvref_t`
    

### type<a name="type"></a>

!!! typedef "using type                        = std::tuple&lt;Ts...&gt;"

        The tuple of types
    

## Variable Details

### SIZE<a name="SIZE"></a>

!!! variable "static constexpr std::size_t SIZE"

        The amount of types held by this list
    

