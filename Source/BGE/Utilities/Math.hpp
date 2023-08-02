/*=============================================================================*
 * Math.hpp - Math tools.
 *
 * Copyright (c) 2023, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *============================================================================*/
#ifndef _BGE_MATH_HPP_
#define _BGE_MATH_HPP_

namespace BGE::Math
{
	// Concept for use with integral types.
	template <typename Type>
	concept Integral = std::is_integral_v<Type>;
	// Concept for use with floating point types.
	template <typename Type>
	concept FloatingPoint = std::is_floating_point_v<Type>;
	// Concept for use with any numeric types.
	template <typename Type>
	concept Numeric = Integral<Type> || FloatingPoint<Type>;
	
	// Math constants:
	inline constexpr double kPI = std::numbers::pi;
	
	// Forward declare Vec2 class:
	template <Numeric Type>
	class Vec2;
	// Declare aliases for Vec2 class:
	using Vec2i  = Vec2<int>;
	using Vec2f  = Vec2<float>;
	using Vec2d  = Vec2<double>;
	
	// Forward declare Vec3 class:
	template <Numeric Type>
	class Vec3;
	// Declare aliases for Vec3 class:
	using Vec3i  = Vec3<int>;
	using Vec3f  = Vec3<float>;
	using Vec3d  = Vec3<double>;
	
	// Forward declare Vec3 class:
	template <Numeric Type>
	class Vec4;
	// Declare aliases for Vec4 class:
	using Vec4i  = Vec4<int>;
	using Vec4f  = Vec4<float>;
	using Vec4d  = Vec4<double>;
	
	// Forward declare Quat (quaternion) class:
	template <Numeric Type>
	class Quat;
	// Declare aliases for Quat (quaternion) class:
	using Quatf  = Quat<float>;
	using Quatd  = Quat<double>;
	
	// Forward declare 2x2 matrix class:
	template <FloatingPoint Type>
	class Mat2x2;
	// Declare aliases for 2x2 matrix class:
	using Mat2x2f = Mat2x2<float>;
	using Mat2x2d = Mat2x2<double>;
	
	// Forward declare 3x3 matrix class:
	template <FloatingPoint Type>
	class Mat3x3;
	// Declare aliases for 3x3 matrix class:
	using Mat3x3f = Mat3x3<float>;
	using Mat3x3d = Mat3x3<double>;
		
	// Forward declare 4x4 matrix class:
	template <FloatingPoint Type>
	class Mat4x4;
	// Declare aliases for 4x4 matrix class:
	using Mat4x4f = Mat4x4<float>;
	using Mat4x4d = Mat4x4<double>;
	// Free functions:

	/**
	 * Vec2 class represents a two component vector. 
	 */
	template <Numeric Type>
	class Vec2
	{
	public:
		static constexpr std::size_t kARRAY_LENGTH = 2;
		using ValueType = Type;
		using TypeArray = std::array<Type, kARRAY_LENGTH>;
	
