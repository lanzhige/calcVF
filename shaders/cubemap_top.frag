#version 330 core

precision highp float;
in vec3 TexCoords;
out vec4 outcolor;
uniform samplerCube cubemap;
//uniform vec2 RES;

void main(){
    // Calculate position on quad relative to center
    vec2 RES = vec2(512, 512);
    vec2 vUV = (gl_FragCoord.xy / RES) - vec2(0.5); // -> [-0.5,0.5]^2
    
    // Calculate distance to center
    float dist =  length( vUV );
    
    // The final color of the pixel
    vec4 fColor;
    
    // If we are inside the disc render fisheye else white
    if (dist <= 0.5){
        // Compute x and y coord relative to center
        
        // Compute directional vector on the unit circle
        vec2 dir = normalize( vUV );
        
        // Calculate phi;
        float phi = 3.14159265359*dist;
        
        // Calculate sampling vector for the cube map
        float temp = sin(phi);
        vec3 vector = vec3(temp*dir.x, cos(phi), temp*dir.y);

        // Sample cube map
        vec3 color = texture(cubemap, vector).rgb;
        fColor = vec4(color,1);
        //color = texture(cubemap, TexCoords);
    } else {
        fColor = vec4(1);
    }
    
    // Set pixel color
    outcolor = fColor;
}