#pragma once

namespace editor_math
{
	template<class T> T min(T a, T b) { return a < b ? a : b; }
	template<class T> T max(T a, T b) { return a < b ? b : a; }
	template<class T> T clamp(T v, T min_v, T max_v) { return min(max(v, min_v), max_v); }
	template<class T> T min(T a, T b, T c) { return min(a, min(b, c)); }

	template<class T> T linear_to_srgb(T linear) {
		if (linear < T(0.0031308f)) {
			return linear * T(12.92f);
		}
		else {
			return T(1.055f) * std::pow(linear, T(1.0f) / T(2.4f)) - T(0.055f);
		}
	}

}
