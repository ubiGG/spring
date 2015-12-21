/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef SMFREADMAP_H
#define SMFREADMAP_H

#include "SMFMapFile.h"
#include "Map/ReadMap.h"
#include "System/EventClient.h"
#include "System/type2.h"


class CSMFGroundDrawer;

class CSMFReadMap : public CReadMap, public CEventClient
{
public:
	// CEventClient interface
	int GetReadAllyTeam() const { return AllAccessTeam; }
	bool WantsEvent(const std::string& eventName) {
		return (eventName == "SunChanged");
	}

	void SunChanged(const float3& sunDir);

public:
	CR_DECLARE(CSMFReadMap)

	CSMFReadMap(std::string mapname);
	// note: textures are auto-deleted
	~CSMFReadMap() {}

	void UpdateShadingTexture();
	void UpdateHeightMapUnsynced(const SRectangle&);

public:
	void SetLuaTexture(const MapTextureData& td) {
		const unsigned int num = Clamp(int(td.num), 0, NUM_SPLAT_DETAIL_NORMALS - 1);

		switch (td.type) {
			case MAP_BASE_GRASS_TEX: { grassShadingTex.SetLuaTexture(td); } break;
			case MAP_BASE_DETAIL_TEX: { detailTex.SetLuaTexture(td); } break;
			case MAP_BASE_MINIMAP_TEX: { minimapTex.SetLuaTexture(td); } break;
			case MAP_BASE_SHADING_TEX: { shadingTex.SetLuaTexture(td); } break;
			case MAP_BASE_NORMALS_TEX: { normalsTex.SetLuaTexture(td); } break;

			case MAP_SSMF_SPECULAR_TEX: { specularTex.SetLuaTexture(td); } break;
			case MAP_SSMF_NORMALS_TEX: { blendNormalsTex.SetLuaTexture(td); } break;

			case MAP_SSMF_SPLAT_DISTRIB_TEX: { splatDistrTex.SetLuaTexture(td); } break;
			case MAP_SSMF_SPLAT_DETAIL_TEX: { splatDetailTex.SetLuaTexture(td); } break;
			case MAP_SSMF_SPLAT_NORMAL_TEX: { splatNormalTextures[num].SetLuaTexture(td); } break;

			case MAP_SSMF_SKY_REFLECTION_TEX: { skyReflectModTex.SetLuaTexture(td); } break;
			case MAP_SSMF_LIGHT_EMISSION_TEX: { lightEmissionTex.SetLuaTexture(td); } break;
			case MAP_SSMF_PARALLAX_HEIGHT_TEX: { parallaxHeightTex.SetLuaTexture(td); } break;

			default: {} break;
		}
	}

public:
	unsigned int GetTexture(unsigned int type, unsigned int num = 0) const {
		unsigned int texID = 0;

		switch (type) {
			case MAP_BASE_GRASS_TEX: { texID = GetGrassShadingTexture(); } break;
			case MAP_BASE_DETAIL_TEX: { texID = GetDetailTexture(); } break;
			case MAP_BASE_MINIMAP_TEX: { texID = GetMiniMapTexture(); } break;
			case MAP_BASE_SHADING_TEX: { texID = GetShadingTexture(); } break;
			case MAP_BASE_NORMALS_TEX: { texID = GetNormalsTexture(); } break;

			case MAP_SSMF_NORMALS_TEX: { texID = GetBlendNormalsTexture(); } break;
			case MAP_SSMF_SPECULAR_TEX: { texID = GetSpecularTexture(); } break;

			case MAP_SSMF_SPLAT_DISTRIB_TEX: { texID = GetSplatDistrTexture(); } break;
			case MAP_SSMF_SPLAT_DETAIL_TEX: { texID = GetSplatDetailTexture(); } break;
			case MAP_SSMF_SPLAT_NORMAL_TEX: { texID = GetSplatNormalTexture(num); } break;

			case MAP_SSMF_SKY_REFLECTION_TEX: { texID = GetSkyReflectModTexture(); } break;
			case MAP_SSMF_LIGHT_EMISSION_TEX: { texID = GetLightEmissionTexture(); } break;
			case MAP_SSMF_PARALLAX_HEIGHT_TEX: { texID = GetParallaxHeightTexture(); } break;

			default: {} break;
		}

		return texID;
	}

