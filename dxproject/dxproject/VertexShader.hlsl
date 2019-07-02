cbuffer mycBuffer : register(b0)
{
	float4x4 mat;
};

struct VSOut
{
	float2 texCoord: Texcoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position,float2 texCoord: Texcoord)
{
	VSOut vso;
	vso.pos = mul(float4(pos.x, pos.y, pos.z, 1.0f),mat);
	vso.texCoord = texCoord;
	return vso;
}