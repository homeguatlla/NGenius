#include "stdafx.h"
#include <stdlib.h>
#include "PNGLoader.h"
#include "../../utils/Log.h"
#include "../Memory.h"

#include <cstring>
#include <iostream>

PNGLoader::PNGLoader(void) : mHasAlpha(false), mNumBits(0)
{
	mData = nullptr;
}

PNGLoader::~PNGLoader(void)
{
	delete mData;
}

bool PNGLoader::ReadPNGFile(const char* file_name)
{
	Log(Log::LOG_INFO) << file_name << "\n";
	
	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp) {
		return false;
	}

	png_bytep header;
	header = new(png_byte);    // 8 is the maximum size that can be checked

	if (header == nullptr)
	{
		fclose(fp);
		return false;
	}

	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8)){
		fclose(fp);
		return false;
	}

	/* initialize stuff */
	mPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!mPngPtr) {
		fclose(fp);
		return false;
	}

	mInfoPtr = png_create_info_struct(mPngPtr);
	if (!mInfoPtr) {
		fclose(fp);
		return false;
	}

	if (setjmp(png_jmpbuf(mPngPtr))){
		fclose(fp);
		return false;
	}
	
	png_init_io(mPngPtr, fp);
	png_set_sig_bytes(mPngPtr, 8);

	png_read_info(mPngPtr, mInfoPtr);

	mWidth = png_get_image_width(mPngPtr, mInfoPtr);
	mHeight = png_get_image_height(mPngPtr, mInfoPtr);
	mColorType = png_get_color_type(mPngPtr, mInfoPtr);
	mBitDepth = png_get_bit_depth(mPngPtr, mInfoPtr);
	
	
	switch (mInfoPtr->color_type) {
	case PNG_COLOR_TYPE_RGBA:
		mHasAlpha = true;
		break;
	case PNG_COLOR_TYPE_RGB:
		mHasAlpha = false;
		break;
	default:
		png_destroy_read_struct(&mPngPtr, &mInfoPtr, NULL);
		fclose(fp);
		return false;
	}

	mNumberOfPasses = png_set_interlace_handling(mPngPtr);
	png_read_update_info(mPngPtr, mInfoPtr);

	/* read file */
	if (setjmp(png_jmpbuf(mPngPtr))){
		fclose(fp);
		return false;
	}

	mRowPointers = (png_bytep*)malloc(sizeof(png_bytep) * mHeight);
	for (mY = 0; mY < mHeight; mY++)
		mRowPointers[mY] = (png_byte*)malloc(png_get_rowbytes(mPngPtr, mInfoPtr));

	png_read_image(mPngPtr, mRowPointers);
	
	long	imageSize;		// size of PNG image
	int colorMode = 3;			// 4 for RGBA, 3 for RGB
	unsigned char *imageData;	// the PNG data

	if (mHasAlpha)
	{
		colorMode = 4;
	}

	// allocate memory for image data

	//allocate memory
	
	//std::cout << colorMode;
	//std::cout << mBitDepth;
	mNumBits = (colorMode * mBitDepth / 8);
	if (mNumBits == 1) mNumBits = 3;

	if (mData != nullptr)
	{
		delete mData;
	}

	mData = DBG_NEW unsigned char[mWidth * mHeight * mNumBits];

	// read image data
	//imageData = (unsigned char*) malloc( sizeof(unsigned char)*imageSize );
	unsigned int row_bytes = png_get_rowbytes(mPngPtr, mInfoPtr);
	imageData = (unsigned char*)malloc(row_bytes * mHeight);
	//png_bytepp mRowPointers = png_get_rows(mPngPtr, mInfoPtr);  
	for (int i = 0; i < mHeight; i++) {
		// note that png is ordered top to  
		// bottom, but OpenGL expect it bottom to top  
		// so the order or swapped  
		memcpy(imageData + (row_bytes * (mHeight - 1 - i)), mRowPointers[i], row_bytes);
	}

	imageSize = row_bytes * mHeight;
	memcpy(mData, imageData, imageSize);

	/* read rest of file, and get additional chunks in mInfoPtr - REQUIRED */
	png_read_end(mPngPtr, mInfoPtr);

	/* clean up after the read, and free any memory allocated - REQUIRED */
	png_destroy_read_struct(&mPngPtr, &mInfoPtr, (png_infopp)NULL);

	fclose(fp);
	delete(imageData);
	imageData = NULL;
	//delete header;

	for (mY = 0; mY < mHeight; mY++)
		delete mRowPointers[mY];
	
	delete mRowPointers;

	return true;
}

unsigned char PNGLoader::GetR(int u, int v) const
{
	return mData[(u + v * mWidth) * mNumBits];
}

unsigned char PNGLoader::GetG(int u, int v) const
{
	return mData[(u + v * mWidth) * mNumBits + 1];
}

unsigned char PNGLoader::GetB(int u, int v) const
{
	return mData[(u + v * mWidth) * mNumBits + 2];
}

unsigned char PNGLoader::GetA(int u, int v) const
{
	if (mHasAlpha)
	{
		return mData[(u + v * mWidth) * 4 + 3];
	}
	else 
	{
		return 255.0f;
	}
}

bool PNGLoader::WritePNGFile(const char* file_name)
{
	/* create file */
	FILE *fp = fopen(file_name, "wb");
	if (!fp){
		return false;
	}

	/* initialize stuff */
	mPngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!mPngPtr){
		return false;
	}

	mInfoPtr = png_create_info_struct(mPngPtr);
	if (!mInfoPtr){
		return false;
	}

	if (setjmp(png_jmpbuf(mPngPtr))){
		return false;
	}

	png_init_io(mPngPtr, fp);


	/* write header */
	if (setjmp(png_jmpbuf(mPngPtr))){
		return false;
	}

	png_set_IHDR(mPngPtr, mInfoPtr, mWidth, mHeight,
		mBitDepth, mColorType, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(mPngPtr, mInfoPtr);


	/* write bytes */
	if (setjmp(png_jmpbuf(mPngPtr))){
		return false;
	}

	//png_write_image(mPngPtr, mRowPointers);
	//png_write_row(mPngPtr, mData);
	png_bytepp rows = (png_bytepp)png_malloc(mPngPtr, mHeight * sizeof(png_bytep));
	for (int i = 0; i < mHeight; ++i)
		rows[i] = (png_bytep)(mData + (mHeight - i - 1) * mWidth * mInfoPtr->channels);

	png_write_image(mPngPtr, rows);


	/* end write */
	if (setjmp(png_jmpbuf(mPngPtr))){
		return false;
	}

	png_write_end(mPngPtr, NULL);

	/* cleanup heap allocation */
	/*for (int y = 0; y < mHeight; y++)
		free(mRowPointers[y]);
	free(mRowPointers);*/

	fclose(fp);

	return false;
}