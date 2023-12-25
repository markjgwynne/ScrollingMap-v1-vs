
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

In this version a world object class was created that allowed management of the generation and rendering of the world.

The world generation is set by passing the world chunk and tile dimensions to the world object.

Only grass and trees are rendered in this version. The grass is light green and the trees are dark green.

**Branch:** 02-camera-position

In this branch we needed to add camera location on top of the world coordinates. 

**Branch:** 03-render-distance-and-collision

This branch added the adjustment of render distance with variables in the main function. 

Collision detection was added to detect impact with trees.

### Improve Rendering

Once the intial version was completed, it was clear that any increase in the size of the world and number of calculations would grind the rendering to a halt. The rendering of only the centre chunk plus one chunk on all sides (9 chunks total) left only up to 50 fps. This was enough to move around with smooth movement, but was clearly not efficient.

**Branch:** 04-render-chunk-in-sprite

To improve efficiency I decided to generate the chunk and then render as a background sprite in the world object. Moving the rendering function into a generate sprite function then calling it on the loading of the world worked nicely. 

One major change was moving the map position. Previously we took the centre of the screen as the camera offset and then subtracted the player position away from it (viCameraOffset - viPlayerPos). This worked when rendering pixels directly, but not with rendering partial sprites.

This is because the rendering of a partial sprite provides the functionality to supply a rendering position and a position on the sprite. Previously we had to manually calculate this. In the new version the calculation was reversed to (viPlayerPos - viCameraOffset).

I also had to create a sprite and decal for a player. Decals are rendered over the top of sprites and draw functions, so the player needed to be a decal to ensure it appeared in front of the background.

