struct PsInput
{
  float4 pos : SV_POSITION;
  float4 col : COLOR;
};

float4
main(PsInput input)
  : SV_TARGET
{
  // return interpolated color
  return input.col;
}