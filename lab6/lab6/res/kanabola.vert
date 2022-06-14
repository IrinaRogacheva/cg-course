void main()
{   
    gl_FrontColor = vec4(0.15f, 0.4f, 0.15f, 1.f);
	float R = (1 + sin(gl_Vertex.x)) * (1 + 0.9 * cos(8*gl_Vertex.x)) * (1 + 0.1*cos(24*gl_Vertex.x)) * (0.5 + 0.05*cos(140*gl_Vertex.x));
	gl_Position = gl_ModelViewProjectionMatrix * vec4(R * cos(gl_Vertex.x), R * sin(gl_Vertex.x), gl_Vertex.z, 1.0);
};