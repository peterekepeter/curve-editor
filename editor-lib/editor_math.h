#pragma once

namespace editor_math
{
	template<class T> T min(T a, T b) { return a < b ? a : b; }
	template<class T> T max(T a, T b) { return a < b ? b : a; }
	template<class T> T clamp(T v, T min_v, T max_v) { return min(max(v, min_v), max_v); }
	template<class T> T min(T a, T b, T c) { return min(a, min(b, c)); }
}