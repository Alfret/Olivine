struct VsInput
{
  float3 pos : POSITION;
  float4 col : COLOR;
  float2 uv : TEXCOORD;
};

struct PsInput
{
  float4 pos : SV_POSITION;
  float4 col : COLOR;
  float2 uv : TEXCOORD;
};

PsInput
main(VsInput input)
{
  PsInput output;
  output.pos = float4(input.pos, 1.0f);
  output.col = input.col;
  output.uv = input.uv;
  return output;
}