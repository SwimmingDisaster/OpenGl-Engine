#pragma once

class FrameBuffer {
private:
	unsigned int m_frameBufferObject{};
	unsigned int m_colorBuffer{};
	unsigned int m_depthBuffer{};
	
	int msaaSamples;

public:
	FrameBuffer(float width, float height, int sampleCount = 0);
	FrameBuffer();
	FrameBuffer(const FrameBuffer& other) = default;
	FrameBuffer(FrameBuffer&& other) = default;
	FrameBuffer& operator=(const FrameBuffer& other) = default;
	FrameBuffer& operator=(FrameBuffer&& other) = default;

public:
	void Bind() const;
	static void Unbind();
	[[nodiscard]] unsigned int GetData() const;
	[[nodiscard]] unsigned int GetObject() const;
	void Resize(int width, int height) const;


};
