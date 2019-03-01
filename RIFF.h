#pragma once
#include <cstdio>
#include <fstream>
#include <tchar.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <Windows.h>
#include "ReadStream.h"
#include "WriteStream.h"
#include <impapi.h>
#include <maxapi.h>
#include "pugixml.hpp"
#include <algorithm>
#pragma pack(push, 1)
struct RIFFSectionHeader
{
	char Name[4];
	UINT Size;
	
};
struct SSim
{
	std::string Variable = "";
	std::string Units = "";
	bool ScaleF = false;
	bool BiasF = false;
	float Scale = 1.0;
	float Bias = 0.0;
};
struct SParameter
{
	bool CodeF = false;
	bool SimF = false;
	bool LagF = false;
	std::string Code = "";
	SSim Sim;
	float Lag = 0.0;
};
struct SAnimationPart
{
	SParameter Parameter;
};
struct SVisible
{
	SParameter Parameter;
};



enum AnimType
{
	Standard,
	Sim
};
struct SAnimation
{
	std::string name;
	std::string guid;
	AnimType type;
	std::string typeParam;
	float length;
	std::string typeParam2;
};
struct SVisibleInRange
{
	bool MinValueB;
	bool MaxValueB;
	SParameter Parameter;
	int MinValue;
	int MaxValue;
};
struct SPartInfo
{
	std::string Name = "";
	std::string Copy = "";
	int AnimLength = 100;
	bool AnimationF = false;
	bool VisibleF = false;
	bool VisibleInRangeF = false;
	
	SAnimationPart Animation;
	SVisible Visible;
	SVisibleInRange VisibleInRange;

};





struct SRIFFData
{
	char Version[4];

};
struct SMDLH
{
	int Magic;
	float Version;

};
struct SMDLG
{
	unsigned long data1;
	unsigned short data2;
	unsigned short data3;
	unsigned char data4[8];

};
struct SBBOX
{
	float xmin;
	float ymin;
	float zmin;
	float xmax;
	float ymax;
	float zmax;
};

struct SIND3
{
	int ind1;
	int ind2;
	int ind3;
};
struct SINDE
{
	short ind1;
	short ind2;
	short ind3;
};

struct SMAT3
{
	int material_flags;
	int material_flags_2;
	int diffuse_texture_index;
	int detail_texture_index;
	int bumpmap_texture_index;
	int specular_texture_index;
	int emissive_texture_index;
	int reflection_texture_index;
	int fresnel_texture_index;
	float diffuse_color_R;
	float diffuse_color_G;
	float diffuse_color_B;
	float diffuse_color_A;
	float specular_color_R;
	float specular_color_G;
	float specular_color_B;
	float specular_color_A;
	float specular_power;
	float detail_map_scale;
	float bump_map_scale;
	float reflection_scale;
	float precipitation_offset;
	float specular_map_power_scale;
	float specular_bloom_floor;
	float ambient_light_scale;
	int source_blend;
	int destination_blend;
	int alpha_test_function;
	float alpha_test_threshold;
	float final_alpha_multiply;
	float zbias;
};

struct SEMT1
{
	float TemperatureScale;
	float DetailOffsetU;
	float DetailOffsetV;
	float DetailRotation;
	float DetailScaleV;
	float DetailBlendWeight;
	float cDetailColor_R;
	float cDetailColor_G;
	float cDetailColor_B;
	float cDetailColor_A;
	int DiffuseUVChannel;
	int DetailUVChannel;
	int SpecularUVChannel;
	int BumpUVChannel;
	int EmissiveUVChannel;
	int unk16;
	int unk17;
	int unk18;
	int unk19;
	int unk20;
	int unk21;
	int unk22;
	int unk23;
	int unk24;
	int unk25;
	int unk26;
	int unk27;
	int unk28;
	int unk29;
	int unk30;
	int unk31;
};
struct SVERT
{
	float position_x;
	float position_y;
	float position_z;
	float normal_x;
	float normal_y;
	float normal_z;
	float texture_mapping_x;
	float texture_mapping_y;
};
struct SPART
{
	int  type; //(1 = Triangle List, 2 = Triangle Fan, 3 = Triangle Strip)
	int  scenegraph_reference;
	int  material_index;
	int  vertex_buffer_index;
	int  vertex_offset;
	int  vertex_count;
	int  index_offset;
	int  index_count;
	int  mouse_rectangle_ref;     //(? )
};

struct SREFP
{
	int scenegraph_reference;
	std::string name;
};

struct SATTO
{
	int  Magic1;
	short type;
	short length;
	short offAttachPoint;
	short unk1;
	int unk2;
	int unk3;
	int unk4;
	int unk5;
	std::string fxName;
	std::string params;
	std::string attachPoint;
	int Magic2;
};

