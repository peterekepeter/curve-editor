pub export fn linear_interpolation(arg_t: f32, arg_a: f32, arg_b: f32) f32 {
    var t = arg_t;
    var a = arg_a;
    var b = arg_b;
    return a + (t * (b - a));
}

pub export fn quadratic_bezier(arg_t: f32, arg_a: f32, arg_b: f32, arg_c: f32) f32 {
    var t = arg_t;
    var a = arg_a;
    var b = arg_b;
    var c = arg_c;
    var one_minus_t: f32 = @as(f32, @floatFromInt(@as(c_int, 1))) - t;
    return (((a * one_minus_t) * one_minus_t) + (((b * @as(f32, @floatFromInt(@as(c_int, 2)))) * one_minus_t) * t)) + ((c * t) * t);
}

pub export fn cubic_bezier(arg_t: f32, arg_a: f32, arg_b: f32, arg_c: f32, arg_d: f32) f32 {
    var t = arg_t;
    var a = arg_a;
    var b = arg_b;
    var c = arg_c;
    var d = arg_d;
    var one_minus_t: f32 = @as(f32, @floatFromInt(@as(c_int, 1))) - t;
    var one_minus_t_sqr: f32 = one_minus_t * one_minus_t;
    var t_sqr: f32 = t * t;
    return ((((a * one_minus_t_sqr) * one_minus_t) + (((@as(f32, @floatFromInt(@as(c_int, 3))) * b) * one_minus_t_sqr) * t)) + (((@as(f32, @floatFromInt(@as(c_int, 3))) * c) * one_minus_t) * t_sqr)) + ((d * t_sqr) * t);
}

pub export fn generalized_bezier(arg_t: f32, arg_points: [*c]const f32, arg_count: c_int) f32 {
    var t = arg_t;
    var points = arg_points;
    var count = arg_count;
    while (true) {
        switch (count) {
            @as(c_int, 0) => return 0.0,
            @as(c_int, 1) => return points.*,
            @as(c_int, 2) => return linear_interpolation(t, points[@as(c_uint, @intCast(@as(c_int, 0)))], points[@as(c_uint, @intCast(@as(c_int, 1)))]),
            @as(c_int, 3) => return quadratic_bezier(t, points[@as(c_uint, @intCast(@as(c_int, 0)))], points[@as(c_uint, @intCast(@as(c_int, 1)))], points[@as(c_uint, @intCast(@as(c_int, 2)))]),
            @as(c_int, 4) => return cubic_bezier(t, points[@as(c_uint, @intCast(@as(c_int, 0)))], points[@as(c_uint, @intCast(@as(c_int, 1)))], points[@as(c_uint, @intCast(@as(c_int, 2)))], points[@as(c_uint, @intCast(@as(c_int, 3)))]),
            @as(c_int, 5) => return linear_interpolation(t, generalized_bezier(t, points, count - @as(c_int, 1)), generalized_bezier(t, points + @as(usize, @bitCast(@as(isize, @intCast(@as(c_int, 1))))), count - @as(c_int, 1))),
            @as(c_int, 6) => return quadratic_bezier(t, generalized_bezier(t, points, count - @as(c_int, 2)), generalized_bezier(t, points + @as(usize, @bitCast(@as(isize, @intCast(@as(c_int, 1))))), count - @as(c_int, 2)), generalized_bezier(t, points + @as(usize, @bitCast(@as(isize, @intCast(@as(c_int, 2))))), count - @as(c_int, 2))),
            else => return cubic_bezier(t, generalized_bezier(t, points, count - @as(c_int, 3)), generalized_bezier(t, points + @as(usize, @bitCast(@as(isize, @intCast(@as(c_int, 1))))), count - @as(c_int, 3)), generalized_bezier(t, points + @as(usize, @bitCast(@as(isize, @intCast(@as(c_int, 2))))), count - @as(c_int, 3)), generalized_bezier(t, points + @as(usize, @bitCast(@as(isize, @intCast(@as(c_int, 3))))), count - @as(c_int, 3))),
        }
        break;
    }
    return 0;
}


test "thing"
{
    const expectApproxEqAbs = @import("std").testing.expectApproxEqAbs;
    const lerp = linear_interpolation;

    try expectApproxEqAbs(@as(f32, 0.5), lerp(0.5, 0.0, 1.0), 0.0001);
}