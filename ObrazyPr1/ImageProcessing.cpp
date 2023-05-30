#include "pch.h"
#include "ImageProcessing.h"

#include <iostream>
#include <vector>
#include <assert.h>
#include <map>

namespace IP {

	std::pair<int, int> ToGrayscale::NewSize(int width, int height){
		return std::pair<int, int>(width, height);
	}

	std::pair<int, int> Erode::NewSize(int width, int height) {
		return std::pair<int, int>(width, height);
	}

	std::pair<int, int> Dilate::NewSize(int width, int height) {
		return std::pair<int, int>(width, height);
	}

	std::pair<int, int> Affine::NewSize(int width, int height) {
		return std::pair<int, int>(width, height);
	}

	std::pair<int, int> EntropyFilt::NewSize(int width, int height) {
		return std::pair<int, int>(width, height);
	}

	std::pair<int, int> EntropyFilt2::NewSize(int width, int height) {
		return std::pair<int, int>(width, height);
	}

	std::pair<int, int> MorphGrad::NewSize(int width, int height){
		return std::pair<int, int>(width, height);
	}

	std::pair<int, int> ClearBorder::NewSize(int width, int height) {
		return std::pair<int, int>(width, height);
	}

	//-------Small algorithms, Grayscale, Dilate, Erode-------

