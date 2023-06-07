//
// Created by Thalins on 1/23/23.
//

#ifndef ORACLES_VECTOR3_H
#define ORACLES_VECTOR3_H

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <yaml-cpp/yaml.h>

namespace oracle_old
{
template <typename T>
struct Vector3
{
public:
	T X = static_cast<T>(0);
	T Y = static_cast<T>(0);
	T Z = static_cast<T>(0);

	/**
	 * \brief Default constructor
	 *
	 * Creates a Vector3(0, 0, 0)
	 */
	Vector3() { }

	/**
	 * \brief Construct a Vector3 from coordinates
	 * \param x X coordinate
	 * \param y Y coordinate
	 * \param z Z coordinate
	 */
	Vector3(T x, T y, T z) : X(x), Y(y), Z(z) { }

	/**
	 * \brief Construct a Vector3 from a scalar value
	 * \param scalar Scalar value for all three coordinates
	 */
	explicit Vector3(T scalar) : X(scalar), Y(scalar), Z(scalar) { }

	/**
	 * \brief Construct a Vector3 from a different Vector3
	 *
	 * Does not replace copy constructor
	 * \param other Vector to convert
	 */
	template <typename U>
	explicit Vector3(const Vector3<U>& other) : X(static_cast<T>(other.X)), Y(static_cast<T>(other.Y)),
	                                            Z(static_cast<T>(other.Z)) { }

	/**
	 * \brief Construct a Vector3 from a glm::vec3
	 * \param other glm::vec3 to convert
	 */
	explicit Vector3(const glm::vec3& other) : X(static_cast<T>(other.x)), Y(static_cast<T>(other.y)),
	                                           Z(static_cast<T>(other.z)) { }

	/**
	 * \brief Construct a Vector3 from a glm::uvec3
	 * \param other glm::uvec3 to convert
	 */
	explicit Vector3(const glm::uvec3& other) : X(static_cast<T>(other.x)), Y(static_cast<T>(other.y)),
	                                            Z(static_cast<T>(other.z)) { }

	/**
	 * \brief Construct a Vector3 from a glm::ivec3
	 * \param other glm::ivec3 to convert
	 */
	explicit Vector3(const glm::ivec3& other) : X(static_cast<T>(other.x)), Y(static_cast<T>(other.y)),
	                                            Z(static_cast<T>(other.z)) { }

	/**
	 * \brief Convert to a glm::vec3
	 */
	explicit operator glm::vec3() const
	{
		return glm::vec3(
			static_cast<float>(this->X),
			static_cast<float>(this->Y),
			static_cast<float>(this->Z)
		);
	}

	/**
	 * \brief Convert to a glm::uvec3
	 */
	explicit operator glm::uvec3() const
	{
		return glm::uvec3(
			static_cast<uint32_t>(this->X),
			static_cast<uint32_t>(this->Y),
			static_cast<uint32_t>(this->Z)
		);
	}

	/**
	 * \brief Convert to a glm::ivec3
	 */
	explicit operator glm::ivec3() const
	{
		return glm::ivec3(
			static_cast<int32_t>(this->X),
			static_cast<int32_t>(this->Y),
			static_cast<int32_t>(this->Z)
		);
	}

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;

	T GetValue(const uint8_t index) const
	{
		switch (index)
		{
		case 0:
			return this->X;
		case 1:
			return this->Y;
		case 2:
			return this->Z;
		default:
			spdlog::error("Vector3 only contains 3 values.");
		}
		return 0.f;
	}

	bool operator==(const Vector3& right) const
	{
		return this->X == right.X && this->Y == right.Y && this->Z == right.Z;
	}

	T& operator[](const uint8_t& index)
	{
		switch (index)
		{
		case 0:
			return this->X;
		case 1:
			return this->Y;
		case 2:
			return this->Z;
		default:
			throw std::out_of_range("Vector3::operator[] only contains 3 values.");
		}
	}

	T GetLength()
	{
		static_assert(std::is_floating_point_v<T>, "Vector3::GetLength() is only supported for floating point types.");

		return static_cast<T>(std::sqrt(this->X * this->X + this->Y * this->Y + this->Z * this->Z));
	}

	Vector3<T> GetNormalized()
	{
		static_assert(std::is_floating_point_v<T>,
			"Vector3::GetNormalized() is only supported for floating point types.");

		assert(*this != Vector3<T>());
		return *this / this->GetLength();
	}

	constexpr T Dot(const Vector3& rhs)
	{
		return this->X * rhs.X + this->Y * rhs.Y + this->Z * rhs.Z;
	}

