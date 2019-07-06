
Texture2D objTexture : Texture: register(t0);
SamplerState objSamplerState : Sampler: register(s0);


float4 main(float2 texCoord : Texcoord) : SV_Target
{
	float3 pixelColor = objTexture.Sample(objSamplerState, texCoord).xyz;
	return float4(pixelColor,1.0f);
}