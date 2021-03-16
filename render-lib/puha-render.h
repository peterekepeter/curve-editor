#pragma once

// Small and simple software rendering library!

template<class Gfx> class ClippedGfx;

class Gfx320x200
{
protected:
	int* buffer; // virtual buffer, pointer inside the real buffer
	int* real_buffer;
	int color;

public:

	// this class is harcoded to tis size
	const int Width = 320;
	const int Height = 200;

	Gfx320x200();
	~Gfx320x200();
	Gfx320x200(const Gfx320x200&) = delete;
	Gfx320x200(Gfx320x200&&) = delete;
	Gfx320x200& operator =(const Gfx320x200&) = delete;
	Gfx320x200& operator =(Gfx320x200&&) = delete;

	// pixels
	void SetColor(int C = 0xffffff) { color = C; }
	int  GetColor() { return color; }
	void PutPixel(int x, int y) { PutPixel(x,y,color); }
	void PutPixel(int x, int y, int c) { buffer[x + (y << 6) + (y << 8)] = c; }
	int  GetPixel(int x, int y) { return buffer[x + (y << 6) + (y << 8)]; }

	// lines
	void Span(int x, int y, int w) { Span(x, y, color); }
	void Span(int x, int y, int w, int color);
	void HLine(int x1, int y1, int x2, int y2 = 0) { HLine(x1, y1, x2, y2, color); }
	void HLine(int x1, int y1, int x2, int y2, int color);
	void VLine(int x1, int y1, int x2, int y2) { VLine(x1, y1, x2, y2, color); }
	void VLine(int x1, int y1, int x2, int y2, int color);
	void Rectangle(int x1, int y1, int x2, int y2) { Rectangle(x1, y1, x2, y2, color); }
	void Rectangle(int x1, int y1, int x2, int y2, int color);
	void Line(int x0, int y0, int x1, int y1) { Line(x0, y0, x1, y1, color); }
	void Line(int, int, int, int, int color);

	// filled shapes
	void RectangleFilled(int x1, int y1, int x2, int y2) { RectangleFilled(x1, y1, x2, y2, color); }
	void RectangleFilled(int x1, int y1, int x2, int y2, int color);
	void RectFill(int x, int y, int w, int h) { RectFill(x, y, w, h, color); }
	void RectFill(int x, int y, int w, int h, int color);

	// text
	void PutChar(int x, int y, char c, int color);
	void PutChar(int x, int y, char c) { PutChar(x, y, c, color); };
	void PutStr(int x, int y, char* s, int color);
	void PutStr(int x, int y, char* s) { PutStr(x, y, s, color); };

	// memory
	bool AreBoundsValid(); // checks memory before and after buffer, returns false if it was accidentally written
	void ResetBoundsValidity(); // resets boundary error detection
	unsigned char* GetBytePtr(int x, int y) { return (unsigned char*)(&buffer[x + (y << 6) + (y << 8)]); }
	int* GetBuffer() { return buffer; }

	ClippedGfx<Gfx320x200> CreateViewport();
	ClippedGfx<Gfx320x200> CreateViewport(int x, int y, int w, int h);
};

template<class Gfx>
class ClippedGfx
{
private:
	Gfx& gfx;
	int x0, y0, x1, y1;
	int color;

public:
	ClippedGfx(Gfx& gfx, int x0, int y0, int x1, int y1) : gfx(gfx), x0(x0), y0(y0), x1(x1), y1(y1) { }
	ClippedGfx(const ClippedGfx& o) : gfx(o.gfx), x0(o.x0), y0(o.y0), x1(o.x1), y1(o.y1) { }

	// clipping
	bool IsInsideX(int x) { return x0 <= x && x < x1; }
	bool IsInsideY(int y) { return y0 <= y && y < y1; }
	bool IsInside(int x, int y) { return IsInsideX(x) && IsInsideY(y); }

	// pixels
	void SetColor(int C = 0xffffff) { color = C; }
	int  GetColor() { return color; }
	void PutPixel(int x, int y) { if (IsInside(x,y)) gfx.PutPixel(x, y, color); }
	void PutPixel(int x, int y, int color) { if (IsInside(x, y)) gfx.PutPixel(x, y, color); }
	int  GetPixel(int x, int y) { return IsInside(x, y) ? gfx.GetPixel(x, y) : 0; }
	
