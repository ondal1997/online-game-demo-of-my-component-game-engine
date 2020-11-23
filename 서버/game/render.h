#pragma once

#include "needHuman.h"

#pragma comment(lib,"msimg32.lib")

#include <ole2.h>
#include <Gdiplus.h>
#pragma comment(lib, "gdiplus")
#include <map>

#include <fstream>

using namespace Gdiplus;

struct Sprite
{
	Image* image;
	int centerX;
	int centerY;
};

class Render : public System
{
public:
	ULONG_PTR gdiplusToken;
	std::map<const char*, Sprite> map;

	Render(CameraSystem* camera, HWND hWnd, int fieldWidth, int fieldHeight) : camera(camera), hWnd(hWnd), fieldWidth(fieldWidth), fieldHeight(fieldHeight)
	{
		addComponentType<Position>();
		addComponentType<Graphic>();

		GdiplusStartupInput gdiplusStartupInput;
		while (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Status::Ok);
	}

	void loadImage(const wchar_t* dir, const wchar_t* name, const char* key)
	{
		wchar_t buf[100] = {};
		wcscpy(buf, dir);
		wcscat(buf, name);

		Image* image = nullptr;
		while (image == nullptr)
		{
			image = Image::FromFile(buf);
		}

		wcscpy(buf, dir);
		wcscat(buf, L"pos.txt");
		std::ifstream fin(buf);
		while (!fin.is_open())
		{
			fin.open(buf);
		}
		int centerX;
		int centerY;
		fin >> centerX >> centerY;
		fin.close();

		map[key] = { image, centerX, centerY };
	}

	void drawImage(HDC hdc, int x, int y, const char* key, bool bFlip)
	{
		if (map.find(key) == map.end())return;
		auto target = map[key];
		Graphics g(hdc);
		if (bFlip)
		{
			target.image->RotateFlip(RotateNoneFlipX);
			g.DrawImage(target.image, Rect(x - (target.image->GetWidth() - target.centerX), y - target.centerY, target.image->GetWidth(), target.image->GetHeight()), 0, 0, target.image->GetWidth(), target.image->GetHeight(), UnitPixel);
			target.image->RotateFlip(RotateNoneFlipX);
		}
		else
		{
			g.DrawImage(target.image, Rect(x - target.centerX, y - target.centerY, target.image->GetWidth(), target.image->GetHeight()), 0, 0, target.image->GetWidth(), target.image->GetHeight(), UnitPixel);
		}

	}

	void drawImage(HDC hdc, int x, int y, const char* key, float value)
	{
		if (map.find(key) == map.end())return;
		auto target = map[key];
		Graphics g(hdc);
		g.DrawImage(target.image, Rect(x - target.centerX, y - target.centerY, target.image->GetWidth() * value, target.image->GetHeight()), 0, 0, target.image->GetWidth() * value, target.image->GetHeight(), UnitPixel);
	}

	~Render()
	{
		for (auto it = map.begin(); it != map.end(); it++)
			delete it->second.image;

		GdiplusShutdown(gdiplusToken);
	}

	HWND hWnd;
	CameraSystem* camera;
	int fieldWidth;
	int fieldHeight;

	void update()
	{
		InvalidateRect(hWnd, NULL, FALSE);

		RECT rect_client;
		GetClientRect(hWnd, &rect_client);

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// 토탈 비트맵
		HDC totalDC = CreateCompatibleDC(hdc);
		HBITMAP totalBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
		HBITMAP hTotalOldBitmap = (HBITMAP)SelectObject(totalDC, totalBitmap);
		RECT total{ 0,0,1280,720 };
		FillRect(totalDC, &total, GetSysColorBrush(COLOR_WINDOW + 2));

		// 필드 비트맵
		HDC fieldDC = CreateCompatibleDC(hdc);
		HBITMAP fieldBitmap = CreateCompatibleBitmap(hdc, fieldWidth, fieldHeight);
		HBITMAP hFieldOleBitmap = (HBITMAP)SelectObject(fieldDC, fieldBitmap);
		RECT a{ 0,0,5000,2000 };
		RECT b{ 0,0,5000,1000 };
		FillRect(fieldDC, &a, GetSysColorBrush(COLOR_WINDOW + 2));
		FillRect(fieldDC, &b, GetSysColorBrush(COLOR_WINDOW + 1));

		// 인터페이스 비트맵
		HDC interfaceDC = CreateCompatibleDC(hdc);
		HBITMAP interfaceBitmap = CreateCompatibleBitmap(hdc, 1280, 720);
		HBITMAP hInterfaceOleBitmap = (HBITMAP)SelectObject(interfaceDC, interfaceBitmap);

		for (auto entity : entities)
		{
			auto position = entity->getComponent<Position>();
			auto graphic = entity->getComponent<Graphic>();

			if (entity->hasComponent<HpBar>())
			{
				// 직접 그리기
				auto hpBar = entity->getComponent<HpBar>();
				drawImage(interfaceDC, position->x, position->y, "hpBack", false);
				drawImage(interfaceDC, position->x, position->y, "hpEffect", hpBar->effect);
				drawImage(interfaceDC, position->x, position->y, "hpValue", hpBar->value);
			}
			else if (entity->hasComponent<MpBar>())
			{
				// 직접 그리기
				auto mpBar = entity->getComponent<MpBar>();
				for (int i = 0; i < 5; i++)
				{
					drawImage(interfaceDC, position->x + i * 64, position->y, "mpBack", false);
				}
				for (float i = 0.0f; i < 5.0f; i += 1.0f)
				{
					if (mpBar->effect < i + 1.0f)
					{
						drawImage(interfaceDC, position->x + i * 64, position->y, "mpValue", 1.0f - (i + 1.0f - mpBar->effect));
						break;
					}
					drawImage(interfaceDC, position->x + i * 64, position->y, "mpEffect", false);
				}
				if (mpBar->value == 5.0f)
				{
					drawImage(interfaceDC, position->x + 4 * 64, position->y, "mpEffect", false);
				}

			}
			else
			{
				if (entity->hasComponent<Direction>() && entity->getComponent<Direction>()->value == -1)
				{
					drawImage(fieldDC, position->x, position->y, graphic->key, true);
				}
				else
				{
					drawImage(fieldDC, position->x, position->y, graphic->key, false);
				}
			}
		}

		// 카메라만큼 잘라붙인다.
		StretchBlt(totalDC, 0, 0, 1280, 720, fieldDC, camera->x, camera->y, camera->width, camera->height, SRCCOPY);

		// 인터페이스를 붙인다.
		// 블랜딩 옵션
		BLENDFUNCTION bf;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = 0xFF;
		AlphaBlend(totalDC, 0, 0, 1280, 720, interfaceDC, 0, 0, 1280, 720, bf);

		StretchBlt(hdc, 0, 0, rect_client.right, rect_client.bottom, totalDC, 0, 0, 1280, 720, SRCCOPY);

		// 파괴
		SelectObject(totalDC, hTotalOldBitmap);
		DeleteObject(totalBitmap);
		DeleteDC(totalDC);

		SelectObject(fieldDC, hFieldOleBitmap);
		DeleteObject(fieldBitmap);
		DeleteDC(fieldDC);

		SelectObject(interfaceDC, hInterfaceOleBitmap);
		DeleteObject(interfaceBitmap);
		DeleteDC(interfaceDC);

		EndPaint(hWnd, &ps);
	}
};
