//
// Created by Thalins on 10/29/22.
//

#ifndef ORACLES_TYPES_H
#define ORACLES_TYPES_H

#include <cstdint>
#include <glm/glm.hpp>
#include <OtterML/System/Angle.hpp>
#include <OtterML/System/FixedPoint.hpp>
#include <OtterML/System/Matrix.hpp>
#include <OtterML/System/Vector2.hpp>
#include <OtterML/System/Vector3.hpp>

namespace oter
{
// unsigned integers
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// signed integers
using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// floating point
using f32 = float_t;
using f64 = double_t;

// fixed point
using i32x8 = FixedPoint<i32, u8>;

inline i32x8 operator""_x(long double value)
{
	return i32x8(value);
}

inline Angle operator""_d(long double value)
{
	return Angle(value);
}

// Matrix
using m3f = Matrix<f32, 3, 3>;

// Vector2
template <typename T>
using v2 = Vector2<T>;
using v2u = v2<u32>;
using v2i = v2<i32>;
using v2x = v2<i32x8>;
using v2f = v2<f32>;

// Vector3
template <typename T>
using v3 = Vector3<T>;
using v3u = v3<u32>;
using v3i = v3<i32>;
using v3x = v3<i32x8>;
using v3f = v3<f32>;

struct Color
{
public:
	u8 Red   = 0x00;
	u8 Green = 0x00;
	u8 Blue  = 0x00;
	u8 Alpha = 0x00;

	Color() {}

	Color(const u8 red, const u8 green, const u8 blue, const u8 alpha = 0xFF)
		: Red(red), Green(green), Blue(blue), Alpha(alpha) {}

	explicit operator glm::vec3() const
	{
		return glm::vec3(
			static_cast<f32>(this->Red),
			static_cast<f32>(this->Green),
			static_cast<f32>(this->Blue)
		);
	}

	explicit operator glm::vec4() const
	{
		return {
			static_cast<f32>(this->Red),
			static_cast<f32>(this->Green),
			static_cast<f32>(this->Blue),
			static_cast<f32>(this->Alpha),
		};
	}
};

template <typename T>
T Clamp(T min, T value, T max)
{
	return std::min(std::max(min, value), max);
}
}

#endif //ORACLES_TYPES_H