		Type x, y;
	public:
		constexpr Vec2(Type _x, Type _y) : x(_x), y(_y) { }
		// Initialize both x & y to same value
		constexpr Vec2(Type singular) : x(singular), y(singular) { }
		// Use array for initial values
		constexpr Vec2(const Type (&array)[2]) : x(array[0]), y(array[1]) { }
		Vec2(const Vec2<Type> &) = default;
		Vec2 &operator=(const Vec2<Type> &) = default;
		Vec2(Vec2<Type> &&) noexcept = default;
		Vec2 &operator=(Vec2<Type> &&) noexcept = default;
	public:
		// TODO: See if spaceship operator default generation is semantically correct.
		auto operator<=>(const Vec2<Type> &) const = default;
		Vec2<Type> operator+(void) const { return Vec2<Type>(+x, +y); } // Unary plus
		Vec2<Type> operator+(const Vec2<Type> &rhs) const { return Vec2<Type>(x + rhs.x, y + rhs.y); } // Binary plus
		Vec2<Type> operator+=(const Vec2<Type> &rhs) // Addition assignment
		{
			x += rhs.x; y += rhs.y;
			return Vec2<Type>(*this);
		}
		Vec2<Type> operator-(void) const { return Vec2<Type>(-x, -y); } // Unary minus (negation)
		Vec2<Type> operator-(const Vec2<Type> &rhs) const { return Vec2<Type>(x - rhs.x, y - rhs.y); } // Binary minus
		Vec2<Type> operator-=(const Vec2<Type> &rhs) // Subtraction assignment
		{
			x -= rhs.x; y -= rhs.y;
			return Vec2<Type>(*this);
		}
		Vec2<Type> operator*(const Vec2<Type> &rhs) const { return Vec2<Type>(x * rhs.x, y * rhs.y); } // Binary multiplication
		Vec2<Type> operator*(Type scalar) const { return Vec2<Type>(x * scalar, y * scalar); } // Binary multiplication (scalar)
		Vec2<Type> operator*=(const Vec2<Type> &rhs) // Multiplication assignment
		{
			x *= rhs.x; y *= rhs.y;
			return Vec2<Type>(*this);
		}
		Vec2<Type> operator*=(Type scalar) // Multiplication assignment (scalar)
		{
			x *= scalar; y *= scalar;
			return Vec2<Type>(*this);
		}
		// Division by another vector is not useful in math, scalar division is however.
		Vec2<Type> operator/(Type scalar) const // Binary division (scalar)
		{
			Type newX = x, newY = y;
			if (scalar != 0)
			{
				newX = x / scalar;
				newY = y / scalar;
			}
			return Vec2<Type>(newX, newY);
		}
		Vec2<Type> operator/=(Type scalar) // Division assignment (scalar)
		{
			if (scalar != 0)
			{
				x /= scalar;
				y /= scalar;
			}
			return *this;
		}
		// Remainder division doesn't really have any use.
		//Vec2<Type> operator%(Type scalar) const; // Binary modulus (scalar)
		//Vec2<Type> operator%=(Type scalar); // Modulus assignment (scalar)
		Vec2<Type> &operator++(void) // Prefix addition
		{
			x++; y++;
			return *this;
		}
		Vec2<Type> &operator--(void) // Prefix subtraction
		{
			x--; y--;
			return *this;
		}
		Vec2<Type> operator++(int) const // Postfix addition (beware of copy!)
		{
			auto copyOfThis(*this);
			operator++(); // Call the prefix increment operator
			return copyOfThis;
		}
		Vec2<Type> operator--(int) const // Postfix subtraction (beware of copy!)
		{
			auto copyOfThis(*this);
			operator--();
			return copyOfThis;
		}
		// "Pseudo-cross product" or "cross product magnitude" magnitude of the cross product between the vectors
		Type PsuedoCross(const Vec2<Type> &vec) const { return std::abs<Type>((x * vec.y) - (y * vec.x)); }
		void Normalize(void) // Normalize vector by magnitude
		{
			const Type kMagnitude = Magnitude();
			if (kMagnitude > 0)
			{
				x /= kMagnitude;
				y /= kMagnitude;
			}
		}
		Vec2<Type> Normalized(void) const // Normalize vector by magnitude
		{
			auto copyOfThis(*this);
			copyOfThis.Normalize();
			return copyOfThis;
		}
		Type Magnitude(void) const { return std::abs<Type>(std::sqrt<Type>((x * x) + (y * y))); } // Vector magnitude/length
		void Inverse(void) { -(*this); } // Negate this
		void Translate(const Vec2<Type> &vec) { (*this) = (*this) + vec; } // Add vec to this
		void Scale(const Vec2<Type> &vec) { (*this) = (*this) * vec; } // Multiply this by vec
		void Scale(Type scalar) { (*this) = (*this) * scalar; } // Multiply this by scalar
		Type Distance(const Vec2<Type> &vec) const // Get the distance between this and vec
		{
			const Type kDX = vec.x - x;
			const Type kDY = vec.y - y;
			return Vec2<Type>(kDX, kDY);
		}
		// Modify all elements in the vector to be absolute values
		void AbsoluteValue(void) { x = std::abs<Type>(x); y = std::abs<Type>(y); }
		// Get the sum of all elements in the vector
		Type Summation(Type startValue = 0) const { return (startValue + (x + y)); }
		// Get the product of all elements in the vector
		Type Product(Type startValue = 0) const { return (startValue + (x * y)); }
		Type Dot(const Vec2<Type> &vec) const { return ((x * vec.x) + (y * vec.y)); } // Dot product		
		bool IsOrthogonal(const Vec2<Type> &vec) const // Check if this is orthogonal to vec
		{
			// The condition checks if the dot product is 0 using a floating - point tolerance value.
			return std::abs(Dot(vec)) < 1E-6;
		}
		TypeArray AsArray(void) const { return { x, y }; } // Convert to std::array type
	};
	
