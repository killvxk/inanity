#ifndef ___INANITY_INANITY_GL_HPP___
#define ___INANITY_INANITY_GL_HPP___

/* Подсистема OpenGL. */

#include "Object.hpp"

#include "graphics/GlAttributeBinding.hpp"
#include "graphics/GlBlendState.hpp"
#include "graphics/GlContext.hpp"
#include "graphics/GlDepthStencilBuffer.hpp"
#include "graphics/GlDevice.hpp"
#include "graphics/GlIndexBuffer.hpp"
#include "graphics/GlInternalProgram.hpp"
#include "graphics/GlInternalProgramCache.hpp"
#include "graphics/GlInternalTexture.hpp"
#include "graphics/GlPixelShader.hpp"
#include "graphics/GlRenderBuffer.hpp"
#include "graphics/GlSamplerState.hpp"
#include "graphics/GlShaderBindings.hpp"
#include "graphics/GlShaderCompiler.hpp"
#include "graphics/GlslSource.hpp"
#include "graphics/GlSystem.hpp"
#include "graphics/GlTexture.hpp"
#include "graphics/GlUniformBuffer.hpp"
#include "graphics/GlVertexBuffer.hpp"
#include "graphics/GlVertexShader.hpp"
#ifdef ___INANITY_WINDOWS
#include "graphics/WglPresenter.hpp"
#endif
#ifdef ___INANITY_LINUX
#include "graphics/GlxPresenter.hpp"
#endif

#endif