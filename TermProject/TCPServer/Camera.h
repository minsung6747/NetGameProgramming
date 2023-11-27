#pragma once

class Camera
{
public:
	Camera();
	~Camera();

	float camera_distance{ 3.f };

	float mouse_sensitivity{ 200.f };

	float camera_drgree_x{};
	float camera_drgree_y{};
	float camera_delta_y{};

	float camera_fov{ 60.f };

	float camera_moving_timer{};
};

