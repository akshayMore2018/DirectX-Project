cbuffer mycBuffer : register(b0)
{
	float x;
	float y;
};

struct VSOut
{
	float2 texCoord: Texcoord;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position,float2 texCoord: Texcoord)
{
	VSOut vso;
	pos.x += x;
	pos.y += y;
	vso.pos = float4(pos.x, pos.y, pos.z, 1.0f);
	vso.texCoord = texCoord;
	return vso;
}