	void ToGrayscale::Process(const CImageCache& input, CImageMov& output){

		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {

				Pixel color = input.GetPixel(x, y);
				uint8_t gray = 0.299 * color.GetR() + color.GetG() * 0.587 + color.GetB() * 0.114;
				Pixel newPixel{ gray };
				
				output.SetPixel(x, y, newPixel);
			}
		}
	}

	void Erode::Process(const CImageCache& input, CImageMov& output, int range){
		//uses 3x3 array of ones

		//For each pixel
		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {

				std::array<std::vector<uint8_t>, 3> colors;
				//For each neighbour
				for (int i = -range; i <= range; ++i) {
					for (int j = -range; j <= range; ++j) {
						auto adjX = x + i;
						auto adjY = y + j;
						if (adjX < 0 || adjX >= input.GetWidth() || 
							adjY < 0 || adjY >= input.GetHeight()) 
						{
							for (auto& vec : colors) vec.push_back(0);
						}
						else 
						{
							Pixel color = input.GetPixel(adjX, adjY);
							
							colors.at(0).push_back(color.r);
							colors.at(1).push_back(color.g);
							colors.at(2).push_back(color.b);
						}
					}
				}
				//Resolve final pixel color
				auto redIter = std::min_element(colors.at(0).begin(), colors.at(0).end());
				auto greenIter = std::min_element(colors.at(1).begin(), colors.at(1).end());
				auto blueIter = std::min_element(colors.at(2).begin(), colors.at(2).end());

				output.SetPixel(x, y, { *redIter, *greenIter, *blueIter });
			}
		}
	}

	void Dilate::Process(const CImageCache& input, CImageMov& output, int range){
		//uses 3x3 array of ones

		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {

				std::array<std::vector<uint8_t>, 3> colors;

				for (int i = -range; i <= range; ++i) {
					for (int j = -range; j <= range; ++j) {
						auto adjX = x + i;
						auto adjY = y + j;
						if (adjX < 0 || adjX >= input.GetWidth() ||
							adjY < 0 || adjY >= input.GetHeight())
						{
							for (auto& vec : colors) vec.push_back(0);
						}
						else 
						{
							Pixel color = input.GetPixel(adjX, adjY);
							colors.at(0).push_back(color.r);
							colors.at(1).push_back(color.g);
							colors.at(2).push_back(color.b);
						}
					}
				}
				//Resolve final pixel color
				auto redIter =	std::max_element(colors.at(0).begin(), colors.at(0).end());
				auto greenIter =std::max_element(colors.at(1).begin(), colors.at(1).end());
				auto blueIter = std::max_element(colors.at(2).begin(), colors.at(2).end());

				output.SetPixel(x, y, { *redIter, *greenIter, *blueIter });
			}
		}
	}

	//-------Affine transform-------

	void Affine::Process(const CImageCache& input, CImageMov& output, const std::array<double, 4>& matrix){
		//matrix is already valid and inversible
		double det = matrix[0] * matrix[3] - matrix[1] * matrix[2];
		std::array<double, 4> inverse = { matrix[3], -matrix[1], -matrix[2], matrix[0] };
		for (auto& val : inverse) val /= det;

		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {

				auto newX = x * inverse[0] + y * inverse[1];
				auto newY = x * inverse[2] + y * inverse[3];
				if (newX >= 0 && newX < input.GetWidth() && newY >= 0 && newY < input.GetHeight()) {
					Pixel color = input.GetPixel(newX, newY);
					output.SetPixel(x, y, color);
				}
			}
		}
	}

	//-------Entropy filter-------

	void EntropyFilt::Process(const CImageCache& input, CImageMov& output, int range){
		int N = 3 * (2 * range + 1) * (2 * range + 1);
		std::vector<double> results{};
		results.reserve(input.GetWidth() * input.GetHeight());

		//For each pixel
		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {

				std::array<uint32_t, 256> hist{};	
				//For each neighbour
				for (int i = -range; i <= range; ++i) {
					for (int j = -range; j <= range; ++j) {
						auto adjX = x + i; 
						if (adjX < 0 || adjX >= input.GetWidth()) adjX = x - i;
						auto adjY = y + j;
						if (adjY < 0 || adjY >= input.GetHeight()) adjY = y - j;

						Pixel color = input.GetPixel(adjX, adjY);
						++hist[color.r]; ++hist[color.g]; ++hist[color.b];	
					}
				}
				
				double E = 0;
				for (int i = 0; i < 256; ++i) {
					double prob = static_cast<double>(hist[i]) / N;
					auto tempE = -std::log2(prob) * prob;
					if (!std::isnan(tempE)) {
						E += tempE;
					}
				}

				results.push_back(E);
			}
		}
		//Normalize
		auto [pMin, pMax] = std::minmax_element(results.begin(), results.end());
		if (*pMin == *pMax) return;

		auto resIter = results.begin();
		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {
				uint8_t normalized = (*resIter++ - *pMin) * (255) / (*pMax - *pMin);
	
				output.SetPixel(x, y, { normalized });
			}
		}
	}

	void EntropyFilt2::Process(const CImageCache& input, CImageMov& output, int range){
		int N = (2 * range + 1) * (2 * range + 1); //map ver.
		std::vector<double> results{};
		results.reserve(input.GetWidth() * input.GetHeight());

		//For each pixel
		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {

				std::map<Pixel, uint32_t> hist{};	//map ver.
				//For each neighbour
				for (int i = -range; i <= range; ++i) {
					for (int j = -range; j <= range; ++j) {
						auto adjX = x + i;
						if (adjX < 0 || adjX >= input.GetWidth()) adjX = x - i;
						auto adjY = y + j;
						if (adjY < 0 || adjY >= input.GetHeight()) adjY = y - j;

						Pixel color = input.GetPixel(adjX, adjY);
						++hist[color];	//map ver.
					}
				}
				double E = 0;
				for (auto& [key, val] : hist) {
					double prob = static_cast<double>(val) / N;
					auto tempE = -std::log2(prob) * prob;
					if (!std::isnan(tempE)) {
						E += tempE;
					}
				}

				results.push_back(E);
			}
		}
		//Normalize
		auto [pMin, pMax] = std::minmax_element(results.begin(), results.end());
		if (*pMin == *pMax) return;

		auto resIter = results.begin();
		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {
				uint8_t normalized = (*resIter++ - *pMin) * (255) / (*pMax - *pMin);

				output.SetPixel(x, y, { normalized });
			}
		}
	}

	//-------Morphological gradient-------

	void MorphGrad::Process(const CImageCache& input, CImageMov& output, int choice)
	{
		switch (choice) {
		case 0: {
			//Input image - eroded image
			CImageMov erodeOut = CImageMov::CreateBasedOn(output);
			Erode::Process(input, erodeOut);
			for (int x = 0; x < output.GetWidth(); ++x) {
				for (int y = 0; y < output.GetHeight(); ++y) {
					Pixel color = input.GetPixel(x, y) - erodeOut.GetPixel(x, y);
					output.SetPixel(x, y, color);
				}
			}
			break;
		}
		case 1: {
			//Dilated image - input image
			CImageMov dilateOut = CImageMov::CreateBasedOn(output);
			Dilate::Process(input, dilateOut);
			for (int x = 0; x < output.GetWidth(); ++x) {
				for (int y = 0; y < output.GetHeight(); ++y) {
					Pixel color = dilateOut.GetPixel(x, y) - input.GetPixel(x, y);
					output.SetPixel(x, y, color);
				}
			}
			break;
		}
		case 2: {
			//Dilated image - eroded image
			CImageMov dilateOut = CImageMov::CreateBasedOn(output);
			Dilate::Process(input, dilateOut);
			CImageMov erodeOut = CImageMov::CreateBasedOn(output);
			Erode::Process(input, erodeOut);
			for (int x = 0; x < output.GetWidth(); ++x) {
				for (int y = 0; y < output.GetHeight(); ++y) {
					Pixel color = dilateOut.GetPixel(x, y) - erodeOut.GetPixel(x, y);
					output.SetPixel(x, y, color);
				}
			}
			break;
		}
		default: {
			assert(false && "ERROR::MORPHGRAD::Unsupported choice!");
		}
		}
	}

	//-------Clear border objects-------

	void ClearBorder::Process(const CImageCache& input, CImageMov& output){

		CImageCache marker(output);
		
		//Copy borders only
		for (int x = 0; x < output.GetWidth(); ++x) {
			Pixel colorUp = input.GetPixel(x, 0);
			Pixel colorDown = input.GetPixel(x, output.GetHeight()-1);
			marker.SetPixel(x, 0, colorUp);
			marker.SetPixel(x, output.GetHeight() - 1, colorDown);
		}
		for (int y = 0; y < output.GetHeight(); ++y) {
			Pixel colorLeft = input.GetPixel(0, y);
			Pixel colorRight = input.GetPixel(output.GetWidth() - 1, y);
			marker.SetPixel(0, y, colorLeft);
			marker.SetPixel(output.GetWidth() - 1, y, colorRight);
		}

		//Dilate (bound by input image), continue, until no change occurs
		while (true) {
			CImageMov dilatedIm = CImageMov::CreateBasedOn(output);
			Dilate::Process(marker, dilatedIm);
			CImageCache dilated{ dilatedIm };
			
			//Keep only pixels shared between dilated and input image
			bool areSame = true;
			for (int x = 0; x < output.GetWidth(); ++x) {
				for (int y = 0; y < output.GetHeight(); ++y) {
					Pixel original = input.GetPixel(x, y);
					Pixel constructed = dilated.GetPixel(x, y);
					if (original < constructed) dilated.SetPixel(x, y, {original});

					if (dilated.GetPixel(x, y) != marker.GetPixel(x, y)) areSame = false;
				}
			}

			//no change occured, we're done
			if (areSame) break;

			//Save current state into prevIteration
			for (int x = 0; x < output.GetWidth(); ++x) {
				for (int y = 0; y < output.GetHeight(); ++y) {
					Pixel color = dilated.GetPixel(x, y);
					marker.SetPixel(x, y, color);
				}
			}
		}

		for (int x = 0; x < output.GetWidth(); ++x) {
			for (int y = 0; y < output.GetHeight(); ++y) {
				Pixel color = input.GetPixel(x, y) - marker.GetPixel(x, y);
				output.SetPixel(x, y, color);
			}
		}
	}

}