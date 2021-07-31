#pragma once

class FrameBuffer {
private:
	unsigned int m_framebufferObject{};
	unsigned int m_colorBuffer{};
	unsigned int m_depthBuffer{};

public:
	FrameBuffer(float width, float height);
	FrameBuffer() {}

public:
	void Bind() const;
	static void Unbind();
	[[nodiscard]] unsigned int GetData() const;
	void Resize(int width, int height) const;


};
