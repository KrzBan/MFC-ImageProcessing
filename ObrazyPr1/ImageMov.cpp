#include "pch.h"
#include "ImageMov.h"

#include <assert.h>


CImageMov::CImageMov() : CImage{}
{
}

CImageMov::CImageMov(CImageMov&& other) noexcept
	:CImage{ other }
{
	other.Detach();
}

CImageMov& CImageMov::operator=(CImageMov&& other) noexcept
{
	Destroy();
	CImage::operator=(other);
	other.Detach();

	return *this;
}


Pixel CImageMov::GetPixel(int x, int y) const {
	
	uint8_t* pixelAddr = (uint8_t*)GetPixelAddress(x, y);

	Pixel data;

	switch (GetBPP()) {
	case 1: {
		int bit = x % 8;	//between the rows/collumns, bits are not packed!!!
		if(GetPitch() < 0) bit = 7 - bit;

		std::bitset<8> bits{ *pixelAddr };
		data.Set(255 * bits[bit]);
		break;
	}
	case 8: {
		data.Set(pixelAddr[0]);
		break;
	}
	case 24: {
		data.Set(pixelAddr[0], pixelAddr[1], pixelAddr[2]);
		break;
	}
	default:
	case 32: {
		data.Set(pixelAddr[0], pixelAddr[1], pixelAddr[2], pixelAddr[3]);
		break;
	}
		assert(false && "Unsupported BPP");
		break;
	}

	return data;
}
void CImageMov::SetPixel(int x, int y, const Pixel& color) {

	uint8_t* pixelAddr = (uint8_t*)GetPixelAddress(x, y);

	switch (GetBPP()) {
	case 1: {

		int bit = x % 8;
		if (GetPitch() < 0) bit = 7 - bit;
		std::bitset<8> bits{ *pixelAddr };

		bits[bit] = color.GetR() > 127 ? 1:0;
		*pixelAddr = static_cast<uint8_t>(bits.to_ulong());
		break;
	}
	case 8: {
		pixelAddr[0] = color.GetR();
		break;
	}
	case 24: {
		pixelAddr[0] = color.GetR();
		pixelAddr[1] = color.GetG();
		pixelAddr[2] = color.GetB();
		break;
	}
	case 32: {
		pixelAddr[0] = color.GetR();
		pixelAddr[1] = color.GetG();
		pixelAddr[2] = color.GetB();
		pixelAddr[3] = color.GetA();
		break;
	}
	default:
		assert(false && "Unsupported BPP");
		break;
	}
}

CImageMov CImageMov::CreateBasedOn(const CImageMov& other)
{
	CImageMov output;
	output.Create(other.GetWidth(), other.GetHeight(), other.GetBPP());

	if (other.IsIndexed()) {
		std::unique_ptr<RGBQUAD[]> table = std::make_unique<RGBQUAD[]>(other.GetMaxColorTableEntries());
		other.GetColorTable(0, other.GetMaxColorTableEntries(), table.get());
		output.SetColorTable(0, other.GetMaxColorTableEntries(), table.get());
	}

	return output;
}

CImageMov CImageMov::CreateBasedOn(const CImageMov& other, int width, int height) {
	CImageMov output;
	output.Create(width, height, other.GetBPP());

	if (other.IsIndexed()) {
		std::unique_ptr<RGBQUAD[]> table = std::make_unique<RGBQUAD[]>(other.GetMaxColorTableEntries());
		other.GetColorTable(0, other.GetMaxColorTableEntries(), table.get());
		output.SetColorTable(0, other.GetMaxColorTableEntries(), table.get());
	}

	return output;
}
