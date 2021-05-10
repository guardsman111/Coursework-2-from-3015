#version 460

//Recieves the triangles from the model 
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

//Outputs the geometry to the frag shader
out vec3 GNormal;
out vec3 GPosition;
noperspective out vec3 GEdgeDistance;

//Input for vertex positions
in vec3 VNormal[];
in vec3 VPosition[];

uniform mat4 ViewportMatrix;

void main()
{
    //Transforming each vertex into viewport space
    vec2 p0 = vec2(ViewportMatrix * (gl_in[0].gl_Position / gl_in[0].gl_Position.w));
    vec2 p1 = vec2(ViewportMatrix * (gl_in[1].gl_Position / gl_in[1].gl_Position.w));
    vec2 p2 = vec2(ViewportMatrix * (gl_in[2].gl_Position / gl_in[2].gl_Position.w));

    //Calculate edges of triangle
    float a = length(p1 - p2);
    float b = length(p2 - p0);
    float c = length(p1 - p0);

    //Angle Calculation
    float alpha = acos((b*b + c*c - a*a) / (2.0 * b * c));
    float beta = acos((a*a + c*c - b*b) / (2.0 * a * c));

    //Distance Calculation
    float ha = abs(c * sin(beta));
    float hb = abs(c * sin(alpha));
    float hc = abs(b * sin(alpha));

    //passing the distance and position of the new vertex, as well as the normal and position of the mesh to frag

    GEdgeDistance = vec3 (ha, 0, 0); //sets each variable of the geometry
    GNormal = VNormal[0];
    GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    
    GEdgeDistance = vec3 (0, hb, 0);
    GNormal = VNormal[1];
    GPosition = VPosition[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    
    GEdgeDistance = vec3 (0, 0, hc);
    GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}