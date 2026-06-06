#pragma once



enum GuiState : std::uint8_t
{
	NONE = 0,
	MENU = 1,
	ALIVE = 2,
	DEAD = 3,
	SPEC = 6,
	SPAWN_MENU = 8,
	LOADING = 10
};


struct RAngle
{
	float yaw, pitch;
	RAngle(float yaw_, float pitch_) : yaw(yaw_), pitch(pitch_) {}
	RAngle() : yaw(0), pitch(0) {}




};

