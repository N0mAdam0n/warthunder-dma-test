#include <imgui.h>

struct bone_pos
{
    Vector2 screen_pos;
    Vector3 world_pos;
};

class Aim
{
private:
    static bone_pos closest_point_to_center;
    static void do_kmbox_aim();
    static bool is_point_between_circle_edges(const Vector2& point, const Vector2& center, float radius, float leftOffset, float rightOffset);
    static bool is_point_inside_circle(const Vector2& point, const Vector2& center, float radius);
    static bone_pos find_closest_point_to_center(const std::vector<bone_pos>& points, const Vector2& center);
    static void move_mouse_from_center_to_point(Vector2 point);
public:
    static void UpdateThread(Aim* instanse);
    static void DrawAim(ImDrawList* draw_list);
    
};


