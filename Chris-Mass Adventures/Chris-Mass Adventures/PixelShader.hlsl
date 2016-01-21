#pragma pack_matrix(row_major)

cbuffer DirectionalLight : register(b0)
{
	float4 Dir_Light_Position;
	
}

texture2D	 baseTexture			: register(t0); // first texture
SamplerState filters[2]				: register(s0);

struct P_IN
{
	float4 wPos: TEXCOORD0;
	float2 uvm : UVM;
	float3 nrm : NRM;
	float4 pos : SV_POSITION;
	float3 tan : TAN;
	float3 bin : BIN;
};

float4 CalcDirLight(float3 surNormals, float4 surColor, float3 pos, float3 dir_lgt_pos)
{
	float3 light_dir = dir_lgt_pos - pos;
		//float4 light_ambient = float4(0.1f, 0.1f, 0.0f, 1.0f);
		float4 light_diffuse = float4(1.0f, 1.0f, 0.3f, 1.0f);

		float lightIntensity = saturate(dot(-light_dir, surNormals));
		float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
		color = saturate(light_diffuse * surColor * lightIntensity);
		return color;
}

float4 main( P_IN input ) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.uvm.xy);

	float3 norm = normalize(input.nrm);

	float3x3 TBN = { input.tan, input.bin, input.nrm };
	float3 nrm = mul(input.nrm, TBN);

	float4 DirectionalLightColor = CalcDirLight(nrm, baseColor, input.wPos, Dir_Light_Position);

	float4 finalColor =  DirectionalLightColor;

	return finalColor;
}