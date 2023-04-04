#ifndef _UTIL_MATH_VECTOR_HPP_
#define _UTIL_MATH_VECTOR_HPP_

#include <Windows.h>
#include <cmath>

template <typename T>
T clip_number( const T& n, const T& lower, const T& upper ) {
	return max( lower, min( n, upper ) );
}

typedef float matrix_t [ 3 ] [ 4 ];

class vec2_t;
class vec3_t;

class vec4_t {
public:
	vec4_t();
	vec4_t(vec2_t);
	vec4_t(float, float, float, float);
	~vec4_t();

	float x, y, z, w; // 16 bytes total

	vec4_t& operator+=(const vec4_t& v) {
		x += v.x; y += v.y; z += v.z; w += v.w; return *this;
	}

	vec4_t& operator-=(const vec4_t& v) {
		x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this;
	}

	vec4_t& operator*=(float v) {
		x *= v; y *= v; z *= v; w *= v; return *this;
	}

	vec4_t& operator/=(float v) {
		x /= v; y /= v; z /= v; w /= v; return *this;
	}

	vec4_t operator/(const vec4_t& v) {
		return vec4_t{x / v.x, y / v.y, z / v.z, w / v.w};
	}

	vec4_t operator-(const vec4_t& v) {
		return vec4_t{x - v.x, y - v.y, z - v.z, w - v.w};
	}

	vec4_t operator+(const vec4_t& v) {
		return vec4_t{x + v.x, y + v.y, z + v.z, w + v.w};
	}

	vec4_t operator*(float v) const {
		return vec4_t{x * v, y * v, z * v, w * v};
	}

	vec4_t operator/(float v) const {
		return vec4_t(x / v, y / v, z / v, w / v);
	}

	vec4_t abs_v() {
		return vec4_t(fabs(x), fabs(y), fabs(z), fabs(w));
	};

	void clamp();
	vec4_t normalized();
	float normalize_in_place();
	void normalize();
	float length();
	float length_sqr();
	float dist_to(const vec4_t& other);
	vec4_t cross(vec4_t cross);
};

class vec3_t {
public:
	vec3_t( );
	vec3_t( vec2_t );
	vec3_t( float, float, float );
	~vec3_t( );

	float x, y, z; // 12 bytes total

	bool operator ==(const vec3_t& v) {
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator ==(const vec3_t& v) const {
		return x == v.x && y == v.y && z == v.z;
	}

	vec3_t& operator+=( const vec3_t& v ) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	vec3_t& operator-=( const vec3_t& v ) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	vec3_t& operator*=( float v ) {
		x *= v; y *= v; z *= v; return *this;
	}

	vec3_t& operator/=( float v ) {
		x /= v; y /= v; z /= v; return *this;
	}

	vec3_t operator/( const vec3_t& v ) {
		return vec3_t { x / v.x, y / v.y, z / v.z };
	}

	vec3_t operator-( const vec3_t& v ) {
		return vec3_t { x - v.x, y - v.y, z - v.z };
	}

	vec3_t operator+( const vec3_t& v ) {
		return vec3_t { x + v.x, y + v.y, z + v.z };
	}

	vec3_t operator*( float v ) const {
		return vec3_t { x * v, y * v, z * v };
	}

	vec3_t abs_v() {
		return vec3_t(fabs(x), fabs(y), fabs(z));
	};

	float dot(vec3_t v) {
		return x * v.x + y * v.y + z * v.z;
	}

	void clamp( );
	vec3_t normalized( );
	float normalize_in_place( );
	void normalize( );
	float length( );
	float length_sqr( );
	float dist_to( const vec3_t& other );
	vec3_t cross( vec3_t cross );
};

class vec2_t {
public:
	vec2_t( );
	vec2_t( vec3_t );
	vec2_t( float, float );
	~vec2_t( );

	float x, y; // 8 bytes total

	vec2_t& operator+=( const vec2_t& v ) {
		x += v.x; y += v.y; return *this;
	}

	vec2_t& operator-=( const vec2_t& v ) {
		x -= v.x; y -= v.y; return *this;
	}

	vec2_t& operator*=( float v ) {
		x *= v; y *= v; return *this;
	}

	vec2_t operator-( const vec2_t& v ) {
		return vec2_t { x - v.x, y - v.y };
	}

	vec2_t operator+( const vec2_t& v ) {
		return vec2_t { x + v.x, y + v.y };
	}

	vec2_t operator*( float v ) const {
		return vec2_t { x * v, y * v };
	}

	void clamp( );
	vec2_t normalized( );
	void normalize( );
	float length( );
	float length_sqr( );
};

#endif