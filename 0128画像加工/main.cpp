#define _CRT_SECURE_NO_WARNINGS
#define _CRT_STDIO_ISO_WIDE_SPECIFIERS
#define _USE_MATH_DEFINES

#define moSize 4
#define rszW 2
#define rszH 2

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <limits.h>
#include <tchar.h>
#include <math.h>

int main()
{
	//unsigned char img[512][512][3];		//�ǂݍ��񂾉摜��ۑ�����(�c��512�܂�)
	//char* img = (char*)malloc(512 * 512 * 3 * sizeof(char));
	unsigned short bfType;
	int bfSize;							//BMP�̃t�@�C���T�C�Y
	unsigned char BitmapFileHeader[8];	//BMP�̃t�@�C���w�b�_�[��ۑ�����
	int biSize;							//BMP�̃T�C�Y
	int biWidth;						//BMP�̕�
	int biHeight;						//BMP�̍���
	unsigned short biPlanes;
	unsigned short biBitcount;			//BMP�̐F�r�b�g��
	int biCompression;
	int biSizeImage;					//BMP�̉摜�f�[�^�T�C�Y
	unsigned char BitmapInfoHeader[16]; //��L��3�ȊO��BMP�̏��w�b�_�[��ۑ�����
	int w, h, c;						//for���p
	FILE* fp;
	char fName[100];
	char inAddress[100];
	char outAddress[100];
	char code[20];
	unsigned char color;
	unsigned char tmp[3];
	int x, y, mozaR, mozaG, mozaB;
	int sx, sy, ex, ey;
	int trmWidth, trmHeight,trmSizeImage,trmbfSize;
	int rszWidth, rszHeight,rszSizeImage,rszbfSize;
	double oriW, oriH;
	int w0, h0;
	int kaku;
	double rad;
	int ktnWidth, ktnHeight;

	printf("���H�̎�ނ�I��ł��������B\n");
	printf("�R�s�[��copy\n");
	printf("�O���[�X�P�[����gray\n");
	printf("�ԉ��H��red\n");
	printf("�Ή��H��green\n");
	printf("���H��blue\n");
	printf("�V�A�����H��cyan\n");
	printf("�����H��yellow\n");
	printf("�}�[���^���H��magenta\n");
	printf("���E���]��sayuu\n");
	printf("�㉺���]��jouge\n");
	printf("�㉺���E���]��jougesayuu\n");
	printf("���U�C�N��mozaic\n");
	printf("�g���~���O��trimming\n");
	printf("���x���]��meidohanten\n");
	printf("�k����resize(������)\n");
	printf("��]��kaiten\n");

	scanf("%s", &code);

	WIN32_FIND_DATA findData;
	HANDLE hFind;

	hFind = FindFirstFile(_T("C:\\Users\\user\\Documents\\�v���O���~���O�Z�@�P\\0128�摜���H\\in.bmp\\*.bmp"), &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {

			//��wchar_t�^����char�^�ւ̕ϊ��p�ϐ�
			char henkan[sizeof(findData.cFileName) / sizeof(findData.cFileName[0]) * MB_LEN_MAX];
			
			//��wchar_t�^����char�^�ւ̕ϊ�
			int len = wcstombs(henkan, findData.cFileName, sizeof(henkan));
			strcpy(inAddress, "C:\\Users\\user\\Documents\\�v���O���~���O�Z�@�P\\0128�摜���H\\in.bmp\\");
			if (len < 0)
			{
				wprintf(L"�t�@�C�����̃��C�h�����񂩂�}���`�o�C�g������ւ̕ϊ����s\n");
			}
			else
			{
				strcat(inAddress, henkan);
				strcpy(fName,inAddress);
			}

			fp = fopen(fName, "rb");						//"in.bmp"����ǂݍ���

			fread(&bfType, sizeof(short), 1, fp);
			fread(&bfSize, sizeof(int), 1, fp);
			fread(BitmapFileHeader, sizeof(char), 8, fp);	//�t�@�C���w�b�_�[��ǂݍ���
			fread(&biSize, sizeof(int), 1, fp);				//���w�b�_�[�̃T�C�Y��ǂݍ���
			fread(&biWidth, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̕���ǂݍ���
			fread(&biHeight, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̍�����ǂݍ���
			fread(&biPlanes, sizeof(short), 1, fp);
			fread(&biBitcount, sizeof(short), 1, fp);			//�摜�̐F�r�b�g��
			fread(&biCompression, sizeof(int), 1, fp);
			fread(&biSizeImage, sizeof(int), 1, fp);
			fread(BitmapInfoHeader, sizeof(char), 16, fp);	//���w�b�_�[�̎c���ǂݍ���
			unsigned char* img = (unsigned char*)malloc(sizeof(unsigned char) * (biHeight) * biWidth * 3);

			if (biSize != 40)
			{
				printf("%s�͑Ή����Ă��܂���B", henkan);
				exit(0);
			}

			if (biBitcount == 1 || biBitcount == 4 || biBitcount == 8)
			{
				printf("%s�͑Ή����Ă��܂���B", henkan);
				exit(0);
			}

			for (h = 0; h < biHeight; h++)		//�������[�v
			{
				for (w = 0; w < biWidth; w++)	//�����[�v
				{
					for (c = 0; c < 3; c++)		//�F���[�v
					{
						fread(&img[(h * biWidth + w)* 3 + c], sizeof(char), 1, fp); //��f��ǂݍ���
					}
				}
			}
			fclose(fp);

			unsigned char* newimg = (unsigned char*)malloc(sizeof(unsigned char) * (biHeight)*biWidth * 3);

			/*���摜���H��*/
			/*���R�s�[��*/
			if (strcmp(code, "copy") == 0)
			{

				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						for (c = 0; c < 3; c++)
						{
							newimg[(h * biWidth + w) * 3 + c] = img[(h * biWidth + w) * 3 + c];
						}
					}
				}
			}
			/*���O���[�X�P�[����*/
			else if (strcmp(code, "gray") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						//gray = (img[w][h][0] + img[w][h][1] + img[w][h][2])/3;
						color = ((int)img[(h * biWidth + w) * 3 + 0] + (int)img[(h * biWidth + w) * 3 + 1] + (int)img[(h * biWidth + w) * 3 + 2]) / 3;
						for (c = 0; c < 3; c++)
						{
							newimg[(h * biWidth + w) * 3 + c] = color;
						}
					}
				}
			}
			/*���ԉ��H��*/
			else if (strcmp(code, "red") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						color = ((int)img[(h * biWidth + w) * 3 + 0] + (int)img[(h * biWidth + w) * 3 + 1] + (int)img[(h * biWidth + w) * 3 + 2]) / 3;
						newimg[(h * biWidth + w) * 3 + 0] = 0;
						newimg[(h * biWidth + w) * 3 + 1] = 0;
						newimg[(h * biWidth + w) * 3 + 2] = color;
					}
				}
			}
			/*���Ή��H��*/
			else if (strcmp(code, "green") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						color = ((int)img[(h * biWidth + w) * 3 + 0] + (int)img[(h * biWidth + w) * 3 + 1] + (int)img[(h * biWidth + w) * 3 + 2]) / 3;
						newimg[(h * biWidth + w) * 3 + 0] = 0;
						newimg[(h * biWidth + w) * 3 + 1] = color;
						newimg[(h * biWidth + w) * 3 + 2] = 0;
					}
				}
			}
			/*�����H��*/
			else if (strcmp(code, "blue") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						color = ((int)img[(h * biWidth + w) * 3 + 0] + (int)img[(h * biWidth + w) * 3 + 1] + (int)img[(h * biWidth + w) * 3 + 2]) / 3;
						newimg[(h * biWidth + w) * 3 + 0] = color;
						newimg[(h * biWidth + w) * 3 + 1] = 0;
						newimg[(h * biWidth + w) * 3 + 2] = 0;
					}
				}
			}
			/*���V�A�����H��*/
			else if (strcmp(code, "cyan") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						color = ((int)img[(h * biWidth + w) * 3 + 0] + (int)img[(h * biWidth + w) * 3 + 1] + (int)img[(h * biWidth + w) * 3 + 2]) / 3;
						newimg[(h * biWidth + w) * 3 + 0] = color;
						newimg[(h * biWidth + w) * 3 + 1] = color;
						newimg[(h * biWidth + w) * 3 + 2] = 0;
					}
				}
			}
			/*�������H��*/
			else if (strcmp(code, "yellow") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						color = ((int)img[(h * biWidth + w) * 3 + 0] + (int)img[(h * biWidth + w) * 3 + 1] + (int)img[(h * biWidth + w) * 3 + 2]) / 3;
						newimg[(h * biWidth + w) * 3 + 0] = 0;
						newimg[(h * biWidth + w) * 3 + 1] = color;
						newimg[(h * biWidth + w) * 3 + 2] = color;
					}
				}
			}
			/*���}�[���^���H��*/
			else if (strcmp(code, "magenta") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						color = ((int)img[(h * biWidth + w) * 3 + 0] + (int)img[(h * biWidth + w) * 3 + 1] + (int)img[(h * biWidth + w) * 3 + 2]) / 3;
						newimg[(h * biWidth + w) * 3 + 0] = color;
						newimg[(h * biWidth + w) * 3 + 1] = 0;
						newimg[(h * biWidth + w) * 3 + 2] = color;
					}
				}
			}

			/*�����E���]��*/
			else if (strcmp(code, "sayuu") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth / 2; w++)
					{
						for (c = 0; c < 3; c++)
						{
							tmp[c] = img[(h * biWidth + w) * 3 + c];
							newimg[(h * biWidth + w) * 3 + c] = img[((h + 1) * biWidth - w - 1)* 3 + c];
							newimg[((h + 1) * biWidth - w - 1) * 3 + c] = tmp[c];
						}
					}
				}
			}

			/*���㉺���]��*/
			else if (strcmp(code, "jouge") == 0)
			{
				for (h = 0; h < biHeight / 2; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						for (c = 0; c < 3; c++)
						{
							tmp[c] = img[(h * biWidth + w) * 3 + c];
							newimg[(h * biWidth + w) * 3 + c] = img[((biHeight - h - 1)* biWidth + w) * 3 + c];
							newimg[((biHeight - h - 1) * biWidth + w) * 3 + c] = tmp[c];
						}
					}
				}
			}

			/*���㉺���E���]��*/
			else if (strcmp(code, "jougesayuu") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth / 2; w++)
					{
						for (c = 0; c < 3; c++)
						{
							tmp[c] = img[(h * biWidth + w) * 3 + c];
							newimg[(h * biWidth + w) * 3 + c] = img[((h + 1) * biWidth - w - 1) * 3 + c];
							newimg[((h + 1) * biWidth - w - 1) * 3 + c] = tmp[c];
						}
					}
				}
				for (h = 0; h < biHeight / 2; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						for (c = 0; c < 3; c++)
						{
							tmp[c] = newimg[(h * biWidth + w) * 3 + c];
							newimg[(h * biWidth + w) * 3 + c] = newimg[((biHeight - h - 1) * biWidth + w) * 3 + c];
							newimg[((biHeight - h - 1) * biWidth + w) * 3 + c] = tmp[c];
						}
					}
				}
			}
			/*�����U�C�N��*/
			else if (strcmp(code, "mozaic") == 0)
			{
				for (h = 0; h < biHeight; h += moSize)
				{
					for (w = 0; w < biWidth; w += moSize)
					{
						mozaB = 0;
						mozaG = 0;
						mozaR = 0;
						for (y = 0; y < moSize; y++)
						{
							for (x = 0; x < moSize; x++)
							{
								mozaB += img[((h + y) * biWidth + (w + x)) * 3 + 0];
								mozaG += img[((h + y) * biWidth + (w + x)) * 3 + 1];
								mozaR += img[((h + y) * biWidth + (w + x)) * 3 + 2];
							}
						}
						mozaB /= moSize * moSize;
						mozaG /= moSize * moSize;
						mozaR /= moSize * moSize;
						for (y = 0; y < moSize; y++)
						{
							for (x = 0; x < moSize; x++)
							{
								newimg[((h + y) * biWidth + (w + x)) * 3 + 0] = mozaB;
								newimg[((h + y) * biWidth + (w + x)) * 3 + 1] = mozaG;
								newimg[((h + y) * biWidth + (w + x)) * 3 + 2] = mozaR;
							}
						}
					}
				}
			}

			/*���g���~���O��*/
			else if (strcmp(code, "trimming") == 0)
			{
				printf("�J�n�_��x���W��?\n");
				scanf("%d", &sx);
				if (sx < 0 || sx > biWidth)
				{
					printf("�J�n�_��x���W���Ԉ���Ă��܂��B\n");
					exit(0);
				}
				printf("�J�n�_��y���W��?\n");
				scanf("%d", &sy);
				if (sy < 0 || sy > biHeight)
				{
					printf("�J�n�_��y���W���Ԉ���Ă��܂��B\n");
					exit(0);
				}
				printf("�I���_��x���W��?\n");
				scanf("%d", &ex);
				if (sx < 0 || ex > biWidth)
				{
					printf("�I���_��x���W���Ԉ���Ă��܂��B\n");
					exit(0);
				}
				printf("�I���_��y���W��?\n");
				scanf("%d", &ey);
				if (sy < 0 || ey > biHeight)
				{
					printf("�I���_��y���W���Ԉ���Ă��܂��B\n");
					exit(0);
				}
				if (sx > ex)
				{
					printf("x���W�̊J�n�_�ƏI���_�̑召�֌W���t�ł��B\n");
					exit(0);
				}
				if (sy > ey)
				{
					printf("y���W�̊J�n�_�ƏI���_�̑召�֌W���t�ł��B\n");
					exit(0);
				}
				trmWidth = ex - sx ;
				trmHeight = ey - sy ;
				trmSizeImage = trmWidth * trmHeight * 3;
				trmbfSize = trmSizeImage + 54;

				unsigned char* trmimg = (unsigned char*)malloc(sizeof(unsigned char) * trmWidth * trmHeight * 3);

				for (h = 0; h < trmHeight; h++)
				{
					for (w = 0; w < trmWidth; w++)
					{
						for (c = 0; c < 3; c++)
						{
							trmimg[(h * trmWidth + w) * 3 + c] = img[((h + sy) * biWidth + w + sx) * 3 + c];
						}
					}
				}

				strcpy(outAddress, "C:\\Users\\user\\Documents\\�v���O���~���O�Z�@�P\\0128�摜���H\\out.bmp\\new");
				strcat(outAddress, henkan);
				strcpy(fName, outAddress);

				fp = fopen(fName, "wb");						//"out.bmp"�ɏ����o��

				fwrite(&bfType, sizeof(short), 1, fp);
				fwrite(&trmbfSize, sizeof(int), 1, fp);
				fwrite(BitmapFileHeader, sizeof(char), 8, fp);	//�t�@�C���w�b�_�[��ǂݍ���
				fwrite(&biSize, sizeof(int), 1, fp);			//���w�b�_�[�̃T�C�Y����������
				fwrite(&trmWidth, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̕�����������
				fwrite(&trmHeight, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̍�������������
				fwrite(&biPlanes, sizeof(short), 1, fp);
				fwrite(&biBitcount, sizeof(short), 1, fp);		//�摜�̐F�r�b�g��
				fwrite(&biCompression, sizeof(int), 1, fp);
				fwrite(&trmSizeImage, sizeof(int), 1, fp);
				fwrite(BitmapInfoHeader, sizeof(char), 16, fp);	//���w�b�_�[�̎c���ǂݍ���

				for (h = 0; h < trmHeight; h++)		//�������[�v
				{
					for (w = 0; w < trmWidth; w++)	//�����[�v
					{
						for (c = 0; c < 3; c++)		//�F���[�v
						{
							fwrite(&trmimg[(h * trmWidth + w) * 3 + c], sizeof(unsigned char), 1, fp); //��f����������
						}
					}
				}
			}
			/*�����x���]��*/
			else if (strcmp(code, "meidohanten") == 0)
			{
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						for (c = 0; c < 3; c++)
						{
							newimg[(h * biWidth + w) * 3 + c] = 255 - img[(h * biWidth + w) * 3 + c];
						}
					}
				}
			}
			/*���g��k����*/
			else if (strcmp(code, "resize") == 0)
			{
				rszWidth = biWidth * rszW;
				rszHeight = biHeight * rszH;

				unsigned char* rszimg = (unsigned char*)malloc(sizeof(unsigned char) * rszWidth * rszHeight * 3);
				
				for (h = 0; h < rszHeight; h++)
				{
					for (w = 0; w < rszWidth; w++)
					{
						for (c = 0; c < 3; c++)
						{
							oriW = (double)w * rszW;
							w0 = (int)(oriW + 0.5);
							if (w0 == biWidth)
							{
								w0 = biWidth - 1;
							}

							oriH = (double)h * rszH;
							h0 = (int)(oriH + 0.5);
							if (h0 == biHeight)
							{
								h0 = biHeight - 1;
							}
							rszimg[(h * rszWidth + w) * 3 + c] = img[(h0 * biWidth + w0) * 3 + c];
						}
					}
				}

				rszSizeImage = rszWidth * rszHeight * 3;
				rszbfSize = rszSizeImage + 54;

				strcpy(outAddress, "C:\\Users\\user\\Documents\\�v���O���~���O�Z�@�P\\0128�摜���H\\out.bmp\\new");
				strcat(outAddress, henkan);
				strcpy(fName, outAddress);

				fp = fopen(fName, "wb");						//"out.bmp"�ɏ����o��

				fwrite(&bfType, sizeof(short), 1, fp);
				fwrite(&rszbfSize, sizeof(int), 1, fp);
				fwrite(BitmapFileHeader, sizeof(char), 8, fp);	//�t�@�C���w�b�_�[��ǂݍ���
				fwrite(&biSize, sizeof(int), 1, fp);			//���w�b�_�[�̃T�C�Y����������
				fwrite(&rszWidth, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̕�����������
				fwrite(&rszHeight, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̍�������������
				fwrite(&biPlanes, sizeof(short), 1, fp);
				fwrite(&biBitcount, sizeof(short), 1, fp);		//�摜�̐F�r�b�g��
				fwrite(&biCompression, sizeof(int), 1, fp);
				fwrite(&rszSizeImage, sizeof(int), 1, fp);
				fwrite(BitmapInfoHeader, sizeof(char), 16, fp);	//���w�b�_�[�̎c���ǂݍ���

				for (h = 0; h < rszHeight; h++)		//�������[�v
				{
					for (w = 0; w < rszWidth; w++)	//�����[�v
					{
						for (c = 0; c < 3; c++)		//�F���[�v
						{
							fwrite(&rszimg[(h * rszWidth + w) * 3 + c], sizeof(unsigned char), 1, fp); //��f����������
						}
					}
				}
			}
			/*����]��*/
			else if (strcmp(code, "kaiten") == 0)
			{
				printf("��]�p�x��0�`359�œ��͂��ĉ������B\n");
				scanf("%d", &kaku);

				rad = (double)kaku * M_PI / (double)180;

				memset(newimg, 0xFF, biWidth* biHeight * 3);
				for (h = 0; h < biHeight; h++)
				{
					for (w = 0; w < biWidth; w++)
					{
						oriW = (w - (int)biWidth / 2) * cos(rad) +(h - (int)biHeight / 2) * sin(rad) + biWidth / 2;
						oriH = -(w - (int)biWidth / 2) * sin(rad) +(h - (int)biHeight / 2) * cos(rad) + biHeight / 2;

						w0 = (int)(oriW + 0.5);
						h0 = (int)(oriH + 0.5);
						if (w0 < 0 || w0 >= biWidth)
						{
							continue;
						}
						if (h0 < 0 || h0 >= biHeight)
						{
							continue;
						}

						for (c = 0; c < 3; c++)
						{
							newimg[(h * biWidth + w) * 3 + c] = img[(h0 * biWidth + w0) * 3 + c];
						}
					}
				}
			}
			else
			{
				printf("�F���ł��Ȃ��R�[�h�ł��B\n");
				exit(0);
			}
			/*���摜���H��*/
			
			if (strcmp(code, "trimming") != 0 && strcmp(code, "resize") != 0)
			{
				strcpy(outAddress, "C:\\Users\\user\\Documents\\�v���O���~���O�Z�@�P\\0128�摜���H\\out.bmp\\new");
				strcat(outAddress, henkan);
				strcpy(fName, outAddress);

				fp = fopen(fName, "wb");						//"out.bmp"�ɏ����o��

				fwrite(&bfType, sizeof(short), 1, fp);
				fwrite(&bfSize, sizeof(int), 1, fp);
				fwrite(BitmapFileHeader, sizeof(char), 8, fp);	//�t�@�C���w�b�_�[��ǂݍ���
				fwrite(&biSize, sizeof(int), 1, fp);			//���w�b�_�[�̃T�C�Y����������
				fwrite(&biWidth, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̕�����������
				fwrite(&biHeight, sizeof(int), 1, fp);			//���w�b�_�[�ɂ���摜�̍�������������
				fwrite(&biPlanes, sizeof(short), 1, fp);
				fwrite(&biBitcount, sizeof(short), 1, fp);		//�摜�̐F�r�b�g��
				fwrite(&biCompression, sizeof(int), 1, fp);
				fwrite(&biSizeImage, sizeof(int), 1, fp);
				fwrite(BitmapInfoHeader, sizeof(char), 16, fp);	//���w�b�_�[�̎c���ǂݍ���

			    for (h = 0; h < biHeight; h++)		//�������[�v
				{
					for (w = 0; w < biWidth; w++)	//�����[�v
					{
						for (c = 0; c < 3; c++)		//�F���[�v
						{
							fwrite(&newimg[(h * biWidth + w) * 3 + c], sizeof(unsigned char), 1, fp); //��f����������
						}
					}
				}
				//fwrite(newimg, sizeof(unsigned char), biWidth* biHeight * 3, fp);
			}
			fclose(fp);
			free(img);
			free(newimg);

		} while (FindNextFile(hFind, &findData));
		FindClose(hFind);
	}
	return 0;
}