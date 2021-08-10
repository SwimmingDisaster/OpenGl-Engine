#include "mypch.h"
#include "core/framebuffer.h"
FrameBuffer::FrameBuffer()
	:msaaSamples(0){}

FrameBuffer::FrameBuffer(float width, float height, int sampleCount)
	:msaaSamples(sampleCount){
	glGenFramebuffers(1, &m_frameBufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);

// create a color attachment texture
	if(msaaSamples == 0) {
		glGenTextures(1, &m_colorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
	}
	else {
		glGenTextures(1, &m_colorBuffer);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffer);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_RGB, (GLsizei)width, (GLsizei)height, GL_TRUE);
		//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffer, 0);
	}

// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer); // now actually attach it

// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	Log(glCheckFramebufferStatus(GL_FRAMEBUFFER));
	Log(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
	Log(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS);
	Log(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
	Log(GL_FRAMEBUFFER_UNSUPPORTED);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Error("Framebuffer is not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void FrameBuffer::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
}
void FrameBuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
unsigned int FrameBuffer::GetData() const {
	return m_colorBuffer;
}
unsigned int FrameBuffer::GetObject() const {
	return m_frameBufferObject;
}
void FrameBuffer::Resize(int width, int height) const {
	//glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
	if(msaaSamples == 0) {
		glBindTexture(GL_TEXTURE_2D, m_colorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0);
	}
	else {
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffer);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_RGB, (GLsizei)width, (GLsizei)height, GL_TRUE);
		//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_colorBuffer, 0);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height); // use a single renderbuffer object for both a depth AND stencil buffer.

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
