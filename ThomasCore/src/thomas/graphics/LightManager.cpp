#include "LightManager.h"
#include "../utils/d3d.h"
#include "../graphics/Shader.h"
namespace thomas
{
	namespace graphics
	{
		LightManager::LightManager()
		{
		}

		LightManager::~LightManager()
		{
			SAFE_RELEASE(s_lightBuffer);
			s_lightBuffer = NULL;
		}

		LightManager::LightBufferStruct LightManager::s_lightstruct;
		ID3D11Buffer* LightManager::s_lightBuffer;
		bool LightManager::UpdateLightBuffer()
		{
			if (!s_lightBuffer)
			{
				s_lightBuffer = thomas::utils::D3d::CreateBufferFromStruct(s_lightstruct, D3D11_BIND_CONSTANT_BUFFER);
				return true;
			}
			else
				return thomas::utils::D3d::FillBuffer(s_lightBuffer, s_lightstruct);

		}

		int LightManager::AddDirectionalLight(DirectionalLightStruct directionalLight)
		{
			int maxlength = sizeof(s_lightstruct.directionalLights) / sizeof(DirectionalLightStruct);
			if (maxlength > s_lightstruct.nrOfDirectionalLights)
			{
				s_lightstruct.directionalLights[s_lightstruct.nrOfDirectionalLights] = directionalLight;
				s_lightstruct.nrOfDirectionalLights++;
				UpdateLightBuffer();
				return s_lightstruct.nrOfDirectionalLights - 1;
			}
			else//log - to many lights
				return -1;

		}

		bool LightManager::UpdateDirectionalLight(DirectionalLightStruct other, int index)
		{
			s_lightstruct.directionalLights[index] = other;
			
			return UpdateLightBuffer();
		}
		bool LightManager::UpdatePointLight(PointLightStruct other, int index)
		{
			s_lightstruct.pointLights[index] = other;

			return UpdateLightBuffer();
		}

		int LightManager::AddPointLight(PointLightStruct pointLight)
		{
			int maxlength = sizeof(s_lightstruct.pointLights) / sizeof(PointLightStruct);
			if (maxlength > s_lightstruct.nrOfPointLights)
			{
				s_lightstruct.pointLights[s_lightstruct.nrOfPointLights] = pointLight;
				s_lightstruct.nrOfPointLights++;
				UpdateLightBuffer();
				return s_lightstruct.nrOfPointLights - 1;
			}
			else//log - to many lights
				return -1;
		}

		bool LightManager::BindDirectionalLight(unsigned int index)
		{
			return false;
		}
		bool LightManager::BindPointLight(unsigned int index)
		{
			return false;
		}
		void LightManager::Destroy()
		{
			SAFE_RELEASE(s_lightBuffer);
			s_lightBuffer = NULL;
			s_lightstruct.nrOfDirectionalLights = 0;
			s_lightstruct.nrOfPointLights = 0;
		}
	}
}