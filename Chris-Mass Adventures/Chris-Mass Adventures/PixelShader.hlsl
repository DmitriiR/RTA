

texture2D	 baseTexture			: register(t0); // first texture
SamplerState filters[2]				: register(s0);

float4 main(
	float4 wPos: POS,
	float3 uvm : UVM,
	float3 nrm : NRM,
	float4 pos : SV_POSITION ) : SV_TARGET
{
	float4 baseColor = baseTexture.Sample(filters[0], uvm.xy);

	float3 norm = normalize(nrm);
	float4 finalColor = baseColor;



	return finalColor;
}