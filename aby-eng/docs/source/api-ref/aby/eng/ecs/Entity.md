---
generator: doxide
---


# Entity

**class Entity**

An extremely lightweight entity class represnting a unique identifier


## Functions

| Name | Description |
| ---- | ----------- |
| [Entity](#Entity) | Create a new entity and register it  |
| [Entity](#Entity) | Only used for copying the id of an entity  |
| [Entity](#Entity) | Only used for copying the id of an entity  |
| [Entity](#Entity) | Sets the moved from entities id to entt::null  |
| [add](#add) | Attach a component to an entity :material-code-tags: `T` :    the component type :material-location-enter: `component` :    a component :material-keyboard-return: **Return** :    the new component  |
| [add](#add) | Attach a component to an entity :material-code-tags: `T` :    default constructible component type :material-keyboard-return: **Return** :    the new component  |
| [emplace](#emplace) | Attach a component to an entity :material-code-tags: `T` :    the component type :material-code-tags: `Args` :    the component constructor arg types :material-location-enter: `args` :    the component constructor args :material-keyboard-return: **Return** :    the new component  |
| [has](#has) | Check if the entity has this component(s) :material-code-tags: `Ts` :    the component type(s)  |
| [get](#get) | Get the component(s) belonging to this entity. |
| [clone](#clone) | Create a new entity and copy all of its component  |

## Function Details

### Entity<a name="Entity"></a>
!!! function "Entity()"

    Create a new entity and register it
    

!!! function "Entity(entt::entity id)"

    Only used for copying the id of an entity
    

!!! function "Entity(const Entity&amp; other)"

    Only used for copying the id of an entity
    

!!! function "Entity(Entity&amp;&amp; other)"

    Sets the moved from entities id to entt::null
    

### add<a name="add"></a>
!!! function "template &lt;typename T&gt; requires(detail::CComponent&lt;T&gt;) auto add(const T&amp; component) -&gt; T&amp;"

    Attach a component to an entity
    
    :material-code-tags: `T`
    :    the component type
        
    :material-location-enter: `component`
    :    a component
        
    :material-keyboard-return: **Return**
    :    the new component
    

!!! function "template &lt;typename T&gt; requires(detail::CComponent&lt;T&gt; &amp;&amp; std::is_default_constructible_v&lt;T&gt;) auto add() -&gt; T&amp;"

    Attach a component to an entity
        
    :material-code-tags: `T`
    :    default constructible component type
        
    :material-keyboard-return: **Return**
    :    the new component
    

### clone<a name="clone"></a>
!!! function "auto clone() const -&gt; Entity"

    Create a new entity and copy all of its component
    

### emplace<a name="emplace"></a>
!!! function "template &lt;typename T, typename... Args&gt; requires(detail::CComponent&lt;T&gt; &amp;&amp; (std::is_constructible_v&lt;T, Args...&gt; || meta::CAggregateInitializable&lt;T, Component, Args...&gt;)) auto emplace(Args&amp;&amp;... args) -&gt; T&amp;"

    Attach a component to an entity
        
    :material-code-tags: `T`
    :    the component type
        
    :material-code-tags: `Args`
    :    the component constructor arg types
        
    :material-location-enter: `args`
    :    the component constructor args
        
    :material-keyboard-return: **Return**
    :    the new component
    

### get<a name="get"></a>
!!! function "template &lt;typename... Ts&gt; requires((detail::CComponent&lt;Ts&gt; &amp;&amp; ...)) auto get() const -&gt; std::conditional_t&lt;sizeof...(Ts) == 1, std::tuple_element_t&lt;0, std::tuple&lt;Ts...&gt;&gt;&amp;, std::tuple&lt;Ts&amp;...&gt;&gt;"

    Get the component(s) belonging to this entity.
        
    :material-code-tags: `Ts`
    :    The component type(s).
        
    :material-keyboard-return: **Return**
    :    T& for a single component, or std::tuple<Ts&...> for multiple components.
    

### has<a name="has"></a>
!!! function "template &lt;typename... Ts&gt; requires((detail::CComponent&lt;Ts&gt; &amp;&amp; ...)) auto has() const -&gt; bool"

    Check if the entity has this component(s)
        
    :material-code-tags: `Ts`
    :    the component type(s)
    

