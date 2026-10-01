---
generator: doxide
---


# App

**class  App**

Core application class

!!! note


	 The entry point will call `App::run`


## Functions

| Name | Description |
| ---- | ----------- |
| [run](#run) | Run the application, create objects, initialize entity systems  |
| [exit](#exit) | Exit the application and cleanup all resources  |
| [window](#window) | Get the application viewport window  |
| [init](#init) | Initializes all rendering systems and windowing libraries  |
| [deinit](#deinit) | Cleanup all rendering systems and windows  |
| [parse_args](#parse_args) | Parses engine arguments and application arguments  |
| [add_obj](#add_obj) | Add an object class to the application :material-location-enter: `object` :  |
| [entity_registry](#entity_registry) | Get the entity registry  |
| [add_entity](#add_entity) | Add an entity to the application  |

## Function Details

### add_entity<a name="add_entity"></a>
!!! function "static auto add_entity(entt::entity entity) -&gt; void"

    Add an entity to the application
    

### add_obj<a name="add_obj"></a>
!!! function "static auto add_obj(ref&lt;Object&gt; object) -&gt; void"

    Add an object class to the application
    
    :material-location-enter: `object`
    :
    

### deinit<a name="deinit"></a>
!!! function "static auto deinit() -&gt; void"

    Cleanup all rendering systems and windows
    

### entity_registry<a name="entity_registry"></a>
!!! function "static auto entity_registry() -&gt; entt::registry&amp;"

    Get the entity registry
    

### exit<a name="exit"></a>
!!! function "static auto exit() -&gt; void"

    Exit the application and cleanup all resources
    

### init<a name="init"></a>
!!! function "static auto init(const AppInfo&amp; info) -&gt; bool"

    Initializes all rendering systems and windowing libraries
    

### parse_args<a name="parse_args"></a>
!!! function "static auto parse_args(const AppInfo&amp; info) -&gt; EngineArgs"

    Parses engine arguments and application arguments
    

### run<a name="run"></a>
!!! function "static auto run() -&gt; void"

    Run the application, create objects, initialize entity systems
    

### window<a name="window"></a>
!!! function "static auto window() -&gt; win::Window&#42;"

    Get the application viewport window
    