	int2 GetTextureSize(unsigned int type, unsigned int num = 0) const {
		int2 size;

		// TODO:
		//   for now, always return the raw texture size even if HasLuaTex
		//   this means any Lua-set textures must have the same dimensions
		//   as their originals (not an unreasonable limitation except for
		//   custom shaders)
		switch (type) {
			case MAP_BASE_GRASS_TEX: { size = grassShadingTex.GetRawSize(); } break;
			case MAP_BASE_DETAIL_TEX: { size = detailTex.GetRawSize(); } break;
			case MAP_BASE_MINIMAP_TEX: { size = minimapTex.GetRawSize(); } break;
			case MAP_BASE_SHADING_TEX: { size = shadingTex.GetRawSize(); } break;
			case MAP_BASE_NORMALS_TEX: { size = normalsTex.GetRawSize(); } break;

			case MAP_SSMF_NORMALS_TEX: { size = blendNormalsTex.GetRawSize(); } break;
			case MAP_SSMF_SPECULAR_TEX: { size = specularTex.GetRawSize(); } break;

			case MAP_SSMF_SPLAT_DISTRIB_TEX: { size = splatDistrTex.GetRawSize(); } break;
			case MAP_SSMF_SPLAT_DETAIL_TEX: { size = splatDetailTex.GetRawSize(); } break;
			case MAP_SSMF_SPLAT_NORMAL_TEX: { size = splatNormalTextures[num].GetRawSize(); } break;

			case MAP_SSMF_SKY_REFLECTION_TEX: { size = skyReflectModTex.GetRawSize(); } break;
			case MAP_SSMF_LIGHT_EMISSION_TEX: { size = lightEmissionTex.GetRawSize(); } break;
			case MAP_SSMF_PARALLAX_HEIGHT_TEX: { size = parallaxHeightTex.GetRawSize(); } break;
		}

		return size;
	}

public:
	unsigned int GetGrassShadingTexture() const { return grassShadingTex.GetID(); }
	unsigned int GetMiniMapTexture() const { return minimapTex.GetID(); }
	unsigned int GetDetailTexture() const { return detailTex.GetID(); }
	unsigned int GetShadingTexture() const { return shadingTex.GetID(); }
	unsigned int GetNormalsTexture() const { return normalsTex.GetID(); }


	unsigned int GetSpecularTexture() const { return specularTex.GetID(); }
	unsigned int GetBlendNormalsTexture() const { return blendNormalsTex.GetID(); }

	unsigned int GetSplatDistrTexture() const { return splatDistrTex.GetID(); }
	unsigned int GetSplatDetailTexture() const { return splatDetailTex.GetID(); }
	unsigned int GetSplatNormalTexture(int i) const { return splatNormalTextures[i].GetID(); }

	unsigned int GetSkyReflectModTexture() const { return skyReflectModTex.GetID(); }
	unsigned int GetLightEmissionTexture() const { return lightEmissionTex.GetID(); }
	unsigned int GetParallaxHeightTexture() const { return parallaxHeightTex.GetID(); }

public:
	void DrawMinimap() const;
	void GridVisibility(CCamera* cam, IQuadDrawer* cb, float maxDist, int quadSize, int extraSize = 0) override;

	void InitGroundDrawer();
	void KillGroundDrawer();

	int GetNumFeatureTypes();
	int GetNumFeatures();
	void GetFeatureInfo(MapFeatureInfo* f); // returns all feature info in MapFeatureInfo[NumFeatures]
	const char* GetFeatureTypeName(int typeID);

	unsigned char* GetInfoMap(const std::string& name, MapBitmapInfo* bm);
	void FreeInfoMap(const std::string& name, unsigned char* data);

	// NOTE: do not use, just here for backward compatibility with SMFGroundTextures.cpp
	inline CSMFMapFile& GetFile() { return file; }
	inline CBaseGroundDrawer* GetGroundDrawer();


