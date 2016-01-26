#pragma pack_matrix(row_major) // this changes the matrix major
struct V_IN
{
	float3 pos : POS;
	float3 uvm : UVM;
	float3 nrm : NRM; 
	float3 tan : TAN;
	float3 bin : BIN;
	int4   bone     :   BID;
	float4 weights  :   BWT;
	//float4 weights  :   BWT;
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

cbuffer SKINNEDOBJECT : register(b3)
{
	float4x4 gSkinnedMatrices[50];	     
}


VS_OUTPUT main(V_IN input) 
{
	VS_OUTPUT output;

	//input.bone = float3(0.0f, 0.0f, 0.0f );
	//input.weights = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 localH = float4(input.pos.xyz, 1);
	localH = mul(localH, worldMatrix);
	localH = mul(localH, view_matrix);
	localH = mul(localH, projection_matrix);

	// transform tangents and normals into camera space
	output.pos = float4(localH);
	output.normal = mul(float4(normalize(input.nrm),0.0f), worldMatrix).xyz;
	output.tan = mul(float4(normalize(input.tan),0.0f), worldMatrix).xyz;
	output.bin = mul(float4(normalize(input.bin),0.0f), worldMatrix).xyz;
	
	output.tan = mul(float4(input.tan.xyz, 0.0f), worldMatrix).xyz;
	output.bin = mul(float4(cross(input.nrm.xyz, input.tan.xyz), 0.0f), worldMatrix).xyz;

	output.wPos = mul(float4(input.pos.xyz, 1.0f), worldMatrix);
	output.TexCoord = float2(input.uvm.xy);

	// skinned animation recalculation
	//float4x4 skinMat = gSkinnedMatrices[input.bone.x] * input.weights.x;
	//skinMat += gSkinnedMatrices[input.bone.y] * input.weights.y;
	//skinMat += gSkinnedMatrices[input.bone.z] * input.weights.z;
	//skinMat += gSkinnedMatrices[input.bone.w] * input.weights.w;
	//output.pos = mul(float4(input.pos, 1.0), skinMat);
	//output.normal = mul(float4(input.nrm, 0.0), skinMat).xyz;



	// for the normals
	float3x3 TBN = { input.tan, input.bin, input.nrm };
	float3 nrm = mul(input.nrm, TBN);
	output.tbn = TBN;

	return output;
}