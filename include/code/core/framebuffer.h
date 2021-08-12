#pragma once

class FrameBuffer {
protected:
	unsigned int m_frameBufferObject{};
	unsigned int m_color{};
	unsigned int m_depthBuffer{};
	
	int msaaSamples = 0;

public:
	FrameBuffer(float width, float height, int sampleCount = 0) {}
	FrameBuffer() = default;
	virtual ~FrameBuffer() {}
	/*
	FrameBuffer(const FrameBuffer& other) = default;
	FrameBuffer(FrameBuffer&& other) = default;
	FrameBuffer& operator=(const FrameBuffer& other) = default;
	FrameBuffer& operator=(FrameBuffer&& other) = default;
	*/

public:
	static void Unbind();
	void Bind() const;

	virtual void Resize(int width, int height) const = 0;
	virtual void Delete() = 0;
	[[nodiscard]] virtual unsigned int GetData() const = 0;
	[[nodiscard]] virtual unsigned int GetObject() const = 0;

};

class ForwardFrameBuffer : public FrameBuffer {
public:
	ForwardFrameBuffer(float width, float height, int sampleCount = 0);
	ForwardFrameBuffer() = default;
	virtual ~ForwardFrameBuffer() {}
public:
	void Resize(int width, int height) const override;
	void Delete() override;
	[[nodiscard]] unsigned int GetData() const override;
	[[nodiscard]] unsigned int GetObject() const override;
};

class DeferredFrameBuffer : public FrameBuffer{
private:
	unsigned int m_position{};
	unsigned int m_normal{};
public:
	DeferredFrameBuffer(float width, float height, int sampleCount = 0);
	DeferredFrameBuffer() = default;
	virtual ~DeferredFrameBuffer() {}
public:
	void Resize(int width, int height) const override;
	void Delete() override;
	[[nodiscard]] unsigned int GetData() const override;
	[[nodiscard]] unsigned int GetObject() const override;
};
