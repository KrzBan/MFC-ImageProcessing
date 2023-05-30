#pragma once

#include "ImageMov.h"
#include <iostream>
#include <array>

namespace IP {

	struct ToGrayscale {
		static std::pair<int, int> NewSize(int width, int height); 

		static void Process(const CImageCache& input, CImageMov& output);
	};

	struct Erode {
		static std::pair<int, int> NewSize(int width, int height);

		static void Process(const CImageCache& input, CImageMov& output, int range = 1);
	};

	struct Dilate {
		static std::pair<int, int> NewSize(int width, int height);

		static void Process(const CImageCache& input, CImageMov& output, int range = 1);
	};

	struct Affine {
		static std::pair<int, int> NewSize(int width, int height);

		static void Process(const CImageCache& input, CImageMov& output, const std::array<double, 4>& matrix);
	};

	struct EntropyFilt {
		static std::pair<int, int> NewSize(int width, int height);

		static void Process(const CImageCache& input, CImageMov& output, int range);
	};

	struct EntropyFilt2 {
		static std::pair<int, int> NewSize(int width, int height);

		static void Process(const CImageCache& input, CImageMov& output, int range);
	};

	struct MorphGrad {
		static std::pair<int, int> NewSize(int width, int height);

		static void Process(const CImageCache& input, CImageMov& output, int choice);
	};

	struct ClearBorder {
		static std::pair<int, int> NewSize(int width, int height);

		static void Process(const CImageCache& input, CImageMov& output);
	};

	template<typename Fn, typename...Args>
	CImageMov Process(const CImageMov& input, Args&&...args) {
		auto newSize = Fn::NewSize(input.GetWidth(), input.GetHeight());
		CImageMov output = CImageMov::CreateBasedOn(input, newSize.first, newSize.second);

		//Run the function
		CImageCache cache(input);
		Fn::Process(input, output, std::forward<Args>(args)...);

		return output;
	}
}

