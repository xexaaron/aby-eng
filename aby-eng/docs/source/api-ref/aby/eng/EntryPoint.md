---
generator: doxide
---


# EntryPoint

**class  EntryPoint**

Define your entry point class and then in your main function call EntryPoint::exec<MyEntryClass>(...);


## Functions

| Name | Description |
| ---- | ----------- |
| [exec](#exec) | Execute the global entry point :material-code-tags: `T` :    EntryPoint derived class :material-location-enter: `argc` :    the argument count supplied by the main function :material-location-enter: `argv` :    the arguments supplied by the main function :material-keyboard-return: **Return** :    the application exit code  |
| [get](#get) | Get the globally set entry point  |
| [on_cmdl](#on_cmdl) | Callback to have the application register arguments for parsing. |
| [on_exec](#on_exec) | Callback to add app data such as objects or ui components. |
| [on_create](#on_create) | Callback for after all objects have been created !!! note Called after systems have been initialized right before the main loop. Different from `on_exec` which is called before `Object::on_create` initialization  |
| [on_exit](#on_exit) | Callback for when the application exits !!! note Called after the window/render loop has exited before app deinitalization  |

## Function Details

### exec<a name="exec"></a>
!!! function "template &lt;typename T&gt; requires(std::derived_from&lt;T, EntryPoint&gt;) static auto exec(i32 argc, char&#42;&#42; argv) -&gt; i32"

    Execute the global entry point
    
    :material-code-tags: `T`
    :    EntryPoint derived class
        
    :material-location-enter: `argc`
    :    the argument count supplied by the main function
        
    :material-location-enter: `argv`
    :    the arguments supplied by the main function
        
    :material-keyboard-return: **Return**
    :    the application exit code
    

### get<a name="get"></a>
!!! function "static auto get() -&gt; ref&lt;EntryPoint&gt;"

    Get the globally set entry point
    

### on_cmdl<a name="on_cmdl"></a>
!!! function "virtual auto on_cmdl(argparse::ArgumentParser&amp; parser) -&gt; void"

    Callback to have the application register arguments for parsing.
        
    :material-location-enter: `parser`
    :    The command line parser to add arguments to.
        
    !!! note
    
    
    	 Use `parser.store_into(...)` to store parsed values.
    
    	 Optionally you can set the epilog or description for the application.
    
    
    !!! warning
    
    
    	 Do not call `parse` or `parse_known_args` on the parser.
    

### on_create<a name="on_create"></a>
!!! function "virtual auto on_create() -&gt; void"

    Callback for after all objects have been created
    
    !!! note
    
    
    	Called after systems have been initialized right before the main loop.
    
    	Different from `on_exec` which is called before `Object::on_create` initialization
    

### on_exec<a name="on_exec"></a>
!!! function "virtual auto on_exec() -&gt; void"

    Callback to add app data such as objects or ui components.
    
    !!! note
    
    
    	Called after systems have been initialized at the start of `App::run()`
    

### on_exit<a name="on_exit"></a>
!!! function "virtual auto on_exit() -&gt; void"

    Callback for when the application exits
    
    !!! note
    
    
    	 Called after the window/render loop has exited before app deinitalization
    

