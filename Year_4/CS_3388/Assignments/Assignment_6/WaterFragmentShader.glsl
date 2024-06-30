#version 440

// Interpolated values from the vertex shaders
in vec3 position_gs;
in vec3 normal_gs;
in vec3 eye_gs;
in vec3 light_gs;
in vec2 uv_gs;

// Ouput data
out vec4 color_out;

uniform sampler2D waterTexture;

void phongColor() {

    // Light emission properties
    vec4 LightColor = vec4(1,1,1,1);

    // Material properties
    vec4 MaterialDiffuseColor = texture(waterTexture, uv_gs);
    vec4 MaterialAmbientColor = vec4(0.2,0.2,0.2,MaterialDiffuseColor.a) * MaterialDiffuseColor;
    vec4 MaterialSpecularColor = vec4(0.7, 0.7, 0.7,MaterialDiffuseColor.a);

    // Normal of the computed fragment, in camera space
    vec3 n = normalize( normal_gs );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( light_gs );
    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float cosTheta = clamp( dot( n,l ), 0,1 );

    // Eye vector (towards the camera)
    vec3 E = normalize(eye_gs);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );

    color_out =
        // Ambient : simulates indirect lighting
        MaterialAmbientColor +
        // Diffuse : "color" of the object
        MaterialDiffuseColor * LightColor * cosTheta +
        // Specular : reflective highlight, like a mirror
        MaterialSpecularColor * LightColor * pow(cosAlpha,8);
}

void main(){
    phongColor();
}
