#pragma pack_matrix(row_major) // this changes the matrix major
struct V_IN
{
	float3 pos : POS;
	float3 uvm : UVM;
	float3 nrm : NRM; 
	float3 tan : TAN;
	float3 bin : BIN;
};

struct VS_OUTPUT
{
	//float4 posH : SV_POSITION;// homoinzed rasterized, on the inbound the value is the same as the vLayour, on the output the pixel shader needs to match the PS parameter 

	float4		wPos			: TEXCOORD0;
	float2      TexCoord		: UVM;
	float3      normal			: NRM;
	float4		pos				: SV_POSITION;
	float3		tan				: TAN;
	float3		bin				: BIN;
	float3x3	tbn				: TBN;
	//float3      lightPosition	: POSITION;
	//float3		dir_lgt_pos		: DIR_POS;

};

cbuffer OBJECT : register(b0)
{
	float4x4 worldMatrix;       // world matrix 

}
cbuffer SCENE : register(b1)
{
	float4x4 view_matrix; // view 
	float4x4 projection_matrix;	      // projection
}



VS_OUTPUT main(V_IN input) 
{
	VS_OUTPUT output;

	float4 localH = float4(input.pos.xyz, 1);
	localH = mul(localH, worldMatrix);
	localH = mul(localH, view_matrix);
	localH = mul(localH, projection_matrix);

	// transform tangents and normals into camera space
	output.pos = float4(localH);
	output.normal = mul(normalize(input.nrm), worldMatrix);
	output.tan = mul(normalize(input.tan), worldMatrix);
	output.bin = mul(normalize(input.bin), worldMatrix);
	
	output.tan = mul(float4(input.tan.xyz, 0.0f), worldMatrix);
	output.bin = mul(float4(cross(input.nrm.xyz, input.tan.xyz), 0.0f), worldMatrix);

	output.wPos = mul(float4(input.pos.xyz, 1.0f), worldMatrix);
	output.TexCoord = float2(input.uvm.xy);

	float3x3 TBN = { input.tan, input.bin, input.nrm };
	float3 nrm = mul(input.nrm, TBN);
	output.tbn = TBN;

	return output;
}