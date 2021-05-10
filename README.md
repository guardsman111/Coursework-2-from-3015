

############################### Introduction ###############################

This is the Github repository for Comp3015 Coursework 2 by Charlie Parsons.

It contains an OpenGL project that manifests a wireframe ogre's head. Behind the head  a particle fountain moves 
right to left, then back again. The Head is slightly see through, and rotates quickly in a clockwise direction.

############################### Instructions ###############################

To run the program simply open the build folder and double click the .exe file.

This will open the program and display a camera rotating around a grey/black ogre head. Behind it, a particle
system will shoot particles for around a minute while moving from right to left and back again.

To open the project view, go to the project folder and double click the Coursework2.sln. In order to open this
you will need to have Visual Studio installed on your machine.

############################### Project Breakdown ###############################

The project first opens the ogre's file, bs_ears.obj and loads it to the memory. It is then handed to the geometry
shader which transforms it into a wireframe mesh, as opposed to the standard shader method. 

Before any initialisation is done on the ogre mesh, the particle system is initialised through a series of buffers.
The particle system is always completed first, where neccessary in the code, such as in the render step. In the 
render step we also move the camera to retotate the view of the ogre mesh, which at the same time moves the 
source of the particle fountain and all of the particles along with it.

![image](https://user-images.githubusercontent.com/33025261/117683407-f271cb80-b1ab-11eb-9c0d-eacd9c09b7db.png)
Face of the head image

![image](https://user-images.githubusercontent.com/33025261/117683523-0f0e0380-b1ac-11eb-8d5c-16bd60422f88.png)
Back of the head image

############################### Speciality ###############################

My project was initially attempting to create a wiremesh ogre head with a particle fountain shooting from the back
of the head, like blood, or having two smaller ones, like tears, from the eyes. However, I was unable to get my
fountain's source position to stay in one place when I was rotating the camera view - I imagine I was in fact
rotating the world instead of just the head or the camera, and that combined with the orthographic view means
it appears the particle system is just moving left to right.

############################### Conclusion ###############################

Overall, I have not enjoyed working with openGL. I found it difficult to debug, giving lots of errors that have
very little explaination and often are not targeted to one problem, meaning that a single error code could
potentially be caused by dozens if not hundreds of errors in the code. 

Combined with the lack of debugging within shaders themselves, this lead to a lot of difficult situations where
I was unsure if the problems were related to syntax, logic, my inputted values, or a combination of any or all.

However, I am satisfied with what I managed to achieve. I used a lot of online materials to help get the basic
principles down, and I am confident I have more knowledge with the mathematics of shaders, as well as knowledge
of where to go and find the mathematics should mine not suffice.