struct SBMAP
{
	long index;
};

struct SSKIN
{
	byte bn[4];
	byte weight[4];
};
//#pragma pack(pop)
struct SVERB
{
	std::vector<SVERT>* VERT;
	std::vector<SBMAP>* BMAP;
	std::vector<SSKIN>* SKIN;
};

struct CSVERB
{
	std::vector<SVERT>* VERT;
	std::vector<SBMAP>* BMAP;
	std::vector<SSKIN>* SKIN;
};
//#pragma pack(push, 1)

struct SLODT
{
	int LOD;
	std::vector<SPART>* PART;
};

struct STRAN
{
	float m00;
	float m01;
	float m02;
	float m03;
	float m10;
	float m11;
	float m12;
	float m13;
	float m20;
	float m21;
	float m22;
	float m23;
	float m30;
	float m31;
	float m32;
	float m33;
};
struct SSCEN
{
	short child_node_index;
	short peer_node_index;
	short amap_offset;
	short parent;
	INode* parentNode;
};
struct SAMAP
{
	int type;
	int matrix;
};
struct SSGAL
{
	short xanim;
};
struct SSGBR
{
	short bone_index;
};

struct SSGBN
{
	std::string bone_name;
};

struct SSGVL
{
	short vis_index;
};

struct SXANKH
{
	byte type;
	float time;
};
struct SXANKT
{
	float x;
	float y;
	float z;
};
struct SXANKR
{
	float q0;
	float q1;
	float q2;
	float q3;
};
struct SXANK
{
	byte type;
	float time;
	float x;
	float y;
	float z;
	float q0;
	float q1;
	float q2;
	float q3;
};

struct SXANS
{
	int animation_type;
	int animation_ID;
	float animation_length;
	std::vector<SXANK>* XANK;
};
struct SXAPS
{
	std::string code;
	std::string variable;
	std::string units;
	std::string bias;
	std::string lag;
};
/*struct SVISC
{
	bool Default;
	std::string code;

	std::string name;
};*/
struct SVISI
{
	bool Default;
	std::string name;
	bool CodeF = false;
	bool SimF = false;
	bool LagF = false;
	std::string Code = "";
	SSim Sim;
	int Lag;
};

struct SXANI
{
	byte guid[16];
	float animation_length; //?
	char type[16]; //"Sim"
	std::string *typeParam;// "AutoPlay
	std::vector<SXANS>* XANS;
	SXAPS XAPS;
};
#pragma pack(pop)



/*#define material_is_specular							0x00000001
#define material_has_a_diffuse_texture					0x00000002
#define material_has_a_bumpmap_texture					0x00000004
#define material_has_a_specular_texture					0x00000008
#define material_has_a_detail_texture					0x00000010
#define material_has_a_reflection_texture				0x00000020
#define Use_global_environment_map_as_reflection		0x00000040
#define material_has_an_emissive_texture_for_night		0x00000080
#define material_has_a_FresnelRamp_texture_Reflection	0x00000100
#define material_has_a_FresnelRamp_texture_Diffuse		0x00000200
#define material_has_a_FresnelRamp_texture_Specular		0x00000400
#define Apply_offset_to_start_of_Precipitation			0x00000800
#define Take_into_account_Precipitation					0x00001000
#define Blend_environment_by_inverse_of_diffuse_alpha	0x00002000
#define Blend_environment_by_specular_map_alpha			0x00004000
#define Assume_vertical_normal							0x00008000
#define Z_Write_alpha									0x00010000
#define No_Z_Write										0x00020000
#define Bloom_material_by_copying						0x00040000
#define Bloom_material_modulating_by_alpha				0x00080000
#define Volume_shadow									0x00100000
#define No_shadow										0x00200000
#define Z_Test_Alpha									0x00400000
#define Emissive_Mode_Blend								0x00800000
#define Set_final_alpha_value_at_render_time			0x01000000
#define Skinned_mesh									0x04000000
#define Allow_bloom										0x08000000
#define Allow_emissive_bloom							0x10000000
#define Blend_diffuse_by_diffuse_alpha					0x20000000
#define Blend_diffuse_by_inverse_of_specular_map_alpha	0x40000000
#define Prelit_vertices									0x80000000

#define Blend_constant									0x00000001
#define Force_Texture_Address_Wrap						0x00000002
#define Force_Texture_Address_Clamp						0x00000004
#define Double_sided									0x00000008
#define Emissive_Mode_AdditiveNightOnlyUserControlled	0x00000010
#define Emissive_Mode_BlendUserControlled				0x00000020
#define Emissive_Mode_MultiplyBlend						0x00000040
#define Emissive_Mode_MultiplyBlendUserControlled		0x00000080
#define Emissive_Mode_Additive							0x00000100
#define Emissive_Mode_AdditiveUserControlled			0x00000200*/

