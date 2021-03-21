struct PixelInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 norm : NORMAL;
};

Texture2D Texture;
SamplerState Sampler;

float4 main(PixelInput Input) : SV_TARGET
{
	float4 Color;

	Color = Texture.Sample(Sampler, Input.tex);

	return Color;
}