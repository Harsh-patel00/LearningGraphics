#include <iostream>
#include <vector>
#include <fstream>
#include <memory>

#include "Helpers/Vec3.h"

void GenerateImageData(unsigned int, unsigned int, Color **);
void CreateImage(unsigned int, unsigned int, Color **);


int main()
{
	// Image

	const unsigned int img_width = 256;
	const unsigned int img_height = 256;

	Color **image = new Color*[img_height];

	for (unsigned int i = 0; i < img_height; i++)
	{
		image[i] = new Color[img_width];
	}


	// Color image[img_height][img_width] = Color[][img_width]{};

	// Pass Image's address in order to modify the value of image and not it's copy
	GenerateImageData(img_width, img_height, image);

	image[5][5] = Vec3(0, 255, 0);

	// Pass image as a const reference so that we don't modify image's value accidentally
	CreateImage(img_width, img_height, image);

}

void GenerateImageData(unsigned int width, unsigned int height, Color **img)
{
	for (unsigned int i = 0; i < width; i++)
	{
		std::cerr << "\rScanline Remaining : " << width - i - 1 << ' ' << std::flush;

		for (unsigned int j = 0; j < height; j++)
		{
			// auto r = double(j) / (width - 1) * 255; // Increase Red brightness from left to right (0->255)
			// auto g = double(width - i) / (width - 1) * 255; // Increase Green brightness from bottom to top (0->255)
			// auto b = .25f * 255; // Blue with 25% brightness

			// img[i][j] = { (float)r, (float)g, (float)b };
			img[i][j] = { 0.f, 0.f, 0.f};
		}
	}

	std::cerr << "\nDone.\n";

}

void CreateImage(unsigned int width, unsigned int height, Color **img)
{
	// File Handling

	std::fstream image;

	try
	{
		image.open("Images/image.ppm", std::ios::out);
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}

	// Render

	image << "P3\n" << width << ' ' << height << "\n255\n";

	for (unsigned int i = 0; i < width; i++)
	{
		std::cerr << "\rWriting to image file : " << width - i - 1 << ' ' << std::flush;

		for (unsigned int j = 0; j < height; j++)
		{	
			image << img[i][j].r << ' ' << img[i][j].g << ' ' << img[i][j].b << "\n";
		}
	}

	std::cerr << "\nDone.\n";
}
