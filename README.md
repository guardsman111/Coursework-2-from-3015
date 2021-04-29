

############################### Introduction ###############################

This is the Github repository for Comp3015 Coursework 2 by Charlie Parsons.

It contains an OpenGL project that attempts to manifest a wireframe ogre and then tessellate it.

Unfortunately, it does not do that, instead flattening it to 1-micron thickness. 

############################### Instructions ###############################

To run the program simply open the build folder and double click the .exe file.

This will open the program and display a rotating camera of a flattened wireframe mesh ogre.

To open the project view, go to the project folder and double click the Coursework2.sln. In order to open this
you will need to have Visual Studio installed on your machine.

############################### Project Breakdown ###############################

The project first opens the ogre's file, bs_ears.obj and loads it to the memory. It is then handed to the geometry
shader which transforms it into a wireframe mesh, as opposed to the standard shader method. 

Once it has finished this, it hands it over the to Tessellation Control shader, which working with the Tessellation
Evaluation shader attempts to minimise the number of vertices and show a mesh with a reduced vertex count. However,
something goes wrong and it instead flattens the mesh, giving a 2D ogre head silhouette with no definition on it's
surface.

![image](https://user-images.githubusercontent.com/33025261/116537263-eb120e80-a8dd-11eb-81c3-e78d5a2aaa2d.png)
![image](https://user-images.githubusercontent.com/33025261/116537132-b900ac80-a8dd-11eb-90e9-1bb0cd8c2c78.png)

############################### Speciality ###############################

There is no speciality to my project, unless you count the fact it doesn't work as special. Though it does
look like a badge from the Ogre Town Police Department in greeny-blue.

I was attempting to create a project where you could press numbers to change the type of shader used on the
ogre model, similar to my Coursework 1. However, the Tessellation shader refused to let the program run
unless it was active in the scene (selfish) so I had to scrap that idea and just show 1 type of shader in
the scene, with another working in the background.

############################### Conclusion ###############################

Overall, I have thoroughly hated working with OpenGL. I have found it to have poor debugging options, with no
option to debug shaders which often leaves you scratching your head as to why a shader is behaving the way it
is. 

I hope I will never have to return to using OpenGL, and if I do, I hope to be in a better mental state than
I am currently, else something extremely bad may happen.
