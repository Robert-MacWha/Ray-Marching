#version 430
layout(local_size_x = 16, local_size_y = 16) in;              // define the local work group
layout(rgba32f, binding = 0) uniform image2D renderImage;     // define the format of the output image

uniform float frame;

uniform float near;
uniform float far;
uniform mat4 projection_inv;

#define INF 1.0/0.0
#define MAX_MARCHING_STEPS 500
#define EPSILON 0.001

//* Ray
struct Ray {

	vec3 origin;
	vec3 direction;

	bool hit;
	vec3 surface_point;
	float depth;
	vec3 color;

	float min_distance;

};

Ray createRay(vec3 origin, vec3 direction) {

	Ray ray;

	ray.origin = origin;
	ray.direction = direction;

	ray.hit = false;
	ray.surface_point = vec3(INF);
	ray.depth = INF;
	ray.min_distance = INF;
	ray.color = vec3(0);

	return ray;

}

// creates a ray based on the inverse projection matrix
Ray createEyeRay(vec2 pixel_coords, vec2 screen_size) {

	float aspect_ratio = screen_size.x / screen_size.y;
	
	// screen coords
	vec2 screen_coords = (2 * pixel_coords) / screen_size - 1;

	// calculate origin
	vec3 origin    = (projection_inv * vec4(screen_coords, -1.0, 1.0) * near).xyz;
	vec3 direction = (projection_inv * vec4(screen_coords * (far - near), far + near, far - near)).xyz;
	direction = normalize(direction);

	Ray ray = createRay(origin, direction);

	return ray;

}

//* OBJECTS
float sdSphere(vec3 point, vec3 pos, float rad) {

	vec3 p = point - pos;
	float d = length(p) - rad;
	return d;

}
float sdBox(vec3 point, vec3 pos, vec3 scale) {

	vec3 p = point - pos;
	vec3 q = abs(p) - scale;
	float d = length(max(q, 0)) + min(max(q.x, max(q.y, q.z)), 0.0);
	return d;

}
float sdTorus(vec3 point, vec3 pos, vec2 scale) {

	vec3 p = point - pos;
	vec2 q = vec2(length(p.xy)-scale.x,p.z);
	return length(q)-scale.y;

}

vec4 sdGround(vec3 point) { 
	return vec4(0.8, 0.8, 0.8, -point.y + 2); 
}
vec4 sdFractal(vec3 z) {

	int Iterations = 12;
	float Scale = 2;
	float Offset = 0.8;

	float r;
    int n = 0;
    while (n < Iterations) {
       if(z.x+z.y<0) z.xy = -z.yx; // fold 1
       if(z.x+z.z<0) z.xz = -z.zx; // fold 2
       if(z.y+z.z<0) z.zy = -z.yz; // fold 3	
       z = z*Scale - Offset*(Scale-1.0);
       n++;
    }
    return vec4(0.9, 0.6, 0.6, (length(z) ) * pow(Scale, -float(n)));

}

struct Sphere {
	vec3 position;
	float radius;
	vec3 color;
	vec4 dist(vec3 point) { return vec4(color, sdSphere(point, position, radius)); } 
}; // position, radius (float), color
struct Box { 
    vec3 position;
	vec3 scale; 
	vec3 color;
	vec4 dist(vec3 point) { return vec4(color, sdBox(point, position, scale)); }
};    // position, scale (vec3)  , color
struct Torus { 
    vec3 position;
	vec2 scale; 
	vec3 color;
	vec4 dist(vec3 point) { return vec4(color, sdTorus(point, position, scale)); }
};  // position, scale (vec2)  , color

//* RAY MARCHING
// operations
vec4 sdIntersection(vec4 a, vec4 b) { return a.w > b.w ? a : b; }
vec4 sdUnion(vec4 a, vec4 b)        { return a.w < b.w ? a : b; }
vec4 sdDifference(vec4 a, vec4 b)   { 
    float d = max(a.w, -b.w);
    return d == a.w ? b : vec4(a.rgb,-a.w); 
}

