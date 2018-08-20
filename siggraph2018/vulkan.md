# Vulkan

Vulkan is not a replacement of OpenGL
OpenGL complexity makes it hard on the driver. This mean big drivers

Kronos is the stadndard org, joined by many companies.

Vulkan is derived from AMD mantle (which was released freely).
Influenced from Metal and DirectX 12.

### Goals
Much driver complexity from driver to app.
Less handholding. No tests in the driver.
Easier multi-threaded.
Handle tiled rendering (important for mobile).

### Diffs with OpenGL
Y facing down.
No current state maintained by the driver. Only tracked by handles.
You must manage the transformation and handle everything in shaders.
Shaders can be pre-"half-compiled". They can be compiled into an IC
representation.

The intermediate representation is known as SPIR-V
(Standard Portable Intermediate Representation for Vuklan).

### Layout

App -> Instance -> Physical Device -> Logical Device -> Queues <= Command Buffers
The multi-threading is that you can have many concurrent command buffers.

Sync is the app job. Events can be polled and waited.
Vulkan does not lock. Your job.

### Compiling

GLSL source -> [COMPILE] -> SPIR-V IC -> [COMPILER IN DRIVER] -> Vendor specific code.

## Concepts

### Data Buffers

A byte buffer in GPU memory. They have no inherent meaning.

Declaring the buffer needs also the memory allocation. You need to set it up to a kind of memory.
There are several types. Can be host-visible, only GPU, etc.

After that do that Memory map, which sends the memory over.
You can query the buffer state of the driver.

## Vertex Buffers

The primitive making with vetices, most came from OpenGL.
They fucked it up with Y down, so keep in mind that GLM wont work out of the box.

You have to invert the glm projection matrix.

Reminder:
Attribute is per vextex data.
Uniform are per pass.

The binding of my info to the attributes is wriiteen by the app.
It is quite verbose.
use offset of. Location is easy,
what is binding in CkVertexInputAttributeDescription?

Pipelne == State

The pipeline is acombination of state, and they're fixed. So you need to create any of them
you want before hand. Some engines can make up to 1000s ot them.


## Draw

Finally it's a call vkCmdDraw
You bind a VertexBuffer to the command buffer and the index buffer.
Then drwa with that command buffer (still a weird state tracking).


