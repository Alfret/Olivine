cbuffer Data : register(b0)
{
  float4x4 mvp;
};

struct VsInput
{
  float3 pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD;
};

struct PsInput
{
  float4 pos : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : TEXCOORD;
};

PsInput
main(VsInput input)
{
  PsInput output;
  output.pos = mul(float4(input.pos, 1.0f), mvp);
  output.normal = float4(input.normal, 0.0f);
  output.uv = input.uv;
  return output;
}