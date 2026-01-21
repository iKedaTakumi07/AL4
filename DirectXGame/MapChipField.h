#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include <cstdint>
#include <string>
#include <vector>

enum class MapChipType {
	kBlank,          // 空白
	kBlock,          // ブロック
	kBreakableBlock, // 破壊可能ブロック
	kChageBreakeBlock, // チャージ破壊可能ブロック
	kPlayer,
	kEnemy,
	kAbility,
	kGoal,
};

enum MapChipCharIndex {
	kChipType = 0, // マップチップタイプ
	kChipSubID = 1 // タイプごとのサブID
};

struct MapChipDataUnit {
	MapChipType type; // マップチップの種別
	uint8_t SubID;    // 種類ごとのサブID
};

struct MapChipDate {
	std::vector<std::vector<MapChipDataUnit>> data;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	struct IndexSet {
		uint32_t xindex;
		uint32_t yindex;
	};

	// 範囲矩形
	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	uint8_t GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical();
	uint32_t GetNumBlockHorizontal();

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByindex(uint32_t xindex, uint32_t yindex);

	// set
	 void SetMapChipType(uint32_t xIndex, uint32_t yIndex, MapChipType mapChipType) { mapChipData_.data[yIndex][xIndex].type = mapChipType; };

private:
	MapChipDate mapChipData_;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 40;
	static inline const uint32_t kNumBlockHorizontal = 100;
};
