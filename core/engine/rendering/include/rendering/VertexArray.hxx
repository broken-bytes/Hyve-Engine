#pragma once

#include "Buffer.hxx"
#include "IndexBuffer.hxx"
#include "VertexBuffer.hxx"

#include <cstdint>
#include <memory>

namespace kyanite::engine::rendering {
	class VertexArray {
	public:
		VertexArray(
			std::shared_ptr<IndexBuffer> ib, 
			std::shared_ptr<VertexBuffer> vb
		) : _indexBuffer(ib), _vertexBuffer(vb) {};
		virtual ~VertexArray() = 0 {}
		virtual auto Id() const->uint32_t = 0;
		virtual auto Bind() const->void = 0;
		virtual auto Indices() const->uint32_t = 0;

		auto IndexBuffer() const -> std::shared_ptr<IndexBuffer> { return _indexBuffer; }
		auto VertexBuffer() const -> std::shared_ptr<VertexBuffer> { return _vertexBuffer; }

	private:
		std::shared_ptr<engine::rendering::IndexBuffer> _indexBuffer;
		std::shared_ptr<engine::rendering::VertexBuffer> _vertexBuffer;
	};
}