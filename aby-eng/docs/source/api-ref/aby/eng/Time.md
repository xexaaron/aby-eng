---
generator: doxide
---


# Time

**class  Time**

Time class mainly used for deltatime with easy conversion functions

!!! note


	 formatter {:ms} (milli) {:s} (sec) {:ns} (nano) {:us} (micro) calls function then uses suffix ms, s, ns, μs


## Functions

| Name | Description |
| ---- | ----------- |
| [Time](#Time) | Construct from milliseconds  |
| [Time](#Time) | Construct from seconds  |
| [Time](#Time) | Construct from nanoseconds  |
| [Time](#Time) | Construct from microseconds  |
| [milli](#milli) | Convert to milliseconds  |
| [sec](#sec) | Convert to seconds  |
| [nano](#nano) | Convert to nanoseconds  |
| [micro](#micro) | Convert to microseconds  |

## Function Details

### Time<a name="Time"></a>
!!! function "explicit Time(std::chrono::milliseconds ms)"

    Construct from milliseconds
    

!!! function "explicit Time(std::chrono::seconds s)"

    Construct from seconds
    

!!! function "explicit Time(std::chrono::nanoseconds s)"

    Construct from nanoseconds
    

!!! function "explicit Time(std::chrono::microseconds s)"

    Construct from microseconds
    

### micro<a name="micro"></a>
!!! function "auto micro() const -&gt; float"

    Convert to microseconds
    

### milli<a name="milli"></a>
!!! function "auto milli() const -&gt; float"

    Convert to milliseconds
    

### nano<a name="nano"></a>
!!! function "auto nano() const -&gt; float"

    Convert to nanoseconds
    

### sec<a name="sec"></a>
!!! function "auto sec() const -&gt; float"

    Convert to seconds
    

