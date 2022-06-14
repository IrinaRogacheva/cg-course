#define M_PI 3.1415926535897932384626433832795


// Check if the point p is on the left side of the line p0p1
bool PointIsOnTheLeft(vec2 p0, vec2 p1, vec2 p)
{
	vec2 p0p1 = p1 - p0;
	// find the orthogonal vector to p0p1
	vec2 n = vec2(-p0p1.y, p0p1.x);
	
	// Find the dot product between n and (p - p0)
	return dot(p - p0, n) > 0.0;
}

vec2 Rotate(vec2 p0, float angle)
{
	mat2 rotate = mat2(cos(angle), sin(angle), -sin(angle), cos(angle));
	return rotate * p0 ;
}

vec2 Translate(vec2 p0, vec2 p1)
{
	return p0 + p1;
}

vec2 Scale(vec2 p0, vec2 p1)
{
	mat2 scale = mat2(p1.x, 0.0, 0.0, p1.y);
	return scale * p0;
}

bool PointIsInEllipse(vec2 center, vec2 radius, vec2 p, float angle)
{
    float cosa=cos(angle);
    float sina=sin(angle);

	float a =pow(cosa*(p.x-center.x)+sina*(p.y-center.y),2);
    float b =pow(sina*(p.x-center.x)-cosa*(p.y-center.y),2);

	return (a/pow(radius.x, 2))+(b/pow(radius.y, 2)) <= 1; 
}

bool PointIsInStar(vec2 p1, vec2 p)
{

	vec2 translateVector = vec2(0.5, 1.37);
	vec2 scalePoints = vec2(0.38, 0.38);

	vec2 points[5];
	float angle = 0;
	for (int i = 0; i < 5; i++)
	{
	    points[i] = Translate(Rotate(Scale(p1, scalePoints), angle * M_PI/180), translateVector);
		angle += 72; 
	}

	return ((PointIsOnTheLeft(points[0], points[1], p) &&
		PointIsOnTheLeft(points[1], points[3], p) &&
		PointIsOnTheLeft(points[3], points[0], p)) //||
		/*(PointIsOnTheLeft(points[4], points[0], p) &&
		PointIsOnTheLeft(points[0], points[2], p) &&
		PointIsOnTheLeft(points[2], points[4], p))*/) &&
		
		((PointIsOnTheLeft(points[1], points[2], p) &&
		PointIsOnTheLeft(points[2], points[4], p) &&
		PointIsOnTheLeft(points[4], points[1], p)) ||
		(PointIsOnTheLeft(points[1], points[3], p) &&
		PointIsOnTheLeft(points[3], points[4], p) &&
		PointIsOnTheLeft(points[4], points[1], p)) ||
		(PointIsOnTheLeft(points[0], points[2], p) &&
		PointIsOnTheLeft(points[2], points[3], p) &&
		PointIsOnTheLeft(points[3], points[0], p)));
}


bool PointIsInsideHammer(vec2 p[32], vec2 p1, vec2 radius[9])
{
	return

		PointIsOnTheLeft(p[0], p[1], p1) &&
		PointIsOnTheLeft(p[1], p[2], p1) &&
		PointIsOnTheLeft(p[2], p[3], p1) &&
		PointIsOnTheLeft(p[4], p[5], p1) ||

		PointIsOnTheLeft(p[5], p[4], p1) &&
		PointIsOnTheLeft(p[6], p[7], p1) &&
		PointIsOnTheLeft(p[8], p[9], p1) &&
		PointIsOnTheLeft(p[10], p[11], p1) ||

		PointIsInEllipse(p[12], vec2(length(p[1] - p[2])/2, length(p[1] - p[2])/2), p1, 0);

}

