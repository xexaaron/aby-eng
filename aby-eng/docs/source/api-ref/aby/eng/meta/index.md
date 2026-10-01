---
generator: doxide
---


# meta



## Types

| Name | Description |
| ---- | ----------- |
| [fixed_string](fixed_string.md) | A fixed string for string literals as template paremeters :material-code-tags: `N` :    the size of the string  |
| [type_list](type_list.md) | A list that "holds" types and can then perform transformation operations on them or be iterated over. |

## Type Aliases

| Name | Description |
| ---- | ----------- |
| [internal_type](#internal_type) | Gets the type, type defintion from a class  |

## Concepts

| Name | Description |
| ---- | ----------- |
| [CAggregateInitializable](#CAggregateInitializable) | Checks if type can be initialized via aggregrate initialization :material-code-tags: `T` :    the type :material-code-tags: `Args` :    the arguments to initialize `T` with  |
| [CConstructible](#CConstructible) | Checks if a type can be constructed or aggregrate initialized :material-code-tags: `T` :    the type :material-code-tags: `Args` :    the arguments to construct `T` with  |

## Functions

| Name | Description |
| ---- | ----------- |
| [for_each_t](#for_each_t) | @brief Iterate a list of types :material-code-tags: `Ts` :    the types to iterate :material-location-enter: `fn` :    The function to execute on each type in the format ``` []<typename T>(){...} ``` !!! note Passing a `type_list<...>` to this function will iterate over the types contained by the list. You can interchange type lists and types freely, even using multiple type lists.  |

## Type Alias Details

### internal_type<a name="internal_type"></a>

!!! typedef "template &lt;typename T&gt; using internal_type = typename T::type"

    Gets the type, type defintion from a class
    

## Concept Details

### CAggregateInitializable<a name="CAggregateInitializable"></a>

!!! concept "template &lt;typename T, typename... Args&gt; concept CAggregateInitializable = requires { T{ std::declval&lt;Args&gt;()... }; }"

    Checks if type can be initialized via aggregrate initialization
    
    :material-code-tags: `T`
    :    the type
        
    :material-code-tags: `Args`
    :    the arguments to initialize `T` with
    

### CConstructible<a name="CConstructible"></a>

!!! concept "template &lt;typename T, typename... Args&gt; concept CConstructible = std::is_constructible_v&lt;T, Args...&gt; || CAggregateInitializable&lt;T, Args...&gt;"

    Checks if a type can be constructed or aggregrate initialized
        
    :material-code-tags: `T`
    :    the type
        
    :material-code-tags: `Args`
    :    the arguments to construct `T` with
    

## Function Details

### for_each_t<a name="for_each_t"></a>
!!! function "template &lt;typename... Ts&gt; constexpr void for_each_t(auto&amp;&amp; fn)"

    @brief Iterate a list of types
        
    :material-code-tags: `Ts`
    :    the types to iterate
        
    :material-location-enter: `fn`
    :    The function to execute on each type in the format 
    ``` []<typename T>(){...} 
    ```
        
    !!! note
    
    
    	 Passing a `type_list<...>` to this function will iterate over the
        	 types contained by the list.
    
    	 You can interchange type lists and types freely, even using multiple type lists.
    

