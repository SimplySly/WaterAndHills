cbuffer matrices
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexInput
{
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexOutput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

VertexOutput main(VertexInput Input)
{
	VertexOutput Output;

	Input.pos.w = 1.f;
	Output.pos = mul(Input.pos, world);
	Output.pos = mul(Output.pos, view);
	Output.pos = mul(Output.pos, projection);

	Output.tex = Input.tex;

	Output.normal = mul(Input.normal, (float3x3)world);
	Output.normal = normalize(Output.normal);

	return Output;
}