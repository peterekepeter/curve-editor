#pragma once

// Small and simple software rendering library!
// Include this file from a CPP file with PUHA_LIB_IMPLEMENTATION defined!

class Gfx320x200
{
protected:
	int* buffer; // virtual buffer, pointer inside the real buffer
	int* real_buffer;
	int color;

public:

	Gfx320x200();
	~Gfx320x200();
	Gfx320x200(const Gfx320x200&) = delete;
	Gfx320x200(Gfx320x200&&) = delete;
	Gfx320x200& operator =(const Gfx320x200&) = delete;
	Gfx320x200& operator =(Gfx320x200&&) = delete;

	void SetColor(int C = 0xffffff) { color = C; }
	int  GetColor() { return color; }
	void PutPixel(int x, int y) { buffer[x + (y << 6) + (y << 8)] = color; }
	int  GetPixel(int x, int y) { return buffer[x + (y << 6) + (y << 8)]; }

	void Span(int x, int y, int w);
	void HLine(int x1, int y1, int x2, int y2 = 0);
	void VLine(int x1, int y1, int x2, int y2);
	void Rectangle(int x1, int y1, int x2, int y2);
	void RectangleFilled(int x1, int y1, int x2, int y2);
	void Line(int, int, int, int);

	void PutChar(int, int, char);
	void PutStr(int, int, char*);
	bool AreBoundsValid(); // checks memory before and after buffer, returns false if it was accidentally written
	
	unsigned char* GetBytePtr(int x, int y) { return (unsigned char*)(&buffer[x + (y << 6) + (y << 8)]); }
	int* GetBuffer() { return buffer; }
};