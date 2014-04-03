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

// װ��һ��λͼ�ļ� 
bool CBMPLoader::LoadBitmap(const char *file)
{
	FILE *pFile = 0; 
	
	// ����λͼ�ļ���Ϣ��λͼ�ļ�ͷ�ṹ 
	BITMAPINFOHEADER bitmapInfoHeader;
	BITMAPFILEHEADER header;
  
	// ���ڽ�ͼ����ɫ��BGR�任��RGB 
	unsigned char textureColors = 0;

    // ���ļ�,�������� 
	pFile = fopen(file, "rb");
		if(pFile == 0) return false;

	// ����λͼ�ļ�ͷ��Ϣ  
	fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
	
	// �����ļ��Ƿ�Ϊλͼ�ļ� 
	if(header.bfType != BITMAP_ID)
	   {
		   fclose(pFile);            
		   return false;
	   }

	//  ����λͼ�ļ���Ϣ 
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);

	//  ����ͼ��Ŀ�Ⱥ͸߶� 
	imageWidth = bitmapInfoHeader.biWidth;
    imageHeight = bitmapInfoHeader.biHeight;

    //  ȷ����ȡ���ݵĴ�С 
   if(bitmapInfoHeader.biSizeImage == 0)
      bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *
      bitmapInfoHeader.biHeight * 3;

	//  ��ָ���Ƶ����ݿ�ʼλ�� 
	fseek(pFile, header.bfOffBits, SEEK_SET);

	//  �����ڴ�
	image = new unsigned char[bitmapInfoHeader.biSizeImage];

	//  ����ڴ�����Ƿ�ɹ� 
	if(!image)                        
	   {
		   delete[] image;
		   fclose(pFile);
		   return false;
	   }

	// ��ȡͼ������ 
	fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);

	// ��ͼ����ɫ���ݸ�ʽ���н���,��BGRת��ΪRGB 
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
   // �ͷŷ�����ڴ� 
   if(image)
      {
         delete[] image;
         image = 0;
      }
}
