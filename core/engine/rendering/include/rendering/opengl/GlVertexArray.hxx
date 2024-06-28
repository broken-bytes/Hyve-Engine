#pragma once

#include "rendering/opengl/GlIndexBuffer.hxx"
#include "rendering/opengl/GlVertexBuffer.hxx"
#include "rendering/VertexArray.hxx"

#include <cstdint>
#include <memory>

namespace kyanite::engine::rendering::opengl {
	class GlVertexArray: public VertexArray {
	public:
		GlVertexArray(
			std::shared_ptr<GlIndexBuffer> ib, 
			std::shared_ptr<GlVertexBuffer> vb
		) : VertexArray(ib, vb){
			glGenVertexArrays(1, &_id);
		};
		virtual ~GlVertexArray() {
			glDeleteVertexArrays(1, &_id);
		}
		virtual uint32_t Id() const override {
			return _id;
		}
		virtual void Bind() const override {
			glBindVertexArray(_id);
		}

		virtual auto Indices() const->uint32_t override {
			auto ib = std::dynamic_pointer_cast<GlIndexBuffer>(IndexBuffer());
			return ib->Size();
		}


	private:
		uint32_t _id;
	};
}