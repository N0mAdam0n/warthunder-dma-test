#pragma once
#include <cstdint>

// =============================================================================
// Offset_manual_update.h
//
// 游戏大版本更新后需要手动维护的全部偏移量。
// 只需修改本文件后重新编译 — offsets.h 会自动包含本文件。
//
// 不在此文件中（运行时由 offset_scanner 自动解析）：
//   offsets::CGame, offsets::m_player_ptr
//
// 故障排查速查：
//   启动时偏移解析失败           -> fallback + m_player::local_player
//   单位列表为空 / 数量错误      -> _CGame::unit_list3, unit_count3
//   ESP 位置 / 世界转屏幕错误    -> _CUnit position/bounds, _CCamera, _CGame::local_camera
//   敌我识别 / 本地玩家错误      -> _CUnit::team_num, _CPlayer::player_unit, m_player::local_player
//   载具名称不显示               -> _CUnit::unit_info, _CUnitInfo, _CVehiceInfo
//   界面状态判断错误             -> _CPlayer::gui_state, _CUnit::unit_state
// =============================================================================

// --- 在此记录逆向信息（可选）---------------------------------------------------
// 游戏版本   :
// 构建日期   :
// 最后更新   :
// 逆向工具   :
// 备注       :
// ------------------------------------------------------------------------------

namespace offsets
{
	// "mem.memreport_sys" 全局块基址到 CGame 指针字段的偏移。
	// offset_scanner 通过字符串引用解析 CGame 时使用。
	constexpr uintptr_t CGame_field_offset = 0x40;

	// 硬编码镜像 RVA — 仅在 string_ref / 特征码扫描均失败时使用。
	// CGame fallback = mem.memreport_sys 全局 RVA + CGame_field_offset。
	namespace fallback
	{
		constexpr uintptr_t CGame_base     = 0x5BD0270;   // mem.memreport_sys 全局 RVA
		constexpr uintptr_t CGame          = CGame_base + CGame_field_offset;
		constexpr uintptr_t m_player_ptr   = 0x5B923B8;
	}

	// m_player 全局结构（镜像级指针在运行时自动解析）。
	namespace m_player
	{
		constexpr uint64_t local_player = 0x58;   // m_player 全局内的 LocalPlayer*
	}

	// CGame 单例实例字段。
	namespace _CGame
	{
		constexpr uintptr_t unit_list3   = 0x328;   // 单位列表指针（scatter 读取源）
		constexpr uintptr_t unit_count3  = 0x338;   // 当前活跃单位数量
		constexpr uintptr_t local_camera = 0x5D8;   // 本地 CCamera*
	}

	// CPlayer 实例字段。
	namespace _CPlayer
	{
		constexpr uintptr_t player_unit = 0x7E8;   // 当前载具指针（读取值需 -1）
		constexpr uintptr_t gui_state   = 0x5F0;   // GuiState 枚举（存活/死亡/观战等）
	}

	// CUnitInfo 实例字段。
	namespace _CUnitInfo
	{
		constexpr uintptr_t tank_info = 0x20;   // CVehicleInfo*
	}

	// CVehicleInfo 实例字段。
	namespace _CVehiceInfo
	{
		constexpr uintptr_t vehicle_name = 0x0;   // char* 载具显示名称
	}

	// CCamera 实例字段。
	namespace _CCamera
	{
		constexpr uintptr_t camera_viewmatrix = 0x1C0;   // 视图矩阵（world_to_screen 用）
	}

	// CUnit 实例字段（ESP 核心）。
	namespace _CUnit
	{
		constexpr uintptr_t invulnerable_offset = 0xc48;   // 无敌状态标志
		constexpr uintptr_t position            = 0xAf0;   // Vector3 世界坐标
		constexpr uintptr_t boundsmin           = 0x220;   // AABB 最小值（屏幕方框）
		constexpr uintptr_t boundsmax           = 0x22c;   // AABB 最大值（屏幕方框）
		constexpr uintptr_t unit_info           = 0xDC8;   // CUnitInfo*
		constexpr uintptr_t unit_state          = 0xD28;   // 单位存活/死亡状态
		constexpr uintptr_t player_ptr          = 0xD40;   // 所属 CPlayer*
		constexpr uintptr_t team_num            = 0xDB8;   // 队伍编号（敌我过滤）
	}
}