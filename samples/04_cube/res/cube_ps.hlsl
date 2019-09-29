Texture2D tex : register(t0);
SamplerState samp : register(s0);

struct PsInput
{
  float4 pos : SV_POSITION;
  float4 normal : NORMAL;
  float2 uv : TEXCOORD;
};

float4
main(PsInput input)
  : SV_TARGET
{
  return tex.Sample(samp, input.uv);
}