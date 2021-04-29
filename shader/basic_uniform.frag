#version 460

uniform int Cycle;

struct LightInfo
{
    vec4 Position; //Position in eye corrds
    vec3 Intensity; //A,D,S Intensity
};
uniform LightInfo Light;

struct MaterialInfo 
{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
};
uniform MaterialInfo Material;

//line struct for rendering the wireframe
uniform struct LineInfo
{
    float Width;
    vec4 Color;
} Line;

in vec3 GPosition;
in vec3 GNormal;
noperspective in vec3 GEdgeDistance;

//out Colour
layout (location = 0) out vec4 FragColor;


vec3 BlinnPhong( vec3 position, vec3 n )
{
    vec3 ambient = Light.Intensity * Material.Ka;  //calculate ambient

    vec3 s = normalize(Light.Position - (vec4(position, 1.0f) * Light.Position.w)).xyz;


    //calculate dot product for vector s and n using max. Read about max in glsl documentation, if not clear talk to me
    float sDotN = max( dot(s,n), 0.0f );

    //difuse formula for light calculations
    //vec3 diffuse = Light.L * Material.Kd * sDotN;

    vec3 spec = vec3(0.0);

    if( sDotN > 0.0 )
    {
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize( v + s );
        spec = Material.Ks * pow( max( dot(h,n), 0.1f ), Material.Shininess );
    }   
    
    return ambient; //+ Light.L * (diffuse + spec);
}

void main()
{
        //calculate the colour
        vec4 color = vec4(BlinnPhong(GPosition, GNormal), 1.0);

        //Find smallest distance for the fragment
        float d = min(GEdgeDistance.x, GEdgeDistance.y);
        d = min(d, GEdgeDistance.z);

        float mixVal;
        if(d < Line.Width - 1)
        {
            mixVal = 1.0;
        }
        else if(d > Line.Width + 1)
        {
            mixVal = 0.0;
        }
        else 
        {
            float x = d - (Line.Width - 1);
            mixVal = exp2(-2.0 * (x * x));
        }


        color = pow(color, vec4(1.0/2.2));
    
        FragColor = mix( color, Line.Color, mixVal);

//    if (Cycle == 1)
//    {
//        FragColor = Line.Color;
//    }
}
