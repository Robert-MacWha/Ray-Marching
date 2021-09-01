# Ray Marching & Ray Tracing
 <img src="images/Union_and_seperation.png" width="100%">
 
This repository contains my C++ implementation of ray marching and ray tracing.  Both shaders are written in GLSL and are my first foray into 3d graphics. Ray tracing and ray marching are both methods for rendering realistic 3D graphics based on the physics of real-world light.

## Ray Tracing
[Ray tracing](http://blog.three-eyed-games.com/2018/05/03/gpu-ray-tracing-in-unity-part-1/) works by sending out rays from the camera for each pixel.  Rays travel in a direction away from the camera untill intersecting with an object, where they can be reflected, refracted, or absorbed.  Depending on how the ray interacts with the object more rays might be created or the color of the pixel might be updated. 

Ray tracing is an effective method for rendering highly realistic images.  However, in my opinion it is significantly less interesting than.

<p align="center">
 <img src="images/Ray_tracing.png" width="50%">
</p>
 
## Ray Marching
[Ray marching](https://iquilezles.org/www/articles/distfunctions/distfunctions.htm) also works by sending out rays from the camera for each pixel.  However, instead o calculation intersetions with objects, ray marching instead works by marching the ray along untill it its an object defined by a distance function.  Because objects are represented by mathematical signed distance functions, you're able to do with objects anything that you'd with with functions.  You can add them together, subtract one from another, and even lerp between them.  Furthermore, one can use the modulo function to create mirrors which reflect the scene about an axis, allowing for the creation of fractle scenes with minimal cmoputions.

Ray marching is in my oppinion much cooler than ray tracing.  Here are some gifs demonstrating what I was able to do with a very basic understanding of the concept.

<p float="left">
 <img src="images/Mixing.gif" width="33%"> 
 <img src="images/Lerping.gif" width="33%"> 
 <img src="images/Fractals.gif" width="33%">
</p>

And here's a link to [procedurally generated 3d model](https://www.youtube.com/watch?v=8--5LwHRhjk&t=313s) (of an Anime Girl, of course) using ray marching.  Disclaimer: I didn't make it, it's just cool.
