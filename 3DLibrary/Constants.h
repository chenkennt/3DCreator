#if !defined(GRAPHICS_3D_CONSTANTS__INCLUDED_)
#define GRAPHICS_3D_CONSTANTS__INCLUDED_

#define ZERO 1E-4
#define ZERO_FOR_SURFACEMODEL 1E-1
#define PROJECTION_PARALLEL 0
#define PROJECTION_PERSPECTIVE 1
#define BALL_VERTICAL_PIECES 12
#define BALL_HORIZONTAL_PIECES 24
#define CYLINDER_PIECES 24
#define CONE_PIECES 24
#define CIRCLELOFT_PIECES 24
#define PI 3.1415926
#define K 1
#define DK 0.0001
#define NO_ANTI_ALIAS 0
#define ANTI_ALIAS_4X 1
#define ANTI_ALIAS_9X 2
#define TOP_VIEW 0
#define BOTTOM_VIEW 1
#define LEFT_VIEW 2
#define RIGHT_VIEW 3
#define FRONT_VIEW 4
#define BACK_VIEW 5
#define PERSPECTIVE_VIEW 6
#define MAX_HORIZONTAL_FACE_PIECES 20
#define MAX_HORIZONTAL_FACE_GAP 50
#define TYPE_CUBE 0
#define TYPE_CYLINDER 1
#define TYPE_CONE 2
#define TYPE_SPHERE 3
#define TYPE_BOOLEAN 4
#define TYPE_CIRCLELOFT 5
#define TYPE_LINELOFT 6
#define TYPE_LIGHT 7
#define TYPE_UNION 8
#define TYPE_INTERSECT 9
#define TYPE_DIFFERENCE 10
#define LINECURVE 5 * PI / 180
#define LINECURVE_FORLOFT 15 * PI / 180
#define DISTANCE_FORSELECT 25
#define MAX_DEPTH 10
#define SIMPLE_RENDER_GAP 4
#define MAX_COLOR_DIFFERENCE 20

#endif