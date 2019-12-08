// Textures
Texture2D tex_albedo : register(t0);
Texture2D tex_roughness : register(t1);
Texture2D tex_metallic : register(t2);
Texture2D tex_normal : register(t3);
SamplerState samp : register(s0);

// Camera information
cbuffer Camera : register(b2)
{
  float4x4 proj_mat;
  float4x4 view_mat;
};

// Light data (Single light at the moment)
cbuffer LightData : register(b1)
{
  float4 light_pos;
  float4 light_col;
};

// ==========================================================================//

static const float PI = 3.14159265f;
static const float GAMMA = 2.2f;

// ==========================================================================//

// Input from vertex shader
struct PsInput
{
  float4 pos : SV_POSITION;
  float3 world_pos : POSITION;
  float3 normal : NORMAL;
  float2 uv : TEXCOORD;
};

// ==========================================================================//

float4
sample_tex_linear(Texture2D tex, float2 uv)
{
  float4 output = tex.Sample(samp, uv);
  output.rgb = pow(output.rgb, float3(GAMMA, GAMMA, GAMMA));
  return output;
}

float4
sample_tex(Texture2D tex, float2 uv)
{
  return tex.Sample(samp, uv);
}

// ==========================================================================//

float3
normal_from_map(float3 world_pos, float2 uv, float3 normal)
{
  float3 tangentNormal = mul(tex_normal.Sample(samp, uv).xyz, 2.0) - 1.0;

  float3 Q1 = ddx(world_pos);
  float3 Q2 = ddy(world_pos);
  float2 st1 = ddx(uv);
  float2 st2 = ddy(uv);

  float3 N = normalize(normal);
  float3 T = normalize(st2.y * Q1 - st1.y * Q2);
  float3 B = -normalize(cross(N, T));
  float3x3 TBN = float3x3(T, B, N);

  return normalize(mul(tangentNormal, TBN));
}

float
distribution_ggx(float3 N, float3 H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}

float
geometry_schlick_ggx(float NdotV, float roughness)
{
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;
  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;
  return nom / denom;
}

float
geometry_smith(float3 N, float3 V, float3 L, float roughness)
{
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = geometry_schlick_ggx(NdotV, roughness);
  float ggx1 = geometry_schlick_ggx(NdotL, roughness);
  return ggx1 * ggx2;
}

float3
fresnel_schlick(float cosTheta, float3 F0)
{
  return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
  // return max(F0 + (1.0 - F0) * pow(1.0 - min(cosTheta, 1.0), 5.0), 0.0);
}

// ==========================================================================//

// Main
float4
main(PsInput input)
  : SV_TARGET
{
  float3 albedo = sample_tex_linear(tex_albedo, input.uv).rgb;
  float roughness = sample_tex(tex_roughness, input.uv).r;
  float metallic = sample_tex(tex_metallic, input.uv).r;
  float ao = 1.0f;

  float3 camera_pos = float3(0.0f, 0.0f, 0.0f);

  float3 N = normal_from_map(input.world_pos, input.uv, input.normal);
  float3 V = normalize(camera_pos - input.world_pos);

  float3 F0 = float3(0.04, 0.04, 0.04);
  F0 = lerp(F0, albedo, metallic);

  // Light
  float3 L = normalize(light_pos.xyz - input.world_pos);
  float3 H = normalize(V + L);
  float distance = length(light_pos.xyz - input.world_pos);
  float attenuation = 1.0 / (distance * distance);
  float3 radiance = light_col.rgb * attenuation;

  // Cook-Torrance BRDF
  float NDF = distribution_ggx(N, H, roughness);
  float G = geometry_smith(N, V, L, roughness);
  float3 F = fresnel_schlick(max(dot(H, V), 0.0), F0);
  float3 nominator = NDF * G * F;
  float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
  float3 specular = nominator / denominator;
  float3 kS = F;
  float3 kD = (1.0f - kS) * (1.0 - metallic);
  float NdotL = max(dot(N, L), 0.0);
  float3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
  float3 ambient = 0.03 * albedo * ao;

  // Output gamma corrected
  float3 output = ambient + Lo;
  output = output / (output + 1.0);
  output = pow(output, float3(1.0 / GAMMA, 1.0 / GAMMA, 1.0 / GAMMA));
  return float4(output, 1.0f);
}