	void ConfigureAnisotropy();
	float GetAnisotropy() const { return anisotropy; }

	bool HaveSpecularTexture() const { return haveSpecularTexture; }
	bool HaveSplatDetailDistribTexture() const { return haveSplatDetailDistribTexture; }
	bool HaveSplatNormalDistribTexture() const { return haveSplatNormalDistribTexture; }
	bool HaveDetailNormalDiffuseAlpha() const { return haveDetailNormalDiffuseAlpha; }

private:
	void ParseHeader();
	void LoadHeightMap();
	void LoadMinimap();
	void InitializeWaterHeightColors();
	void CreateSpecularTex();
	void CreateSplatDetailTextures();
	void CreateGrassTex();
	void CreateDetailTex();
	void CreateShadingTex();
	void CreateNormalTex();

	void UpdateVertexNormalsUnsynced(const SRectangle& update);
	void UpdateFaceNormalsUnsynced(const SRectangle& update);
	void UpdateNormalTexture(const SRectangle& update);
	void UpdateShadingTexture(const SRectangle& update);

	inline void UpdateShadingTexPart(int idx1, int idx2, unsigned char* dst) const;
	inline const float GetCenterHeightUnsynced(const int x, const int y) const;

	inline float DiffuseSunCoeff(const int x, const int y) const;
	inline float3 GetLightValue(const int x, const int y) const;
	void ParseSMD(std::string filename);

public:
	// constants
	static const int tileScale     = 4;
	static const int bigSquareSize = 32 * tileScale;
	static const int NUM_SPLAT_DETAIL_NORMALS = 4;

	// globals for SMFGround{Drawer, Textures}
	int numBigTexX;
	int numBigTexY;
	int bigTexSize;
	int tileMapSizeX;
	int tileMapSizeY;
	int tileCount;
	int mapSizeX;
	int mapSizeZ;
	int maxHeightMapIdx;
	int heightMapSizeX;

private:
	CSMFMapFile file;
	CSMFGroundDrawer* groundDrawer;

private:
	std::vector<float> cornerHeightMapSynced;
	std::vector<float> cornerHeightMapUnsynced;

	std::vector<unsigned char> shadingTexBuffer;
	std::vector<unsigned char> waterHeightColors;

private:
	MapTexture grassShadingTex;       // specifies grass-blade modulation color (defaults to minimapTex)
	MapTexture detailTex;             // supplied by the map
	MapTexture minimapTex;            // supplied by the map
	MapTexture shadingTex;            // holds precomputed dot(lightDir, vertexNormal) values
	MapTexture normalsTex;            // holds vertex normals in RGBA32F internal format (GL_RGBA + GL_FLOAT)

	MapTexture specularTex;           // supplied by the map, moderates specular contribution
	MapTexture blendNormalsTex;       // tangent-space offset normals

	MapTexture splatDistrTex;         // specifies the per-channel distribution of splatDetailTex (map-wide, overrides detailTex)
	MapTexture splatDetailTex;        // contains per-channel separate greyscale detail-textures (overrides detailTex)

	// contains RGBA texture with RGB channels containing normals;
	// alpha contains greyscale diffuse for splat detail normals
	// if haveDetailNormalDiffuseAlpha (overrides detailTex)
	MapTexture splatNormalTextures[NUM_SPLAT_DETAIL_NORMALS];

	MapTexture skyReflectModTex;      // modulates sky-reflection RGB intensities (must be the same size as specularTex)
	MapTexture lightEmissionTex;
	MapTexture parallaxHeightTex;

private:
	int shadingTexUpdateProgress;

	float anisotropy;

	bool haveSpecularTexture;
	bool haveSplatDetailDistribTexture; // true if we have both splatDetailTex and splatDistrTex
	bool haveSplatNormalDistribTexture; // true if we have splatDistrTex and at least one splat[Detail]NormalTex
	bool haveDetailNormalDiffuseAlpha;

	bool minimapOverride;
	bool shadingTexUpdateNeeded;
};

#endif