	constexpr T Cross(const Vector3& rhs)
	{
		return this->X * rhs.X - this->Y * rhs.Y - this->Z * rhs.Z;
	}
};

// Scalar operations
template <typename T>
Vector3<T> operator+(const Vector3<T>& left, const T& right)
{
	return Vector3<T>(left.X + right, left.Y + right, left.Z + right);
}

template <typename T1, typename T2>
auto operator+(const Vector3<T1>& left, const T2& right) -> Vector3<decltype(left.X + right)>
{
	using R = decltype(left.X + right);
	return Vector3<R>(left.X + right, left.Y + right, left.Z + right);
}

template <typename T>
Vector3<T>& operator+=(Vector3<T>& left, const T& right)
{
	left.X += right;
	left.Y += right;
	left.Z += right;
	return left;
}

template <typename T>
Vector3<T> operator-(const Vector3<T>& left, const T& right)
{
	return Vector3<T>(left.X - right, left.Y - right, left.Z + right);
}

template <typename T1, typename T2>
auto operator-(const Vector3<T1>& left, const T2& right) -> Vector3<decltype(left.X - right)>
{
	using R = decltype(left.X - right);
	return Vector3<R>(left.X - right, left.Y - right, left.Z + right);
}

template <typename T>
Vector3<T>& operator-=(Vector3<T>& left, const T& right)
{
	left.X -= right;
	left.Y -= right;
	left.Z -= right;
	return left;
}

template <typename T>
Vector3<T> operator*(const Vector3<T>& right, const T& left)
{
	return Vector3<T>(right.X * left, right.Y * left, right.Z * left);
}

template <typename T>
Vector3<T> operator*(const T& left, const Vector3<T>& right)
{
	return Vector3<T>(right.X * left, right.Y * left, right.Z * left);
}

template <typename T1, typename T2>
auto operator*(const Vector3<T1>& left, const T2& right) -> Vector3<decltype(left.X * right)>
{
	using R = decltype(left.X * right);
	return Vector3<R>(left.X * right, left.Y * right, left.Z * right);
}

template <typename T1, typename T2>
auto operator*(const T2& left, const Vector3<T1>& right) -> Vector3<decltype(right.X * left)>
{
	using R = decltype(right.X * left);
	return Vector3<R>(right.X * left, right.Y * left, right.Z * left);
}

template <typename T>
Vector3<T>& operator*=(Vector3<T>& left, const T& right)
{
	left.X *= right;
	left.Y *= right;
	left.Z *= right;
	return left;
}

template <typename T>
Vector3<T> operator/(const Vector3<T>& left, const T& right)
{
	return Vector3<T>(left.X / right, left.Y / right, left.Z / right);
}

template <typename T1, typename T2>
auto operator/(const Vector3<T1>& left, const T2& right) -> Vector3<decltype(left.X / right)>
{
	using R = decltype(left.X / right);
	return Vector3<R>(left.X / right, left.Y / right, left.Z / right);
}

template <typename T>
Vector3<T>& operator/=(Vector3<T>& left, const T& right)
{
	left.X /= right;
	left.Y /= right;
	left.Z /= right;
	return left;
}

// Vector operations
template <typename T>
Vector3<T> operator+(const Vector3<T>& left, const Vector3<T>& right)
{
	return Vector3<T>(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
}

template <typename T1, typename T2>
auto operator+(const Vector3<T1>& left, const Vector3<T2>& right) -> Vector3<decltype(left.X + right.X)>
{
	using R = decltype(left.X + right.X);
	return Vector3<R>(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
}

template <typename T>
Vector3<T>& operator+=(Vector3<T>& left, const Vector3<T>& right)
{
	left.X += right.X;
	left.Y += right.Y;
	left.Z += right.Z;
	return left;
}

template <typename T1, typename T2>
Vector3<T1>& operator+=(Vector3<T1>& left, const Vector3<T2>& right)
{
	left.X += right.X;
	left.Y += right.Y;
	left.Z += right.Z;
	return left;
}

template <typename T>
Vector3<T> operator-(const Vector3<T>& left, const Vector3<T>& right)
{
	return Vector3<T>(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
}

template <typename T1, typename T2>
auto operator-(const Vector3<T1>& left, const Vector3<T2>& right) -> Vector3<decltype(left.X - right.X)>
{
	using R = decltype(left.X - right.X);
	return Vector3<R>(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
}

template <typename T>
Vector3<T>& operator-=(Vector3<T>& left, const Vector3<T>& right)
{
	left.X -= right.X;
	left.Y -= right.Y;
	left.Z -= right.Z;
	return left;
}

template <typename T1, typename T2>
Vector3<T1>& operator-=(Vector3<T1>& left, const Vector3<T2>& right)
{
	left.X -= right.X;
	left.Y -= right.Y;
	left.Z -= right.Z;
	return left;
}

template <typename T>
const Vector3<T> Vector3<T>::Zero(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
template <typename T>
const Vector3<T> Vector3<T>::UnitX(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
template <typename T>
const Vector3<T> Vector3<T>::UnitY(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
template <typename T>
const Vector3<T> Vector3<T>::UnitZ(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
}

template <typename T>
struct YAML::convert<oter::Vector3<T>>
{
	static Node encode(const oter::Vector3<T>& rhs)
	{
		Node node;
		node["x"] = rhs.X;
		node["y"] = rhs.Y;
		node["z"] = rhs.Z;
		return node;
	}

	static bool decode(const Node& node, oter::Vector3<T>& rhs)
	{
		if (node.IsSequence())
			return decode_sequence(node, rhs);
		if (node.IsMap())
			return decode_map(node, rhs);

		return false;
	}

private:
	static bool decode_sequence(const Node& node, oter::Vector3<T>& rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
			return false;

		rhs.X = node[0].as<T>();
		rhs.Y = node[1].as<T>();
		rhs.Z = node[2].as<T>();
		return true;
	}

	static bool decode_map(const Node& node, oter::Vector3<T>& rhs)
	{
		if (!node.IsMap() || node.size() != 3)
			return false;

		rhs.X = node["x"].as<T>();
		rhs.Y = node["y"].as<T>();
		rhs.Z = node["z"].as<T>();
		return true;
	}
};

template <typename T>
std::ostream& operator <<(std::ostream& os, const oter::Vector3<T>& vec)
{
	fmt::format_to(std::ostream_iterator<char>(os), "[ {}, {}, {} ]", vec.X, vec.Y, vec.Z);
	return os;
}

#endif // ORACLES_VECTOR3_H
