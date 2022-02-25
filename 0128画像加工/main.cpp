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
	//unsigned char img[512][512][3];		//読み込んだ画像を保存する(縦横512まで)
	//char* img = (char*)malloc(512 * 512 * 3 * sizeof(char));
	unsigned short bfType;
	int bfSize;							//BMPのファイルサイズ
	unsigned char BitmapFileHeader[8];	//BMPのファイルヘッダーを保存する
	int biSize;							//BMPのサイズ
	int biWidth;						//BMPの幅
	int biHeight;						//BMPの高さ
	unsigned short biPlanes;
	unsigned short biBitcount;			//BMPの色ビット数
	int biCompression;
	int biSizeImage;					//BMPの画像データサイズ
	unsigned char BitmapInfoHeader[16]; //上記の3つ以外のBMPの情報ヘッダーを保存する
	int w, h, c;						//for文用
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

	printf("加工の種類を選んでください。\n");
	printf("コピー→copy\n");
	printf("グレースケール→gray\n");
	printf("赤加工→red\n");
	printf("緑加工→green\n");
	printf("青加工→blue\n");
	printf("シアン加工→cyan\n");
	printf("黄加工→yellow\n");
	printf("マゼンタ加工→magenta\n");
	printf("左右反転→sayuu\n");
	printf("上下反転→jouge\n");
	printf("上下左右反転→jougesayuu\n");
	printf("モザイク→mozaic\n");
	printf("トリミング→trimming\n");
	printf("明度反転→meidohanten\n");
	printf("縮小→resize(未完成)\n");
	printf("回転→kaiten\n");

	scanf("%s", &code);

	WIN32_FIND_DATA findData;
	HANDLE hFind;

	hFind = FindFirstFile(_T("C:\\Users\\user\\Documents\\プログラミング技法１\\0128画像加工\\in.bmp\\*.bmp"), &findData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {

			//↓wchar_t型からchar型への変換用変数
			char henkan[sizeof(findData.cFileName) / sizeof(findData.cFileName[0]) * MB_LEN_MAX];
			
			//↓wchar_t型からchar型への変換
			int len = wcstombs(henkan, findData.cFileName, sizeof(henkan));
			strcpy(inAddress, "C:\\Users\\user\\Documents\\プログラミング技法１\\0128画像加工\\in.bmp\\");
			if (len < 0)
			{
				wprintf(L"ファイル名のワイド文字列からマルチバイト文字列への変換失敗\n");
			}
			else
			{
				strcat(inAddress, henkan);
				strcpy(fName,inAddress);
			}

			fp = fopen(fName, "rb");						//"in.bmp"から読み込む

			fread(&bfType, sizeof(short), 1, fp);
			fread(&bfSize, sizeof(int), 1, fp);
			fread(BitmapFileHeader, sizeof(char), 8, fp);	//ファイルヘッダーを読み込む
			fread(&biSize, sizeof(int), 1, fp);				//情報ヘッダーのサイズを読み込む
			fread(&biWidth, sizeof(int), 1, fp);			//情報ヘッダーにある画像の幅を読み込む
			fread(&biHeight, sizeof(int), 1, fp);			//情報ヘッダーにある画像の高さを読み込む
			fread(&biPlanes, sizeof(short), 1, fp);
			fread(&biBitcount, sizeof(short), 1, fp);			//画像の色ビット数
			fread(&biCompression, sizeof(int), 1, fp);
			fread(&biSizeImage, sizeof(int), 1, fp);
			fread(BitmapInfoHeader, sizeof(char), 16, fp);	//情報ヘッダーの残りを読み込む
			unsigned char* img = (unsigned char*)malloc(sizeof(unsigned char) * (biHeight) * biWidth * 3);

			if (biSize != 40)
			{
				printf("%sは対応していません。", henkan);
				exit(0);
			}

			if (biBitcount == 1 || biBitcount == 4 || biBitcount == 8)
			{
				printf("%sは対応していません。", henkan);
				exit(0);
			}

			for (h = 0; h < biHeight; h++)		//高さループ
			{
				for (w = 0; w < biWidth; w++)	//幅ループ
				{
					for (c = 0; c < 3; c++)		//色ループ
					{
						fread(&img[(h * biWidth + w)* 3 + c], sizeof(char), 1, fp); //画素を読み込む
					}
				}
			}
			fclose(fp);

			unsigned char* newimg = (unsigned char*)malloc(sizeof(unsigned char) * (biHeight)*biWidth * 3);

			/*↓画像加工↓*/
			/*↓コピー↓*/
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
			/*↓グレースケール↓*/
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
			/*↓赤加工↓*/
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
			/*↓緑加工↓*/
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
			/*↓青加工↓*/
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
			/*↓シアン加工↓*/
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
			/*↓黄加工↓*/
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
			/*↓マゼンタ加工↓*/
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

			/*↓左右反転↓*/
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

			/*↓上下反転↓*/
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

			/*↓上下左右反転↓*/
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
			/*↓モザイク↓*/
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

			/*↓トリミング↓*/
			else if (strcmp(code, "trimming") == 0)
			{
				printf("開始点のx座標は?\n");
				scanf("%d", &sx);
				if (sx < 0 || sx > biWidth)
				{
					printf("開始点のx座標が間違っています。\n");
					exit(0);
				}
				printf("開始点のy座標は?\n");
				scanf("%d", &sy);
				if (sy < 0 || sy > biHeight)
				{
					printf("開始点のy座標が間違っています。\n");
					exit(0);
				}
				printf("終了点のx座標は?\n");
				scanf("%d", &ex);
				if (sx < 0 || ex > biWidth)
				{
					printf("終了点のx座標が間違っています。\n");
					exit(0);
				}
				printf("終了点のy座標は?\n");
				scanf("%d", &ey);
				if (sy < 0 || ey > biHeight)
				{
					printf("終了点のy座標が間違っています。\n");
					exit(0);
				}
				if (sx > ex)
				{
					printf("x座標の開始点と終了点の大小関係が逆です。\n");
					exit(0);
				}
				if (sy > ey)
				{
					printf("y座標の開始点と終了点の大小関係が逆です。\n");
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

				strcpy(outAddress, "C:\\Users\\user\\Documents\\プログラミング技法１\\0128画像加工\\out.bmp\\new");
				strcat(outAddress, henkan);
				strcpy(fName, outAddress);

				fp = fopen(fName, "wb");						//"out.bmp"に書き出す

				fwrite(&bfType, sizeof(short), 1, fp);
				fwrite(&trmbfSize, sizeof(int), 1, fp);
				fwrite(BitmapFileHeader, sizeof(char), 8, fp);	//ファイルヘッダーを読み込む
				fwrite(&biSize, sizeof(int), 1, fp);			//情報ヘッダーのサイズを書き込む
				fwrite(&trmWidth, sizeof(int), 1, fp);			//情報ヘッダーにある画像の幅を書き込む
				fwrite(&trmHeight, sizeof(int), 1, fp);			//情報ヘッダーにある画像の高さを書き込む
				fwrite(&biPlanes, sizeof(short), 1, fp);
				fwrite(&biBitcount, sizeof(short), 1, fp);		//画像の色ビット数
				fwrite(&biCompression, sizeof(int), 1, fp);
				fwrite(&trmSizeImage, sizeof(int), 1, fp);
				fwrite(BitmapInfoHeader, sizeof(char), 16, fp);	//情報ヘッダーの残りを読み込む

				for (h = 0; h < trmHeight; h++)		//高さループ
				{
					for (w = 0; w < trmWidth; w++)	//幅ループ
					{
						for (c = 0; c < 3; c++)		//色ループ
						{
							fwrite(&trmimg[(h * trmWidth + w) * 3 + c], sizeof(unsigned char), 1, fp); //画素を書き込む
						}
					}
				}
			}
			/*↓明度反転↓*/
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
			/*↓拡大縮小↓*/
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

				strcpy(outAddress, "C:\\Users\\user\\Documents\\プログラミング技法１\\0128画像加工\\out.bmp\\new");
				strcat(outAddress, henkan);
				strcpy(fName, outAddress);

				fp = fopen(fName, "wb");						//"out.bmp"に書き出す

				fwrite(&bfType, sizeof(short), 1, fp);
				fwrite(&rszbfSize, sizeof(int), 1, fp);
				fwrite(BitmapFileHeader, sizeof(char), 8, fp);	//ファイルヘッダーを読み込む
				fwrite(&biSize, sizeof(int), 1, fp);			//情報ヘッダーのサイズを書き込む
				fwrite(&rszWidth, sizeof(int), 1, fp);			//情報ヘッダーにある画像の幅を書き込む
				fwrite(&rszHeight, sizeof(int), 1, fp);			//情報ヘッダーにある画像の高さを書き込む
				fwrite(&biPlanes, sizeof(short), 1, fp);
				fwrite(&biBitcount, sizeof(short), 1, fp);		//画像の色ビット数
				fwrite(&biCompression, sizeof(int), 1, fp);
				fwrite(&rszSizeImage, sizeof(int), 1, fp);
				fwrite(BitmapInfoHeader, sizeof(char), 16, fp);	//情報ヘッダーの残りを読み込む

				for (h = 0; h < rszHeight; h++)		//高さループ
				{
					for (w = 0; w < rszWidth; w++)	//幅ループ
					{
						for (c = 0; c < 3; c++)		//色ループ
						{
							fwrite(&rszimg[(h * rszWidth + w) * 3 + c], sizeof(unsigned char), 1, fp); //画素を書き込む
						}
					}
				}
			}
			/*↓回転↓*/
			else if (strcmp(code, "kaiten") == 0)
			{
				printf("回転角度を0～359で入力して下さい。\n");
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
				printf("認識できないコードです。\n");
				exit(0);
			}
			/*↑画像加工↑*/
			
			if (strcmp(code, "trimming") != 0 && strcmp(code, "resize") != 0)
			{
				strcpy(outAddress, "C:\\Users\\user\\Documents\\プログラミング技法１\\0128画像加工\\out.bmp\\new");
				strcat(outAddress, henkan);
				strcpy(fName, outAddress);

				fp = fopen(fName, "wb");						//"out.bmp"に書き出す

				fwrite(&bfType, sizeof(short), 1, fp);
				fwrite(&bfSize, sizeof(int), 1, fp);
				fwrite(BitmapFileHeader, sizeof(char), 8, fp);	//ファイルヘッダーを読み込む
				fwrite(&biSize, sizeof(int), 1, fp);			//情報ヘッダーのサイズを書き込む
				fwrite(&biWidth, sizeof(int), 1, fp);			//情報ヘッダーにある画像の幅を書き込む
				fwrite(&biHeight, sizeof(int), 1, fp);			//情報ヘッダーにある画像の高さを書き込む
				fwrite(&biPlanes, sizeof(short), 1, fp);
				fwrite(&biBitcount, sizeof(short), 1, fp);		//画像の色ビット数
				fwrite(&biCompression, sizeof(int), 1, fp);
				fwrite(&biSizeImage, sizeof(int), 1, fp);
				fwrite(BitmapInfoHeader, sizeof(char), 16, fp);	//情報ヘッダーの残りを読み込む

			    for (h = 0; h < biHeight; h++)		//高さループ
				{
					for (w = 0; w < biWidth; w++)	//幅ループ
					{
						for (c = 0; c < 3; c++)		//色ループ
						{
							fwrite(&newimg[(h * biWidth + w) * 3 + c], sizeof(unsigned char), 1, fp); //画素を書き込む
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