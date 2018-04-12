#pragma once

#include <string>
#include "../../Common.h"
#include <d3d11.h>
#include "../Resource.h"
namespace thomas
{
	namespace resource
	{
		class THOMAS_API Texture : public Resource
		{

		public:

			enum class WrapMode {
				Wrap,
				Clamp,
				Decal,
				Mirror
			};

			enum class Dimension {
				Unknown = -1,
				None = 0,
				Tex1D = 1,
				Tex2D = 2,
				Tex3D = 3,
				Cube = 4,
				Any = 6
			};

			enum class FilterMode {
				Point,
				Bilinear,
				Trilinear
			};

		private:
			virtual void OnChanged();

		public:
			Texture() : Resource("") {}
			Texture(std::string path) : Resource(path) {};
			virtual ~Texture();

			WrapMode GetWrapMode();
			void SetWrapMode(WrapMode mode);
			Dimension GetDimension();
			FilterMode GetFilterMode();
			void SetFilterMode(FilterMode mode);
			int GetAnistropicLevel();
			void SetAnistropicLevel(int value);
			int GetWidth();
			int GetHeight();
			
			ID3D11ShaderResourceView* GetResourceView();


		protected:
			WrapMode m_wrapMode;
			Dimension m_texDim;
			FilterMode m_filterMode;
			int m_anistropicFilteringLevel;
			int m_width;
			int m_height;

			ID3D11Resource* m_resource;
			ID3D11ShaderResourceView* m_srv;
		};
	}
}