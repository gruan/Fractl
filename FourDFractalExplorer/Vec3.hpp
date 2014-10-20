#ifndef VEC3_H
#define VEC3_H

class Vec3{
public:
	float x, y, z;



	Vec3(){
		x = y = z = 0.f;
	}

	Vec3(const Vec3 &v){
		x = v.x;
		y = v.y;
		z = v.z;
	}


	Vec3(const float &f){
		x = y = z = f;
	}

	Vec3(const float _x, const float _y, const float _z){
		x = _x;
		y = _y;
		z = _z;
	}

	float dot(const Vec3 &o) const {
		return x*o.x + y*o.y + z*o.z;
	}

	Vec3 cross(const Vec3 &o) const {
		const float i = y*o.z - z*o.y;
		const float j = z*o.x - x*o.z;
		const float k = x*o.y - y*o.x;

		return Vec3(i,j,k);
	}

	Vec3 normalized(){
		const float total = this->abs();

		const float tx = x / total;
		const float ty = y / total;
		const float tz = z / total;

		return Vec3(tx,ty,tz);
	}
	float absq() const {
		return this->dot(*this);
	}
	float abs() const {
		return sqrtf(absq());
	}

	bool operator==(const Vec3 &o){
		return x==o.x && y==o.y && z==o.z;
	}

	Vec3 operator+(const Vec3 &o) const {
		return Vec3(x+o.x, y+o.y, z+o.z);
	}

	Vec3 operator-(const Vec3 &o) const {
		return Vec3(x-o.x, y-o.y, z-o.z);
	}
	
	Vec3 operator*(const Vec3 &o) const {
		return Vec3(x*o.x, y*o.y, z*o.z);
	}
	
	Vec3 operator/(const Vec3 &o) const {
		return Vec3(x/o.x, y/o.y, z/o.z);
	}

	void operator=(const Vec3 &o){
		x = o.x; y = o.y; z = o.z;
	}

	void operator+=(const Vec3 &o){
		x+=o.x; y+=o.y; z+=o.z;
	}

	void operator-=(const Vec3 &o){
		x-=o.x; y-=o.y; z-=o.z;
	}
	
	void operator*=(const Vec3 &o){
		x*=o.x; y*=o.y; z*=o.z;
	}
	
	void operator/=(const Vec3 &o){
		x/=o.x; y/=o.y; z/=o.z;
	}
};

#endif