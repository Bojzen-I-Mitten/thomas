#pragma once
#include "../Common.h"
#include "../utils/Math.h"
#include "Resource.h"
#include <vector>
#include <memory>
namespace thomas {
	namespace graphics
	{
		class Mesh;
	}
	namespace resource {
		
		class THOMAS_API Model : public Resource {
		private:
			math::BoundingBox GenerateBounds();
			void OnChanged();
		public:
			Model(std::string path);

			std::vector<std::shared_ptr<graphics::Mesh>> GetMeshes();

			~Model();
			
		public:
			math::BoundingBox m_bounds;
		private:
			std::vector<std::shared_ptr<graphics::Mesh>> m_meshes;
						
		};
	}
}