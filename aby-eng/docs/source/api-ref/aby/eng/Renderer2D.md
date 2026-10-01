---
generator: doxide
---


# Renderer2D

**class  Renderer2D**

Two dimensional rendering interface


## Functions

| Name | Description |
| ---- | ----------- |
| [quad](#quad) | Upload a quad to be renderered :material-location-enter: `transform` :    The quad transform in pixel coordinates :material-location-enter: `material` :    The rendering material style  |
| [text](#text) | Upload a string of text to be rendered as glyphs via the fonts glyph map :material-location-enter: `pos` :    the position: (0, 0) -> top left. (the text is positioned from its top left corner as well). :material-location-enter: `font` :    a resource pointer to a loaded font object :material-location-enter: `text` :    text and styling !!! note newlines will put the text on the next line height of space  |
| [textf](#textf) | Upload a string of formattable text to be rendered as glyphs via the fonts glyph map :material-location-enter: `pos` :    the position: (0, 0) -> top left. (the text is positioned from its top left corner as well). :material-location-enter: `text` :    text and styling !!! example "To-do" more robust text formatting !!! note format protocol: ansi esc codes & <col:#RRGGBB></col> tags (currently)  |
| [submit](#submit) | Submit a draw cmd to the renderers render pass :material-location-enter: `cmd` :    the rhi draw cmd to upload. !!! note copies the draw command but the vertex & index buffers remain the same. this allows for a draw command to be reused but change its user data  |
| [enable_scissor](#enable_scissor) | Set the scissor flag :material-location-enter: `enable` :    [true | false]  |
| [set_scissor](#set_scissor) | Set the scissor region :material-location-enter: `min` :    The min coords of the rectangle :material-location-enter: `max` :    The max coords of the rectangle  |

## Function Details

### enable_scissor<a name="enable_scissor"></a>
!!! function "static auto enable_scissor(bool enable) -&gt; void"

    Set the scissor flag
        
    :material-location-enter: `enable`
    :    [true | false]
    

### quad<a name="quad"></a>
!!! function "static auto quad(const Transform2D&amp; transform, const Material2D&amp; material = Material2D()) -&gt; void"

    Upload a quad to be renderered
    
    :material-location-enter: `transform`
    :    The quad transform in pixel coordinates
        
    :material-location-enter: `material`
    :    The rendering material style
    

### set_scissor<a name="set_scissor"></a>
!!! function "static auto set_scissor(glm::ivec2 min, glm::ivec2 max) -&gt; void"

    Set the scissor region
        
    :material-location-enter: `min`
    :    The min coords of the rectangle
        
    :material-location-enter: `max`
    :    The max coords of the rectangle
    

### submit<a name="submit"></a>
!!! function "static auto submit(const rhi::DrawCmd&amp; cmd) -&gt; void"

    Submit a draw cmd to the renderers render pass
            
    :material-location-enter: `cmd`
    :    the rhi draw cmd to upload.
        
    !!! note
    
    
    	copies the draw command but the vertex & index buffers remain the same.
          this allows for a draw command to be reused but change its user data
    

### text<a name="text"></a>
!!! function "static auto text(const glm::fvec2&amp; pos, FontPtr font, const Text2D&amp; text) -&gt; void"

    Upload a string of text to be rendered as glyphs via the fonts glyph map
        
    :material-location-enter: `pos`
    :    the position: (0, 0) -> top left. (the text is positioned from its top left corner as well).
        
    :material-location-enter: `font`
    :    a resource pointer to a loaded font object
        
    :material-location-enter: `text`
    :    text and styling
        
    !!! note
    
    
    	 newlines will put the text on the next line height of space
    

### textf<a name="textf"></a>
!!! function "static auto textf(const glm::fvec2&amp; pos, FontPtr font, const Text2D&amp; text) -&gt; void"

    Upload a string of formattable text to be rendered as glyphs via the fonts glyph map
        
    :material-location-enter: `pos`
    :    the position: (0, 0) -> top left. (the text is positioned from its top left corner as well).
        
    :material-location-enter: `text`
    :    text and styling
        
    !!! example "To-do"
             more robust text formatting
            
    !!! note
    
    
    	 format protocol: ansi esc codes & <col:#RRGGBB></col> tags (currently)
    

