uniform float u_time;

void main() 
{
    float a = 1.4;
    float b = 1.4;

    float startY = ((gl_Vertex.x * gl_Vertex.x) / (a * a) + (gl_Vertex.z * gl_Vertex.z) / (b * b) )/ 2.0;
    float endY = ((gl_Vertex.x * gl_Vertex.x / (a * a) - (gl_Vertex.z * gl_Vertex.z) / (b * b))) / 2.0;

    float currY = mix(startY, endY, sin(u_time));
    gl_FrontColor = vec4(0.15f, 0.4f, 0.15f, 1.f);
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4( gl_Vertex.x, currY, gl_Vertex.z, 1.0);
}
