struct VSOut
{
	float3 color: Color;
	float4 pos : SV_Position;
};

VSOut main(float3 pos : Position,float3 color: Color)
{
	VSOut vso;
	vso.pos = float4(pos.x, pos.y, pos.z, 1.0f);
	vso.color = float4(color.x, color.y, color.z, 1.0f);
	return vso;
}