/*enum source_blend
{
	zero = 1,
	one,
	srcColor,
	invSrcColor,
	srcAlpha,
	invSrcAlpha,
	destAlpha,
	invDestAlpha,
	destColor,
	invDestColor
};
enum alpha_test_function
{
	never = 1,
	less,
	equal,
	lessEqual,
	greater,
	notEqual,
	greaterEqual,
	always
};*/

struct SLODEData
{
	UINT LOD;

};


class RIFF
{
private:
	ReadStream* RIFFStream = NULL;
	WriteStreamText* RIFFText = NULL;
	UINT64 Seek = 0;
	
public:
	RIFF(const TCHAR* filename, ImpInterface * i, Interface * gi);
	std::string ShortCode(std::string Code);
	std::vector<std::string> ConvertVisAnim(std::string std, int maxCount, int NamePar, int type);
	HRESULT ReadSectionsHierarhy();
	UINT UINT2(char Name[4]);
	void WriteData(int Data, UINT parent);
	void WriteData(float Data, UINT parent);
	void WriteData(std::string Data, UINT parent);
	HRESULT ReadSection(int parent = 0);
	ImpInterface * i; 
	Interface * gi;
	~RIFF();

	SRIFFData RIFFData;
	SLODEData LODEData;
	SMDLH MDLH;
	SMDLG MDLG;
	SBBOX BBOX;
	float RADI;
//#pragma pack(push, 1)
	/*std::vector<std::string>* TEXT = new std::vector<std::string>();
	std::vector<SMAT3>* MAT3 = new std::vector<SMAT3>();
	std::vector<SEMT1>* EMT1 = new std::vector<SEMT1>();
	std::vector<SIND3>* IND3 = new std::vector<SIND3>();
	std::vector<SINDE>* INDE = new std::vector<SINDE>();
	std::vector<SVERB>* VERB = new std::vector<SVERB>();
	std::vector<SLODT>* LODT = new std::vector<SLODT>();
	std::vector<STRAN>* TRAN = new std::vector<STRAN>();
	std::vector<SSCEN>* SCEN = new std::vector<SSCEN>();
	std::vector<SSGAL>* SGAL = new std::vector<SSGAL>();
	std::vector<SSGBR>* SGBR = new std::vector<SSGBR>();
	std::vector<SSGBN>* SGBN = new std::vector<SSGBN>();
	std::vector<SSGVL>* SGVL = new std::vector<SSGVL>();
	std::vector<SVISI>* VISC = new std::vector<SVISI>();
	std::vector<SAMAP>* AMAP = new std::vector<SAMAP>();
	std::vector<SXANI>* XANI = new std::vector<SXANI>();
	std::vector<SREFP>* REFP = new std::vector<SREFP>();
	std::vector<SATTO>* ATTO = new std::vector<SATTO>();


	std::vector<SAnimation>* AnimationXML = new std::vector<SAnimation>();
	std::vector<SPartInfo>* PartInfoXML = new std::vector<SPartInfo>();
	//std::vector<SXANS>* XANS = new std::vector<SXANS>();
	//std::vector<SXANK>* XANK = new std::vector<SXANK>();

	std::vector<UINT>* BadSec = new std::vector<UINT>();*/
	std::vector<std::string>* TEXT;
	std::vector<SMAT3>* MAT3;
	std::vector<SEMT1>* EMT1;
	std::vector<SIND3>* IND3;
	std::vector<SINDE>* INDE;
	std::vector<SVERB>* VERB;
	std::vector<SLODT>* LODT;
	std::vector<STRAN>* TRAN;
	std::vector<SSCEN>* SCEN;
	std::vector<SSGAL>* SGAL;
	std::vector<SSGBR>* SGBR;
	std::vector<SSGBN>* SGBN;
	std::vector<SSGVL>* SGVL;
	std::vector<SVISI>* VISI;
	std::vector<SAMAP>* AMAP;
	std::vector<SXANI>* XANI;
	std::vector<SREFP>* REFP;
	std::vector<SATTO>* ATTO;


	std::vector<SAnimation>* AnimationXML;
	std::vector<SPartInfo>* PartInfoXML;
	//std::vector<SXANS>* XANS = new std::vector<SXANS>();
	//std::vector<SXANK>* XANK = new std::vector<SXANK>();

	std::vector<UINT>* BadSec = NULL;
	SXANI* CurrXANI = NULL;
	SXANS* CurrXANS = NULL;
//#pragma pack(pop)
	SLODT CurrLOD;
	SVERB CurrVERB;
	std::string MDLN;
	int UniVISI = 0;
};

extern BOOL DrawPart(RIFF* riff, SPART* part);
