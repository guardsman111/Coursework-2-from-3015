#version 460

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 GNormal;
out vec3 GPosition;

noperspective out vec3 GEdgeDistance;

in vec3 TENormal[];
in vec3 TEPosition[];

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

    GEdgeDistance = vec3 (ha, 0, 0);
    GNormal = TENormal[0];
    GPosition = TEPosition[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    
    GEdgeDistance = vec3 (0, hb, 0);
    GNormal = TENormal[1];
    GPosition = TEPosition[1];
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    
    GEdgeDistance = vec3 (0, 0, hc);
    GNormal = TENormal[2];
    GPosition = TEPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}