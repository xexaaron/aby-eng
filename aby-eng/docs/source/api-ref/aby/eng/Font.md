---
generator: doxide
---


# Font

**class Font**

Font class for loading local or system fonts
and storing them as renderable glyphs with caching.


## Functions

| Name | Description |
| ---- | ----------- |
| [create](#create) | Font creation function :material-location-enter: `rel_path` :    Path relative to cwd/system font folder :material-location-enter: `system` :    Is the font contained by the system !!! note For system fonts the paths are: Linux: `/usr/share/fonts/TTF` Win32: `C:\\Windows\\Fonts` macOS: `/System/Library/Fonts`  |

## Function Details

### create<a name="create"></a>
!!! function "static auto create(const fs::path&amp; rel_path, float px_size = 12.f, bool system = false) -&gt; FontPtr"

    Font creation function
    
    :material-location-enter: `rel_path`
    :    Path relative to cwd/system font folder
        
    :material-location-enter: `system`
    :    Is the font contained by the system
        
    !!! note
    
    
    	 For system fonts the paths are:
    
    	 Linux: `/usr/share/fonts/TTF`
    
    	 Win32: `C:\\Windows\\Fonts`
    
    	 macOS: `/System/Library/Fonts`
    