	template <Numeric Type>
	class Vec3
	{
	public:
		static constexpr std::size_t kARRAY_LENGTH = 3;
		using ValueType = Type;
		using TypeArray = std::array<Type, kARRAY_LENGTH>;

		Type x, y, z;
	public:
		constexpr Vec3(Type _x, Type _y, Type _z) : x(_x), y(_y), z(_z) { }
		constexpr Vec3(Type singular) : x(singular), y(singular), z(singular) { } // Initialize both x & y to same value
		// Use array for initial values
		constexpr Vec3(const Type (&array)[3]) : x(array[0]), y(array[1]), z(array[2]) { }
		Vec3(const Vec3<Type> &) = default;
		Vec3& operator=(const Vec3<Type> &) = default;
		Vec3(Vec3<Type> &&) noexcept = default;
		Vec3& operator=(Vec3<Type> &&) noexcept = default;
	public:
		// TODO: See if spaceship operator default generation is semantically correct.
		auto operator<=>(const Vec3<Type> &) const = default;
		Vec3<Type> operator+(void) const { return Vec3<Type>(+x, +y, +z); } // Unary plus
		Vec3<Type> operator+(const Vec3<Type> &rhs) const { return Vec3<Type>(x + rhs.x, y + rhs.y, z + rhs.z); } // Binary plus
		Vec3<Type> operator+=(const Vec3<Type> &rhs) // Addition assignment
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}
		Vec3<Type> operator-(void) const { return Vec3<Type>(-x, -y, -z); } // Unary minus (negation)
		Vec3<Type> operator-(const Vec3<Type> &rhs) const { return Vec3<Type>(x - rhs.x, y - rhs.y, z - rhs.z); } // Binary minus
		Vec3<Type> operator-=(const Vec3<Type> &rhs) // Subtraction assignment
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
			return *this;
		}
		Vec3<Type> operator*(const Vec3<Type> &rhs) const // Binary multiplication
		{
			return Vec3<Type>(x * rhs.x, y * rhs.y, z * rhs.z);
		}
		Vec3<Type> operator*(Type scalar) const // Binary multiplication (scalar)
		{
			return Vec3<Type>(x * scalar, y * scalar, z * scalar);
		}
		Vec3<Type> operator*=(const Vec3<Type> &rhs) // Multiplication assignment
		{
			x *= rhs.x; y *= rhs.y; z *= rhs.z;
			return *this;
		}
		Vec3<Type> operator*=(Type scalar) // Multiplication assignment (scalar)
		{
			x *= scalar; y *= scalar; z *= scalar;
			return *this;
		}
		// Division by another vector is not useful in math, scalar division is however.
		Vec3<Type> operator/(Type scalar) const // Binary division (scalar)
		{
			Type newX = x, newY = y, newZ = z;
			if (scalar != 0)
			{
				newX = x / scalar;
				newY = y / scalar;
				newZ = z / scalar;
			}
			return Vec3<Type>(newX, newY, newZ);
		}
		Vec3<Type> operator/=(Type scalar) // Division assignment (scalar)
		{
			if (scalar != 0)
			{
				x /= scalar;
				y /= scalar;
				z /= scalar;
			}
			return *this;
		}
		// Remainder division doesn't really have any use.
		//Vec3<Type> operator%(Type scalar) const; // Binary modulus (scalar)
		//Vec3Type> operator%=(Type scalar); // Modulus assignment (scalar)
		Vec3<Type> &operator++(void) // Prefix addition
		{
			x++; y++; z++;
			return *this;
		}
		Vec3<Type> &operator--(void) // Prefix subtraction
		{
			x--; y--; z--;
			return *this;
		}
		Vec3<Type> operator++(int) const // Postfix addition (beware of copy!)
		{
			auto copyOfThis(*this);
			operator++(); // Call the prefix increment operator
			return copyOfThis;
		}
		Vec3<Type> operator--(int) const // Postfix subtraction (beware of copy!)
		{
			auto copyOfThis(*this);
			operator--();
			return copyOfThis;
		}
		// "Pseudo-cross product" or "cross product magnitude" magnitude of the cross product between the vectors
		Vec3<Type> Cross(const Vec3<Type> &vec) const // Cross multiply this by vec
		{
			return Vec3<Type>((y * vec.z) - (z * vec.y), (z * vec.x) - (x * vec.z), (x * vec.y) - (y * vec.x));
		}
		void Normalize(void) // Normalize vector by magnitude
		{
			const Type kMagnitude = Magnitude();
			if (kMagnitude != 0)
			{
				x /= kMagnitude;
				y /= kMagnitude;
				z /= kMagnitude;
			}
		}
		Vec3<Type> Normalized(void) const // Normalize vector by magnitude
		{
			auto copyOfThis(*this);
			copyOfThis.Normalize();
			return copyOfThis;
		}
		Type Magnitude(void) const { return std::sqrt((x * x) + (y * y) + (z * z)); } // Vector magnitude/length
		void Inverse(void) { (*this) = -(*this); } // Negate this
		void Translate(const Vec3<Type> &vec) { (*this) = (*this) + vec; } // Add vec to this
		void Scale(const Vec3<Type> &vec) { (*this) = (*this) * vec; } // Multiply this by vec
		void Scale(Type scalar) { (*this) = (*this) * scalar; } // Multiply this by scalar
		Type Distance(const Vec3<Type> &vec) const // Get the distance between this and vec
		{
			const Type kDX = x - vec.x;
			const Type kDY = y - vec.y;
			const Type kDZ = z - vec.z;
			return std::sqrt<Type>((kDX * kDX) + (kDY * kDY) + (kDZ * kDZ));
		}
		void AbsoluteValue(void) // Modify all elements in the vector to be absolute values
		{
			x = std::abs<Type>(x);
			y = std::abs<Type>(y);
			z = std::abs<Type>(z);
		}
		Type Summation(Type startValue = 0) const // Get the sum of all elements in the vector
		{
			return (startValue + (x + y + z));
		}
		Type Product(Type startValue = 0) const // Get the product of all elements in the vector
		{
			return Type(startValue + (x * y * z));
		}
		Type Dot(const Vec3<Type> &vec) const // Dot product
		{
			return ((x * vec.x) + (y * vec.y) + (z * vec.z));
		}
		bool IsOrthogonal(const Vec3<Type> &vec) const // Check if this is orthogonal to vec
		{
			return std::abs(Dot(vec)) < 1E-6;
		}
		TypeArray AsArray(void) const { return { x, y, z }; } // Convert to std::array type
	};
	
	template <Numeric Type>
	class Vec4
	{
	public:
		Type x, y, z, w;
	};
	
	template <Numeric Type>
	class Quat
	{
	public:
		static constexpr std::size_t kARRAY_LENGTH = 4;
		using ValueType = Type;
		using TypeArray = std::array<Type, kARRAY_LENGTH>;
	
		Type x, y, z, w;
	public:
		constexpr Quat(void) : x(0), y(0), z(0), w(1) { }
		constexpr Quat(Type _x, Type _y, Type _z, Type _w) : x(_x), y(_y), z(_z), w(_w) { }
		//constexpr Quat(Type singular); // Initialize both x, y, z, w to same value. (This probably isn't applicable)
		// Use array for initial values.
		constexpr Quat(const Type(&array)[4]) : x(array[0]), y(array[1]), z(array[2]), w(array[3]) { }
		Quat(const Quat<Type> &) = default;
		Quat &operator=(const Quat<Type> &) = default;
		Quat(Quat<Type> &&) noexcept = default;
		Quat &operator=(Quat<Type> &&) noexcept = default;
	public:
		// TODO: See if spaceship operator default generation is semantically correct.
		auto operator<=>(const Quat<Type>&) const = default;
		Quat<Type> operator+(void) const { return Quat<Type>(+x, +y, +z, +w); } // Unary plus
		Quat<Type> operator+(const Quat<Type>& rhs) const // Binary plus
		{
			return Quat<Type>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
		}
		Quat<Type> operator+=(const Quat<Type> &rhs) // Addition assignment
		{
			x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
			return *this;
		}
		Quat<Type> operator-(void) const { return Quat<Type>(-x, -y, -z, -w); } // Unary minus (negation)
		Quat<Type> operator-(const Quat<Type> &rhs) const // Binary minus
		{
			return Quat<Type>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
		}
		Quat<Type> operator-=(const Quat<Type> &rhs) // Subtraction assignment
		{
			x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
			return *this;
		}
		Quat<Type> operator*(const Quat<Type> &rhs) const // Binary multiplication
		{
			const Type kX = (w * rhs.x) + (x * rhs.w) + (y * rhs.z) - (z * rhs.y);
			const Type kY = (w * rhs.y) - (x * rhs.z) + (y * rhs.w) + (z * rhs.x);
			const Type kZ = (w * rhs.z) + (x * rhs.y) - (y * rhs.x) + (z * rhs.w);
			const Type kW = (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z);
			return Quat<Type>(kX, kY, kZ, kW);
		}
		Quat<Type> operator*(Type scalar) const; // Binary multiplication (scalar)
		Quat<Type> operator*=(const Quat<Type>& rhs); // Multiplication assignment
		Quat<Type> operator*=(Type scalar); // Multiplication assignment (scalar)
		// Division by another vector is not useful in math, scalar division is however.
		Quat<Type> operator/(Type scalar) const; // Binary division (scalar)
		Quat<Type> operator/=(Type scalar); // Division assignment (scalar)
		// Remainder division doesn't really have any use.
		//Quat<Type> operator%(Type scalar) const; // Binary modulus (scalar)
		//QuatType> operator%=(Type scalar); // Modulus assignment (scalar)
		Quat<Type>& operator++(void); // Prefix addition
		Quat<Type>& operator--(void); // Prefix subtraction
		Quat<Type> operator++(int) const; // Postfix addition (beware of copy!)
		Quat<Type> operator--(int) const; // Postfix subtraction (beware of copy!)
		// "Pseudo-cross product" or "cross product magnitude" magnitude of the cross product between the vectors
		Quat<Type> Cross(const Quat<Type> &vec) const; // Psuedo Cross multiply this by vec
		void Normalize(void); // Normalize vector by magnitude
		Quat<Type> Normalized(void) const; // Normalize vector by magnitude
		Type Magnitude(void) const; // Vector magnitude/length
		Quat<Type> Conjugate(void) const { return Quat<Type>(-x, -y, -z, w); }
		void Inverse(void) // Negate this
		{
			const Type kMagnitude = Magnitude();
			*this = (kMagnitude != 0) ? (Conjugate() / Magnitude) : *this;
		}
		void Translate(const Vec3<Type> &vec); // Add vec to this
		void Scale(const Quat<Type> &vec); // Multiply this by vec
		void Scale(Type scalar); // Multiply this by scalar
		Type Distance(const Vec3<Type> &vec) const; // Get the distance between this and vec
		void AbsoluteValue(void); // Modify all elements in the vector to be absolute values
		Type Summation(Type startValue = 0) const; // Get the sum of all elements in the vector
		Type Product(Type startValue = 0) const; // Get the product of all elements in the vector
		Type Dot(const Quat<Type> &vec) const // Dot product
		{
			return ((x * vec.x) + (y * vec.y) + (z * vec.z) + (w * vec.w));
		}
		bool IsOrthogonal(const Vec3<Type> &vec) const; // Check if this is orthogonal to vec
		TypeArray AsArray(void) const { return { x, y, z, w }; } // Convert to std::array type
		static Quat<Type> Identity(void) { return Quat<Type>(); }
		static Quat<Type> Slerp(const Quat<Type> &start, const Quat<Type> &end, Type interpFactor)
		{
			interpFactor = std::clamp(interpFactor, static_cast<Type>(0), static_cast<Type>(1));
			Type cosTheta = start.Dot(end);
			if (cosTheta < static_cast<Type>(0))
			{
				//end = -end;
				//cosTheta = -cosTheta;
			}
			if (cosTheta > static_cast<Type>(0.9995f))
			{
				//Quat<Type> result = (static_cast<Type>(1) - interpFactor) * start + interpFactor * end;
				//return result.Normalized();
			}
			Type theta = std::acos(cosTheta);
			Type sinTheta = std::sin(theta);
			//const Type kA = std::sin((static_cast<Type>(1) - interpFactor) * theta) / sinTheta;
			//const Type kB = std::sin(interpFactor * theta) / sinTheta;
			//Quat<Type> result = kA * start + kB * end;
			//return result.Normalized();
			return {};
		}
	};
	
	template <FloatingPoint Type>
	class Mat2x2
	{
	public:
	};
	
	template <FloatingPoint Type>
	class Mat3x3
	{
	public:
	};
	
	template <FloatingPoint Type>
	class Mat4x4
	{
	public:
	};
} // End namespace (BGE::Math)

#endif /* !_BGE_MATH_HPP_ */