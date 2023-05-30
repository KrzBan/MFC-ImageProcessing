#pragma once

#include <bitset>
#include <iostream>
#include <algorithm>
#include <numeric>

struct Pixel {
	Pixel() {}
	Pixel(const Pixel& other) : r{ other.r }, g{ other.g }, b{ other.b }{}
	Pixel(uint8_t val): Pixel(val,val,val) {}
	Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0) { Set(r, g, b); }

	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 255;

	uint8_t GetR() const {
		return r;
	}
	uint8_t GetG() const {
		return g;
	}
	uint8_t GetB() const {
		return b;
	}

	uint8_t GetA() const {
		return a;
	}

	void SetR(uint8_t r) {
		this->r = r;
	}
	void SetG(uint8_t g) {
		this->g = g;
	}
	void SetB(uint8_t b) {
		this->b = b;
	}
	void SetA(uint8_t a) {
		this->a = a;
	}

	void Set(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
		SetR(r); SetG(g); SetB(b); SetA(a);
	}
	void Set(uint8_t col) {
		Set(col,col,col);
	}

	double Luminance() const {
		return 0.21 * r + 0.72 * g + 0.07 * b;
	}

	Pixel& operator+=(const Pixel& rhs) {
		r = std::clamp(static_cast<int>(r) + rhs.r, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		g = std::clamp(static_cast<int>(g) + rhs.g, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		b = std::clamp(static_cast<int>(b) + rhs.b, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		a = std::clamp(static_cast<int>(a) + rhs.a, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		return *this;
	}
	Pixel& operator-=(const Pixel& rhs) {
		r = std::clamp(static_cast<int>(r) - rhs.r, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		g = std::clamp(static_cast<int>(g) - rhs.g, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		b = std::clamp(static_cast<int>(b) - rhs.b, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		a = std::clamp(static_cast<int>(a) - rhs.a, static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		return *this;
	}
	Pixel& operator*=(const double& rhs) {
		r = std::clamp(static_cast<int>(r * rhs), static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		g = std::clamp(static_cast<int>(g * rhs), static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		b = std::clamp(static_cast<int>(b * rhs), static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		a = std::clamp(static_cast<int>(a * rhs), static_cast<int>((std::numeric_limits<uint8_t>::min)()), static_cast<int>((std::numeric_limits<uint8_t>::max)()));
		return *this;
	}
};

inline Pixel operator+(Pixel lhs, const Pixel& rhs) {
	return lhs += rhs;
}
inline Pixel operator-(Pixel lhs, const Pixel& rhs) {
	return lhs -= rhs;
}
inline Pixel operator*(Pixel lhs, const double& rhs) {
	return lhs *= rhs;
}

inline bool operator==(const Pixel& lhs, const Pixel& rhs) {
	return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

inline bool operator!=(const Pixel& lhs, const Pixel& rhs) {
	return !(lhs==rhs);
}

inline bool operator<(const Pixel& lhs, const Pixel& rhs) {
	return lhs.Luminance() < rhs.Luminance();
}
