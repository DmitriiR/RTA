#pragma pack_matrix(row_major)

cbuffer DirectionalLight : register(b0)
{
	float4 Dir_Light_Position;
	
}

texture2D	 baseTexture			: register(t0); // first texture
texture2D	 baseTexture_normalMap	: register(t1); // second texture

SamplerState filters[2]				: register(s0);

struct P_IN
{
	float4 wPos: TEXCOORD0;
	float2 uvm : UVM;
	float3 nrm : NRM;
	float4 pos : SV_POSITION;
	float3 tan : TAN;
	float3 bin : BIN;
	float3x3 tbn : TBN;
};

float4 CalcDirLight(float3 surNormals, float4 surColor, float3 pos, float3 dir_lgt_pos)
{
	    float3 light_dir = dir_lgt_pos - pos;
		//float4 light_ambient = float4(0.1f, 0.1f, 0.0f, 1.0f);
		float4 light_diffuse = float4(0.3f, 0.3f, 0.3f, 1.0f);

		float lightIntensity = saturate(dot(-light_dir, surNormals));
		float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
		color = saturate(light_diffuse * surColor * lightIntensity);
		return color;
}
// point light
float4 CalcPointLight(float3 toLight, float3 surNormals, float4 surColor, float4 lightColor)//, float lightIntensity_Normals)
{
	float4 returnColor;

	float min_dist = 1.0f;
	float max_dist = 15.0f;
	float distance = length(toLight);
	float strength = 1 - saturate((distance - min_dist) / (max_dist - min_dist));

	returnColor = strength * lightColor * surColor;//*lightIntensity_Normals;

	return returnColor;
}

float4 main( P_IN input ) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], input.uvm.xy);
	float3 baseColorNormalMap = baseTexture_normalMap.Sample(filters[0], input.uvm.xy).xyz;
	baseColorNormalMap.xyz = (baseColorNormalMap * 2) - 1.0f;

	float4 finalColor = baseColor;
	float3 toLight = Dir_Light_Position - input.wPos.xyz;
	float3 norm = normalize(input.nrm);

	// normal mapping

		//float3x3 TBN = { input.tan, input.bin, input.nrm };
	float3 nrm = mul(input.nrm, input.tbn);
	//baseColorNormalMap = mul(baseColorNormalMap, input.tbn);

	float4 DirectionalLightColor = CalcDirLight(baseColorNormalMap, baseColor, input.wPos.xyz, Dir_Light_Position.xyz);
	
	float4 lightColor_point = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float4 PointLightColor = CalcPointLight(toLight, baseColorNormalMap, baseColor, lightColor_point);//, lightIntensity_Normals);


	finalColor = saturate( DirectionalLightColor + PointLightColor);

	return finalColor;
}