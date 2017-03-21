# Ray-EZ
This is a raytracer framework i'm writing for fun and to expand my knowledge on math and threaded systems.

Below is a list of things I implemented or am planning to implement, they can be marked as follows:
* !! - This is not implemented but planned
* ! - This is implemented but not tested thouroughly
* \* There is currently a problem with the functionality of this part

## Memory management:
The project relies heavily on raw pointers in some places, I opted out of using smart pointers because these pointers often occur in "hot" code and I dont want to pay the performance overhead. To have some clarity about ownership and lifetime of some of the objects on the heap I will write the explanation below.

I (which is mostly a personal pledge to myself) will also make sure after any major change to the raytracer that a full run though valgrind will report no memory leaks/invalid writes/reads etc..

#### Scene objects (& shapes)
The scene objects are passed to rendermodels as pointer, but the rendermodel will never delete a scene object, as such a scene object may belong to more then 1 rendermodel at the same time and must be deleted manually after the rendermodels are unlinked.

All pointers contained by the scene (shapes & lights) are owned by the scene and will be destroyed when the scene is destroyed.

#### Material objects
Material objects are owned by the Shape object that links to it, as such a material object should never be passed to more then 1 shape at a time. There is however one exception to this, The Triangle shape does not take ownedship of a material object. A triangle assumes the material object belongs to the mesh the triangle also belongs to and as such the destruction of the material is the responsibility of the mesh.

For textures a system will be created so that materials can share textures without having ownedship (textures will be destroyed at another point at the end of runtime.)

## subsystems:
The framework consists of a selection of classes catergorized by function.
* core: contains some generally used functions and the core class.
* main: contians the main-function starting the everything.

### Data
This category contains classes to parse scenes.
* !datanode: This class represents an array/object/value in an json file.
* image: This class contains image data and read/write data.
* !json: This file contains json parsing functions
* !stepdocument: This class contains functions used by parsers.

### Math
* !!algebra: contains functions to solve certain formula's
* !math: contains certain commonly used constants and functions
* !matrix4x4: this class represents a 4x4 matrix used for translation and projection
* vector3: this class represents a 3x1 vector used to contain color and coordinates etc.
* !vector4: ???

### raytracer
This category contains certain classes designed specifically for the raytracer framework.
* camera: represents a camera in the scene, contains information required to setup a viewmodel, image. Also represents DOF and supersampling.
* !hit: represents a hit-point where a ray hits a shape.
    + !!subcase for meshes and transparency
* material: represents color and (reflective) characteristics of a material.
* ray: represents a ray (orgigin, direction) used to determine hits.


### Lights
This category contains light-types.
* point-light: A light that emits light of a certain color from a certain point

Idea for soft shadows: light-volumes, for example a line-light (tl-bar or whatever?) traces rays to the to-be-shadowed point from its width (maybe a 2d space). pointlights can use cones but maybe also make specialized lights for that, so make a light have actual shadowing.

ps. think about shadows + refraction.

### rendering
This category contains a base-class for a (virtual)renderer object and subclasses to determine the shading
* !!depthshader: (debug-based) this class renders the scene based on the z-buffer.
* !!goochshader: this class renders the scene with gooch shading
    + !!supports toon-edges
* !!normalshader: (debug-based) this class renders the scene based on surface normals.
* phongshader: this class renders the scene with phong shading
* rendermodel: this class is the baseclass of all rendermodels.
    + supports: threading.
    + !!supports: refraction
    + supports: reflection
    + !supports: (!!soft) shadows.

### shapes
This category contains raytracable shapes
* !!disk: class representing a disk, or plane when radius is set to infinite.
* !!mesh: class represents a mesh consisting of a multitude of triangles.
* shape: baseclass for every shape.
* sphere: class representing a perfect sphere.
* !!triangle: class represents a (clockwise) triangle.
