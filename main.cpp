#include<windows.h>
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
string s;
void ScreenCapture(LPSTR filename,WORD BitCount,LPRECT lpRect){
	HBITMAP hBitmap;
	HDC hScreenDC=CreateDC("DISPLAY",NULL,NULL,NULL),hmemDC=CreateCompatibleDC(hScreenDC);
	int ScreenWidth=GetDeviceCaps(hScreenDC,HORZRES),ScreenHeight=GetDeviceCaps(hScreenDC,VERTRES);
	HBITMAP hOldBM;
	PVOID lpvpxldata;
	INT ixStart,iyStart,iX,iY;
	DWORD dwBitmapArraySize,nBitsOffset,lImageSize,lFileSize,dwWritten;
	BITMAPINFO bmInfo;
	BITMAPFILEHEADER bmFileHeader;
	HANDLE hbmfile;
	if(!lpRect){
		ixStart=iyStart=0;
		iX=ScreenWidth;
		iY=ScreenHeight;
	}else{
		ixStart=lpRect->left;
		iyStart=lpRect->top;
		iX=lpRect->right-lpRect->left;
		iY=lpRect->bottom-lpRect->top;
	}
	hBitmap=CreateCompatibleBitmap(hScreenDC,iX,iY);
	hOldBM=(HBITMAP)SelectObject(hmemDC,hBitmap);
	BitBlt(hmemDC,0,0,iX,iY,hScreenDC,ixStart,iyStart,SRCCOPY);
	hBitmap=(HBITMAP)SelectObject(hmemDC,hOldBM);
	if(!filename){
		DeleteDC(hScreenDC);
		DeleteDC(hmemDC);
		return;
	}
	dwBitmapArraySize=((((iX*32)+31)&~31)>>3)*iY;
	lpvpxldata=HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,dwBitmapArraySize);
	ZeroMemory(lpvpxldata,dwBitmapArraySize);
	ZeroMemory(&bmInfo,sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth=iX;
	bmInfo.bmiHeader.biHeight=iY;
	bmInfo.bmiHeader.biPlanes=1;
	bmInfo.bmiHeader.biBitCount=BitCount;
	bmInfo.bmiHeader.biCompression=BI_RGB;
	ZeroMemory(&bmFileHeader,sizeof(BITMAPFILEHEADER));
	nBitsOffset=sizeof(BITMAPFILEHEADER)+bmInfo.bmiHeader.biSize;
	lImageSize=((((bmInfo.bmiHeader.biWidth*bmInfo.bmiHeader.biBitCount)+31)&~31)>>3)*bmInfo.bmiHeader.biHeight;
	lFileSize=nBitsOffset+lImageSize;
	bmFileHeader.bfType='B'+('M'<<8);
	bmFileHeader.bfSize=lFileSize;
	bmFileHeader.bfOffBits=nBitsOffset;
	GetDIBits(hmemDC,hBitmap,0,bmInfo.bmiHeader.biHeight,lpvpxldata,&bmInfo,DIB_RGB_COLORS);
	hbmfile=CreateFileA(filename,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteFile(hbmfile,&bmFileHeader,sizeof(BITMAPFILEHEADER),&dwWritten,NULL);
	WriteFile(hbmfile,&bmInfo,sizeof(BITMAPINFO),&dwWritten,NULL);
	WriteFile(hbmfile,lpvpxldata,lImageSize,&dwWritten,NULL);
	CloseHandle(hbmfile);
	HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,lpvpxldata);
	ReleaseDC(0,hScreenDC);
	DeleteDC(hmemDC);
	DeleteObject(hBitmap);
}
int main(){
	while(true){
		if(GetAsyncKeyState(VK_MBUTTON))return 0;
		ScreenCapture("screen.bmp",32,NULL);
		Sleep(500);
	}
}
