#pragma once

struct HINSTANCE__;
struct HMENU__;
struct HWND__;
struct HFONT__;

namespace WinAPI::details
{
struct Rect
{
	long left;
	long top;
	long right;
	long bottom;

	template<class T>
	operator T()
	{
		return T {.left = left, .top = top, .right = right, .bottom = bottom};
	}
};

struct Point
{
	Point() = default;
	Point(long _x, long _y) : x(_x), y(_y)
	{}

	template<class T>
	Point(T t) : x(t.cx), y(t.cy)
	{}

	long x;
	long y;

	template<class T>
	operator T()
	{
		return T {.cx = x, .cy = y};
	}
};

using HINSTANCE = HINSTANCE__*;
using HMENU = HMENU__*;
using HWND = HWND__*;
using HFONT = HFONT__*;

using DWORD = unsigned long;
using LPCWSTR = const wchar_t*;

using LRESULT = long long;
using LPARAM = long long;
using WPARAM = unsigned long long;
using UINT = unsigned int;
using LONG_PTR = long long;
}
