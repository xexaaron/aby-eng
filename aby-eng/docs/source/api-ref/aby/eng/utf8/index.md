---
generator: doxide
---


# utf8



## Types

| Name | Description |
| ---- | ----------- |
| [codepoint](codepoint.md) |   A 32 bit utf-8 codepoint class to 'ensure' operations from the utf8 namespace are used on codepoints instead of the standard library functions  |

## Functions

| Name | Description |
| ---- | ----------- |
| [codepoints](#codepoints) | Convienence function to iterate codepoints :material-location-enter: `string` :    The string to get the codepoints from :material-keyboard-return: **Return** :    Codepoints(string)  |
| [encode](#encode) | encode a codepoint into a string :material-location-enter: `cp` :    utf8 codepoint :material-keyboard-return: **Return** :    encoded std::string  |
| [encode](#encode) | encode a list of codepoints into a string :material-location-enter: `cps` :    utf8 codepoints :material-keyboard-return: **Return** :    encoded std::string  |
| [encode](#encode) | encode a codepoint into a string :material-location-enter: `str` :    string to append to :material-location-enter: `cp` :    utf8 codepoint  |
| [encode](#encode) | encode a list of codepoints into a string :material-location-enter: `str` :    string to append to :material-location-enter: `cps` :    utf8 codepoints  |
| [to_lower](#to_lower) | Get the lowercase version of an ascii character that is utf8 encoded :material-keyboard-return: **Return** :    lowercase utf8::codepoint  |
| [to_upper](#to_upper) | Get the uppercase version of an ascii character that is utf8 encoded :material-keyboard-return: **Return** :    uppercase utf8::codepoint  |

## Function Details

### codepoints<a name="codepoints"></a>
!!! function "auto codepoints(std::string_view string) -&gt; Codepoints"

    Convienence function to iterate codepoints
    
    :material-location-enter: `string`
    :    The string to get the codepoints from
        
    :material-keyboard-return: **Return**
    :    Codepoints(string)
    

### encode<a name="encode"></a>
!!! function "auto encode(codepoint cp) -&gt; std::string"

    encode a codepoint into a string
        
    :material-location-enter: `cp`
    :    utf8 codepoint
        
    :material-keyboard-return: **Return**
    :    encoded std::string
    

!!! function "auto encode(std::span&lt;const codepoint&gt; cps) -&gt; std::string"

    encode a list of codepoints into a string
        
    :material-location-enter: `cps`
    :    utf8 codepoints
        
    :material-keyboard-return: **Return**
    :    encoded std::string
    

!!! function "auto encode(std::string&amp; str, codepoint cp) -&gt; void"

    encode a codepoint into a string
        
    :material-location-enter: `str`
    :    string to append to
        
    :material-location-enter: `cp`
    :    utf8 codepoint
    

!!! function "auto encode(std::string&amp; str, std::span&lt;const codepoint&gt; cps) -&gt; void"

    encode a list of codepoints into a string
        
    :material-location-enter: `str`
    :    string to append to
        
    :material-location-enter: `cps`
    :    utf8 codepoints
    

### to_lower<a name="to_lower"></a>
!!! function "auto to_lower(utf8::codepoint cp) -&gt; utf8::codepoint"

    Get the lowercase version of an ascii character that is utf8 encoded
        
    :material-keyboard-return: **Return**
    :    lowercase utf8::codepoint
    

### to_upper<a name="to_upper"></a>
!!! function "auto to_upper(utf8::codepoint cp) -&gt; utf8::codepoint"

    Get the uppercase version of an ascii character that is utf8 encoded
        
    :material-keyboard-return: **Return**
    :    uppercase utf8::codepoint
    

