Assistant+
----
Assistant+ is a personal AI tool for 

* Analyzing and visualizing data
* Searching infomation on special topic
* PIM
* Tools integration
## How to compile

### Requirements
Most 3rd party libraries are integrated in the source code. The needed libraries and tools are listed as follows. 

* [cmake 3.0 or newer](http://www.cmake.org)
* [boost](http://www.boost.org)
* [tinyxml2](http://www.grinninglizard.com/tinyxml2/)
* Redis  [windows](https://github.com/ServiceStack/redis-windows),  [linux](https://redis.io)
### Windows
Use CMakeGUI to generate visual studio project.

### Linux

```
mkdir build
cd build
cmake ..
make
```


## How to use

Assistant+ uses a xml file to save configuation, and uses redis to store running information.

1. Configuration
```
<?xml version="1.0" encoding="UTF-8"?>
<setting>
    <add key="ServerIP" value="127.0.0.1"/>
    <add key="Serverport" value="80"/>
    <add key="Number of Threads" value="5"/>
    <add key="Timeout" value="5000"/>
    <add key="No Transfer Timeout" value="30000"/>
    <add key="Enable logging" value="0"/>
    <add key="Web Root" value="WebRoot"/>
    <add key="App Root" value="APP"/>
    <add key="Database address" value="127.0.0.1"/>
</setting>
```

2. Install Redis.

### windows

* Start redis server
* Start Assistant+


### linux

```
systemctrl start redis
setsid Assistant+
```

## TODO
* App plugins
* Rule Engine
* Task Manger

