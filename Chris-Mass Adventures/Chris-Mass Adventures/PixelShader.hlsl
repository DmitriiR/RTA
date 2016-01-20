#include "SharedDefines.h"
//#include "HeaderLights.hlsli"
texture2D	 baseTexture			: register(t0); // first texture
SamplerState filters[2]				: register(s0);

struct P_IN
{
	float4 wPos: POS;
	float2 uvm : UVM;
	float3 nrm : NRM;
	float4 pos : SV_POSITION;

};


float4 main( P_IN input ) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.uvm.xy);

	float3 norm = normalize(input.nrm);
	float4 DirectionalLightColor = CalcDirLight(norm, baseColor, input.pos, dir_light_pos);

	float4 finalColor = baseColor + DirectionalLightColor;



	return finalColor;
}