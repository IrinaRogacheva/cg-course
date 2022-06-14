uniform sampler2D tex;
uniform vec3 light_dir;

varying vec3 normal;
varying vec2 uv;

void main()
{
	vec3 n = normalize(normal);
	vec3 l = normalize(light_dir);

	float kd = clamp(dot(n, -l), 0.0, 1.0);

	vec3 tex_color = texture2D(tex, uv).rgb;
	vec3 color = kd * tex_color;

	gl_FragColor = vec4(color, 1.0);
}