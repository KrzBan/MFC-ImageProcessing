#pragma once

#include <iostream>
#include <vector>
#include <assert.h>

#include "Pixel.h"

//using PixelPtr = std::unique_ptr<PixelBase>;

class CImageMov: public CImage{
public:
	CImageMov();

	CImageMov(const CImageMov& other) = delete;
	CImageMov& operator=(const CImageMov& other) = delete;

	CImageMov(CImageMov&& other) noexcept;
	CImageMov& operator=(CImageMov&& other) noexcept;

	Pixel GetPixel(int x,int y) const;
	void SetPixel(int x, int y, const Pixel& color);

	static CImageMov CreateBasedOn(const CImageMov& other);
	static CImageMov CreateBasedOn(const CImageMov& other, int width, int height);
};

class CImageCache {

	std::vector<Pixel> m_Data;
	int m_Width = 0;
	int m_Height = 0;
public:
	CImageCache(const CImageMov& input) : m_Width{ input.GetWidth() }, m_Height{ input.GetHeight() } {
		m_Data.reserve(m_Width* m_Height);
		for (int y = 0; y < m_Height; ++y) {
			for (int x = 0; x < m_Width; ++x) {
				Pixel color = input.GetPixel(x, y);
				m_Data.push_back(color);
			}
		}
	}

	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

	Pixel GetPixel(int x, int y) const {
		auto coord = x + m_Width * y;
		assert(coord >= 0); assert(coord < m_Width*m_Height);
		return m_Data[coord];
	}

	void SetPixel(int x, int y, const Pixel& color) {
		auto coord = x + m_Width * y;
		assert(coord >= 0); assert(coord < m_Width* m_Height);
		m_Data.at(coord).Set(color.r, color.g, color.b);
	}

};

