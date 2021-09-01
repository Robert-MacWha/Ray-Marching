#version 430
layout(local_size_x = 16, local_size_y = 16) in;              // define the local work group
layout(rgba32f, binding = 0) uniform image2D renderImage;     // define the format of the output image

uniform float frame;

uniform float near;
uniform float far;
uniform mat4 projection_inv;
uniform int bounce_limit;

const float INF = 1.0/0.0;

//* RAY
struct Ray {
	vec3 origin;
	vec3 direction;
	vec3 energy;
};

Ray createRay(vec3 origin, vec3 direction) {

	Ray ray;

	ray.origin = origin;
	ray.direction = direction;
	ray.energy = vec3(1, 1, 1);

	return ray;

}

//* RAYHIT
struct RayHit {
	
	vec3 position;
	float distance;
	vec3 normal;
	vec3 color;

};

RayHit createRayHit() {
	
	RayHit hit;

	hit.position = vec3(0, 0, 0);
	hit.distance = INF;
	hit.normal = vec3(0, 0, 0);
	hit.color = vec3(0.3, 0.5, 0.9);

	return hit;

}

// creates a ray based on the inverse projection matrix
Ray createEyeRay(vec2 pixel_coords, vec2 work_groups) {

	float aspect_ratio = work_groups.x / work_groups.y;
	
	// screen coords
	vec2 screen_coords = (2 * pixel_coords) / work_groups - 1;

	// calculate origin
	vec3 origin    = (projection_inv * vec4(screen_coords, -1.0, 1.0) * near).xyz;
	vec3 direction = (projection_inv * vec4(screen_coords * (far - near), far + near, far - near)).xyz;
	direction = normalize(direction);

	Ray ray = createRay(origin, direction);

	return ray;

}

//* OBJECTS
struct Sphere { 
	vec3 position; 
	float radius; 
	vec3 color;
	void intersect(Ray ray, inout RayHit bestHit) { 
		// calculate distance along ray where sphere is intersected
		vec3 d = ray.origin - position;
		float p1 = -dot(ray.direction, d);
		float p2sqr = p1 * p1 - dot(d, d) + radius * radius;

		if (p2sqr < 0)
			return;

		float p2 = sqrt(p2sqr);
		float t = p1 - p2 > 0 ? p1 - p2 : p1 + p2;

		if (t > 0 && t < bestHit.distance) {
			bestHit.distance = t;
			bestHit.position = ray.origin + t * ray.direction;
			bestHit.normal = normalize(bestHit.position - position);
			bestHit.color = color;
		}
	} 
};

struct Light  { vec3 position; float intensity; };

Sphere createSphere(vec3 position, float radius, vec3 color) {
	Sphere s;
	s.position = vec3(position.x, -position.yz); s.radius = radius; s.color = color;
	return s;
}

Light createLight(vec3 position, float intensity) {
	Light l;
	l.position = position; l.intensity = intensity;
	return l;
}

//* INTERSECTIONS
void intersectGroundPlane(Ray ray, inout RayHit bestHit) {

	// calculate the distance along the ray where the ground plane is intersected
	float t = -ray.origin.y / ray.direction.y;

	if (t > 0 && t < bestHit.distance) { // if t == 1 then no intersection.  if t < bestHit.distance then intersection is too late
		
		bestHit.distance = t;
		bestHit.position = ray.origin + t * ray.direction;
		bestHit.normal = vec3(0, -1, 0);
		bestHit.color = vec3(0.8, 0.8, 0.8);

	}

}

RayHit trace(Ray ray) {
	
	RayHit bestHit = createRayHit();

	intersectGroundPlane(ray, bestHit);
	float offset = sin((frame) / 20) / 2;
	createSphere(vec3(-5, offset, 10), 2, vec3(0.8, 0.3, 0.3)).intersect(ray, bestHit);

	offset = sin((frame + 30) / 20) / 2;
	createSphere(vec3(4, 2 + offset, 12), 2, vec3(0.4, 0.9, 0.6)).intersect(ray, bestHit);

	offset = sin((frame + 60) / 20) / 2;
	createSphere(vec3(-6, 5 + offset, 20), 1, vec3(0.4, 0.9, 0.9)).intersect(ray, bestHit);

	offset = 0;
	createSphere(vec3(-6, 4 + offset, 9), 1, vec3(0.9, 0.9, 0.3)).intersect(ray, bestHit);

	return bestHit;

}

//* EFFECTS
vec3 shade(inout Ray ray, RayHit hit) {

	if (hit.distance < INF) {

		vec3 specular = vec3(0.6, 6, 0.6);

		// reflect the ray
		ray.origin = hit.position + hit.normal * 0.01;
		ray.direction = reflect(ray.direction, hit.normal);
		ray.energy *= specular;

		return vec3(0, 0, 0);
	}

	ray.energy = vec3(0);

	float sky_tint = dot(ray.direction, vec3(0,-1,0));
	sky_tint = sky_tint * 0.7 + 0.1;
	return vec3(sky_tint * 0.8, sky_tint, 1) * 10;

}

void main() {

	// get x,y position of pixel
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
	ivec2 work_groups = ivec2(gl_NumWorkGroups.xy) * 16;

	// initalize the ray
	Ray ray = createEyeRay(pixel_coords, work_groups);

	// trace and shade
	vec3 result = vec3(0, 0, 0);
	for (int i = 0; i < bounce_limit; i ++) {

		RayHit hit = trace(ray);
		result += ray.energy * shade(ray, hit);

		if (ray.energy.x < 0.01 && ray.energy.y < 0.01 && ray.energy.z < 0.01)
			break;
	}

	// pass the color of the ray's intersection to the texture
	imageStore(renderImage, pixel_coords, vec4(result, 1));

}