	// lines
	void Span(int x, int y, int w) { Span(x, y, color); }
	void Span(int x, int y, int w, int color);
	void HLine(int x1, int y1, int x2, int y2 = 0) { HLine(x1, y1, x2, y2, color); }
	void HLine(int x1, int y1, int x2, int y2, int color);
	void VLine(int x1, int y1, int x2, int y2) { VLine(x1, y1, x2, y2, color); }
	void VLine(int x1, int y1, int x2, int y2, int color);
	void Rectangle(int x1, int y1, int x2, int y2) { Rectangle(x1, y1, x2, y2, color); }
	void Rectangle(int x1, int y1, int x2, int y2, int color);

	// filled shapes
	void RectangleFilled(int x1, int y1, int x2, int y2) { RectangleFilled(x1, y1, x2, y2, color); }
	void RectangleFilled(int x1, int y1, int x2, int y2, int color) { RectFill(x1, y1, x2-x1+1, y2-y1+1, color); }
	void RectFill(int x, int y, int w, int h) { RectFill(x, y, w, h, color); }
	void RectFill(int x, int y, int w, int h, int color);

	// text
	void PutChar(int x, int y, char c, int color);
	void PutChar(int x, int y, char c) { PutChar(x, y, c, color); };
	void PutStr(int x, int y, char* s, int color);
	void PutStr(int x, int y, char* s) { PutStr(x, y, s, color); };

	// memory
	bool AreBoundsValid() { return gfx.AreBoundsValid(); }
	unsigned char* GetBytePtr(int x, int y) { IsInside(x, y) ? gfx.GetBytePtr(x, y) : nullptr; }
	int* GetBuffer() { return gfx.GetBuffer(); }
};

template<class Gfx>
inline void ClippedGfx<Gfx>::Span(int x, int y, int w, int color)
{
	if (!IsInsideY(y)) {
		return;
	}
	if (x < x0) {
		int dx = x0 - x;
		x += dx;
		w -= w;
	}
	if (x + w > x1) {
		int dx = x + w - x1;
		w -= dx;
	}
	if (w > 0) {
		gfx.Span(x, y, w, color);
	}
}

template<class Gfx>
inline void ClippedGfx<Gfx>::HLine(int x1, int y1, int x2, int y2, int color)
{
	Span(x1, y1, x2 - x1 + 1, color);
}

template<class Gfx>
inline void ClippedGfx<Gfx>::VLine(int x0, int y0, int x1, int y1, int color)
{
	if (!IsInsideX(x0))
	{
		return;
	}
	if (y0 < this->y0) {
		y0 = this->y0;
	}
	if (y1 >= this->y1) {
		y1 = this->y1 - 1;
	}
	gfx.VLine(x0, y0, x1, y1, color);
}

template<class Gfx>
inline void ClippedGfx<Gfx>::Rectangle(int x1, int y1, int x2, int y2, int color)
{
	HLine(x1, y1, x2, y1, color);
	HLine(x1, y2, x2, y2, color);
	VLine(x1, y1, x1, y2, color);
	VLine(x2, y1, x2, y2, color);
}

template<class Gfx>
inline void ClippedGfx<Gfx>::RectFill(int x, int y, int w, int h, int color)
{
	if (x < x0) {
		int dx = x0 - x;
		x += dx;
		w -= dx;
	}
	if (y < y0) {
		int dy = y0 - y;
		y += dy;
		h -= dy;
	}
	if (x + w > x1) {
		int dx = x + w - x1;
		w -= dx;
	}
	if (y + h > y1) {
		int dy = y + h - y1;
		h -= dy;
	}
	if (w > 0 && h > 0) {
		gfx.RectFill(x, y, w, h, color);
	}
}

template<class Gfx>
inline void ClippedGfx<Gfx>::PutChar(int x, int y, char c, int color)
{
	const int size = 5;
	if (IsInside(x, y) && IsInside(x + size, y + size)) {
		gfx.PutChar(x, y, c, color);
	}
}

template<class Gfx>
inline void ClippedGfx<Gfx>::PutStr(int x, int y, char* s, int color)
{
	const int size = 5;
	while (*s)
	{
		PutChar(x, y, *s, color);
		x += size + 1;
		s++;
	}
}
