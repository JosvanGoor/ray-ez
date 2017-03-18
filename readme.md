# Ray-EZ
This is a raytracer framework i'm writing for fun and to expand my knowledge on math and threaded systems.

## subsystems:
The framework consists of a selection of classes catergorized by function.

#### Data
This category contains classes to parse scenes.
* datanode: This class represents an array/object/value in an json file.
* json: This file contains json parsing functions
* stepdocument: This class contains functions used by parsers.

#### Lights
This category contains light-types.
* point-light: A light that emits light of a certain color from a certain point

#### Math
* algebra: contains functions to solve certain formula's
* math: contains certain commonly used constants and functions
* matrix4x4: this class represents a 4x4 matrix used for translation and projection
* vector3: this class represents a 3x1 vector used to contain color and coordinates etc.
* vector4: ???

#### raytracer
This category contains certain classes designed specifically for the raytracer framework.
* camera: represents a camera in the scene, delivers the rays to be traced.
* hit: represents a hit-point where a ray hits a shape.
* material: represents color and (reflective) characteristics of a material.
* ray: represents a ray (orgigin, direction) used to determine hits.

#### shapes
This category contains raytracable shapes
* disk: class representing a disk, or plane when radius is set to infinite.
* shape: baseclass for every shape.
* sphere: class representing a perfect sphere.

#### threads
This category contains threading functionality to ensure maximum performance! :)
* task: this (virtual)class represents a "thing" a thread can do.
* thread: This (virtual) class represents a general purpose thread.
* workerpool: a class that functions as a task-dispensing unit for workerthreads.
