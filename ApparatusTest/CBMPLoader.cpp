#include"CBMPLoader.h"           

CBMPLoader::CBMPLoader()
{
	image = 0;
	imageWidth = 0;
	imageHeight = 0;
}


CBMPLoader::~CBMPLoader()
{
   FreeImage(); 
}

// 装载一个位图文件 
bool CBMPLoader::LoadBitmap(const char *file)
{
	FILE *pFile = 0; 
	
	// 创建位图文件信息和位图文件头结构 
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
  
	// 用于将图像颜色从BGR变换到RGB 
	unsigned char textureColors = 0;

    // 打开文件,并检查错误 
	pFile = fopen(file, "rb");
		if(pFile == 0) return false;

	// 读入位图文件头信息  
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	
	// 检查该文件是否为位图文件 
	if(header.bfType != BITMAP_ID)
	   {
		   fclose(pFile);            
		   return false;
	   }

	//  读入位图文件信息 
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	//  保存图像的宽度和高度 
	imageWidth = bitmapInfoHeader.biWidth;
    imageHeight = bitmapInfoHeader.biHeight;

    //  确保读取数据的大小 
   if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
      bitmapInfoHeader.biHeight * 3;

	//  将指针移到数据开始位置 
	fseek(pFile, header.bfOffBits, SEEK_SET);

	//  分配内存
	image = new unsigned char[bitmapInfoHeader.biSizeImage];

	//  检查内存分配是否成功 
	if(!image)                        
	   {
		   delete[] image;
		   fclose(pFile);
		   return false;
	   }

	// 读取图像数据 
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	// 将图像颜色数据格式进行交换,由BGR转换为RGB 
	for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3)
	   {
		   textureColors = image[index];
		   image[index] = image[index + 2];
		   image[index + 2] = textureColors;
	   }
  
	fclose(pFile);       
	return true;        
}

void CBMPLoader::FreeImage()
{
   // 释放分配的内存 
   if(image)
      {
         delete[] image;
         image = 0;
      }
}
