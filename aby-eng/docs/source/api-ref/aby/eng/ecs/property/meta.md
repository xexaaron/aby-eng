---
generator: doxide
---


# meta

**struct meta**

The property's meta information


## Functions

| Name | Description |
| ---- | ----------- |
| [name](#name) | Get the display name for the property  |
| [access](#access) | Get the access qualifier for the property  |
| [type_name](#type_name) | Get the demangled type name of the property's wrapped type  |
| [type_name](#type_name) | Get the type name of the property's wrapped type  |
| [type](#type) | Get the type info of the property's wrapped type  |

## Function Details

### access<a name="access"></a>
!!! function "static constexpr auto access() -&gt; EProperty"

    Get the access qualifier for the property
    

### name<a name="name"></a>
!!! function "static constexpr auto name() -&gt; std::string_view"

    Get the display name for the property
    

### type<a name="type"></a>
!!! function "static constexpr auto type() -&gt; const std::type_info&amp;"

    Get the type info of the property's wrapped type
    

### type_name<a name="type_name"></a>
!!! function "static auto type_name() -&gt; std::string"

    Get the demangled type name of the property's wrapped type
    

!!! function "static constexpr auto type_name() -&gt; std::string_view"

    Get the type name of the property's wrapped type
    

