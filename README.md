
# Auto generated scrolling map


### Inital goal 
The initial goal of this project was to create a scrolling infinite map that self-generated randomly, attempting to understand the difficulties that minecraft has solved, but by discovering them myself.

The concept was to set up layers, whereby the world engine would build up each layer bottom to top.

For example, from bottom to top, the following layers would generate and overlay the previous where applicable:
1. grass
1. trees

In this example the bottom layer would need to be the default covering, which makes sense for grass. Then trees can be randomly added on top. This gets more complicated for future layers, but learnings from the these two are the point of the exercise.

When rendering trees, consider tree density and attempt to plot random world coordinates for forests. Populate these areas with more dense trees within a set perimeter range. Several perimeters can be added to reduce density down to the minimum level.

As stated, forests are plotted up front. This will benefit from a specific random generation function to allow this from a seed, such as that in Minecraft. This supports reuse of a seed for testing, and if you like a particular seed.

There also does not need to be a minimum space between forests. Forests can overlap creating large forests based on random chance.

**Research: Perlin Noise**

### First Version

The first version was built in stages and can be viewed in the following GitHub branches:

**Branch:** 01-implement-world-object
I created a new project in Visual Studio 2022. This was adapted from a previous half started project called CarRace that had been adapted to be a map scolling game. The name CarRace was partly kept through these first 3 branches.

In this version the world class was created that allowed management of the generation and rendering of a world.




**Branch:** 02-camera-position


**Branch:** 03-render-distance-and-collision