bool PointIsInsideSickle(vec2 p[32], vec2 p1, vec2 radius[9])
{
	return

		PointIsInEllipse(p[13], radius[0], p1, 0) ||
		PointIsInEllipse(p[14], radius[1], p1, 0) ||
		PointIsInEllipse(p[15], radius[2], p1, -M_PI/4) ||
		PointIsInEllipse(p[16], radius[1], p1, 0) ||
		PointIsInEllipse(p[17], radius[3], p1, 0) ||
		!PointIsInEllipse(p[18], radius[4], p1, 0) &&
		!PointIsInEllipse(p[19], radius[4], p1, 0) &&
		PointIsOnTheLeft(p[18], p[18] + p[18], p1) &&
		PointIsOnTheLeft(p[19], p[19] - p[19], p1) &&
		PointIsOnTheLeft(p[20], p[28], p1) &&
		PointIsOnTheLeft(p[21], p[29], p1) ||
		PointIsOnTheLeft(p[22], p[30], p1) &&
		!PointIsInEllipse(p[23], radius[5], p1, 0)&&
		PointIsInEllipse(p[24], radius[6], p1, 0) &&
		!PointIsOnTheLeft(p[25], p[31], p1) ||
		PointIsOnTheLeft(p[25], p[31], p1) &&
		PointIsInEllipse(p[26], radius[7], p1, 0) &&
		!PointIsInEllipse(p[27], radius[8], p1, 0);

}

bool PointIsInsideEmblem(vec2 p[32], vec2 p1, vec2 radius[9])
{
    vec2 scalePoints = vec2(0.4, 0.4);
	vec2 translateVector = vec2(0.28, 0.85);
	for (int i = 0; i < 32; i++)
	{
	    p[i] = Translate(Scale(p[i], scalePoints), translateVector);
	}

	for (int i = 0; i < 9; i++)
	{
	    radius[i] = Scale(radius[i], scalePoints);
	}

	return

		PointIsInsideHammer(p, p1, radius) ||

		PointIsInsideSickle(p, p1, radius) ||

		PointIsInStar(vec2(0.0, 0.25), p1) &&
		!PointIsInStar(vec2(0.0, 0.16), p1);
}

void main()
{
	vec2 pos = gl_TexCoord[0].xy;
	
	vec2 p[32] = {
		vec2(0.0, 0.95), 
		vec2(0.9, 0.0), 
		vec2(1.0, 0.1), 
		vec2(0.05, 1.0),
		vec2(0.75, 1.0),
		vec2(0.0, 0.25),
		vec2(0.5, 1.0),
		vec2(0.0, 0.5), 
		vec2(0.1, 0.6), 
		vec2(0.7, 0.0),
		vec2(1.0, 0.75), 
		vec2(0.0, 0.9),

		vec2(0.95, 0.05),
		vec2(0.02, 0.02),
		vec2(0.05, 0.05),
		vec2(0.1, 0.1),

		vec2(0.25, 0.25),
		vec2(0.29, 0.29),
		vec2(0.35, 0.05),
		vec2(0.05, 0.35),
		vec2(0.3, 0.2),
		vec2(0.1, 0.2),
		vec2(0.4, 0.4),
		vec2(0.52, 0.52),
		vec2(0.58, 0.48),
		vec2(0.0, 0.5),
		vec2(0.34, 0.45),
		vec2(0.2, 0.5),

		vec2(0.2, 0.3),
		vec2(0.2, 0.1),
		vec2(0.2, 0.2),
		vec2(1.0, 0.5)
		};

	vec2 radius[9] = {
		vec2(0.02, 0.02), 
		vec2(0.05, 0.05), 
		vec2(sqrt(0.005), sqrt(0.0128)), 
		vec2(0.04, 0.04),
		vec2(0.175, 0.175),
		vec2(0.27, 0.27),
		vec2(0.34, 0.34), 
		vec2(0.58, 0.58), 
		vec2(0.59, 0.59),
		};

	if (PointIsInsideEmblem(p, pos, radius))
	{
		gl_FragColor = vec4(0.99, 0.9, 0.06, 1.0);
	}
	else
	{
		vec2 stepXY = step(vec2(0.5, 0.5), fract(pos));
		gl_FragColor = vec4(0.94, 0, 0.17, 1.0);
	}
}
