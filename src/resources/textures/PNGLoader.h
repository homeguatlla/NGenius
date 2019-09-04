#pragma once

#include "../lpng1634/png.h"
#include "../lpng1634/pngstruct.h"
#include "../lpng1634/pnginfo.h"

#include "../../zlib-1.2.11/zlib.h"

/*!
	\class PNGLoader
	\brief
		This class allow the PNG file image load and save.
*/
class PNGLoader
{
	int mX, mY;
	int mWidth, mHeight;
	png_byte mColorType;
	png_byte mBitDepth;

	png_structp mPngPtr;
	png_infop mInfoPtr;
	int mNumberOfPasses;
	png_bytep * mRowPointers;
	bool mHasAlpha;
	int mNumBits;

	unsigned char* mData;

public:
	PNGLoader(void);
	bool ReadPNGFile(const char* file_name);
	bool WritePNGFile(const char* file_name);
	unsigned char* GetData() const { return mData; }
	int GetWidth() const { return mWidth;  }
	int GetHeight() const { return mHeight;  }
	unsigned char GetR(int u, int v) const;
	unsigned char GetG(int u, int v) const;
	unsigned char GetB(int u, int v) const;
	unsigned char GetA(int u, int v) const;
	bool HasAlpha() const { return mHasAlpha; }

	void SetHeight(int height) { mHeight = height;  }
	void SetWidth(int width) { mWidth = width;  }
	void SetData(unsigned char* data) { mData = data; }
	void SetColorType(unsigned char colorType) { mColorType = colorType; }
	void SetBitDepth(unsigned char bitDepth) { mBitDepth = bitDepth;  }

	
	virtual ~PNGLoader(void);
};