vec4 sdSmoothIntersection( vec4 d1, vec4 d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2.w-d1.w)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) + k*h*(1.0-h); }
vec4 sdSmoothUnion( vec4 d1, vec4 d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2.w-d1.w)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h); }
vec4 sdSmoothDifference( vec4 d1, vec4 d2, float k ) {
    float h = clamp( 0.5 - 0.5*(d2.w+d1.w)/k, 0.0, 1.0 );
    return mix( d2, vec4(d1.xyz, -d1.w), h ) + k*h*(1.0-h); }

vec3 sdSymmetryX(vec3 p) { return vec3(abs(p.x), p.yz); }
vec3 sdSymmetryY(vec3 p) { return vec3(p.x, abs(p.y), p.z); }
vec3 sdSymmetryZ(vec3 p) { return vec3(p.xy, abs(p.z)); }
vec3 sdSymmetryXZ(vec3 p) { return sdSymmetryX(sdSymmetryZ(p)); }


// calculate the signed distance to the scene from a point
vec4 sceneSDF(vec3 point) {
	
	vec4 min_dist = vec4(INF);
	
	min_dist = sdUnion(min_dist, sdFractal(point));

	return min_dist;

}

void march(inout Ray ray) {
	
	float depth = near;

	// march the ray
	for (int i = 0; i < MAX_MARCHING_STEPS; i ++) {
		
		vec3 ray_point = ray.origin + ray.direction * depth;

		vec4 color_dist = sceneSDF(ray_point);
		vec3 color = color_dist.xyz;
		float dist = color_dist.w;

		ray.min_distance = min(ray.min_distance, dist);

		if (dist < EPSILON) {
			
			// we are now inside of the surface
			ray.hit = true;
			ray.surface_point = ray_point;
			ray.depth = depth;
			ray.color = color;
			return;

		}

		// move allong the view ray
		depth += dist;

		if (depth >= far) {
			// reached the far plane - there was nothing to hit along this ray
			return;
		}

	}

}

//* LIGHTING
vec3 estimateNormal(vec3 p) {

    return normalize(vec3(
        sceneSDF(vec3(p.x + EPSILON, p.y, p.z)).w - sceneSDF(vec3(p.x - EPSILON, p.y, p.z)).w,
        sceneSDF(vec3(p.x, p.y + EPSILON, p.z)).w - sceneSDF(vec3(p.x, p.y - EPSILON, p.z)).w,
        sceneSDF(vec3(p.x, p.y, p.z  + EPSILON)).w - sceneSDF(vec3(p.x, p.y, p.z - EPSILON)).w
    ));

}

float fastLighting(vec3 light, vec3 point, vec3 normal) {
	
	vec3 light_dir = normalize(light - point);
	float diff = max(dot(normal, light_dir), 0);

	return min(max(diff, 0), 1) / 0.75 + 0.25;

}

float trueLighting(vec3 light, vec3 point, vec3 normal, float k) {
	
	vec3 ro = point + normal * EPSILON;
	vec3 rd = normalize(light - point);

	float res = 1.0;
    float ph = 1e20;
    for( float t=near; t<far; )
    {
        float h = sceneSDF(ro + rd*t).w;
        if( h<0.001 )
            return 0.2;
        float y = h*h/(2.0*ph);
        float d = sqrt(h*h-y*y);
        res = min( res, k*d/max(0.0,t-y) );
        ph = h;
        t += h;
    }

    return min(max(res, 0.2), 0.9);

}

void main() {
	
	// get x,y position of pixel
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screen_size = ivec2(gl_NumWorkGroups.xy) * 16;

	// initalize the ray
	Ray ray = createEyeRay(pixel_coords, screen_size);

	// march the ray
	march(ray);

	// calculate information for the surface
	vec3 color = vec3(0.4, 0.6, 0.9);
	if (ray.hit) {

		vec3 point = ray.origin + ray.direction * ray.depth;
		vec3 normal = estimateNormal(point);

		float light_rate = 1000;
		vec3 light1 = vec3(5, 10, -10);
		vec3 light2 = vec3(-10, 10, -5);
		float shading = trueLighting(-light1, point, normal, 10) * 1;
		// shading += trueLighting(-light2, point, normal, 10) * 0.1;
		shading = min(shading, 1);

		color = ray.color * shading;

	}

	imageStore(renderImage, pixel_coords, vec4(color.xyz, 0));

}