const std = @import("std");
const testing = std.testing;

// FIXME: This doesn't feel right
const c_float = f32;

fn todo() noreturn {
    @panic("Not yet implemented");
}

const c = @cImport({
    @cInclude("sgl.h");
});

export fn sglGetError() c.sglEErrorCode {
    todo();
}

export fn sglGetErrorString(err: c.sglEErrorCode) [*]u8 {
    todo();
}

export fn sglInit() void {
    std.log.info("Hello from Zig", .{});
    todo();
}

export fn sglFinish() void {
    todo();
}

export fn sglCreateContext(width: c_int, height: c_int) c_int {
    todo();
}

export fn sglDestroyContext(id: c_int) void {
    todo();
}

export fn sglSetContext(id: c_int) void {
    todo();
}

export fn sglGetContext() c_int {
    todo();
}

export fn sglGetColorBufferPointer() [*]c_float {
    todo();
}

export fn sglClear(what: c.sglEClearBit) void {
    todo();
}

export fn sglBegin(mode: c.sglEElementType) void {
    todo();
}

export fn sglEnd() void {
    todo();
}

export fn sglVertex4f(x: c_float, y: c_float, z: c_float, w: c_float) void {
    todo();
}

export fn sglVertex3f(x: c_float, y: c_float, z: c_float) void {
    todo();
}

export fn sglVertex2f(x: c_float, y: c_float) void {
    todo();
}

export fn sglCircle(x: c_float, y: c_float, z: c_float, radius: c_float) void {
    todo();
}

export fn sglEllipse(x: c_float, y: c_float, z: c_float, a: c_float, b: c_float) void {
    todo();
}

export fn sglArc(x: c_float, y: c_float, z: c_float, radius: c_float, from: c_float, to: c_float) void {
    todo();
}

export fn sglMatrixMode(mode: c.sglEMatrixMode) void {
    todo();
}

export fn sglPushMatrix() void {
    todo();
}

export fn sglPopMatrix() void {
    todo();
}

export fn sglLoadIdentity() void {
    todo();
}

export fn sglLoadMatrix(matrix: [*]const c_float) void {
    todo();
}

export fn sglMultMatrix(matrix: [*]const c_float) void {
    todo();
}

export fn sglTranslate(x: c_float, y: c_float, z: c_float) void {
    todo();
}

export fn sglScale(scale_x: c_float, scale_y: c_float, scale_z: c_float) void {
    todo();
}

export fn sglRotate2D(angle: c_float, center_x: c_float, center_y: c_float) void {
    todo();
}

export fn sglRotateY(angle: c_float) void {
    todo();
}

export fn sglOrtho(left: c_float, right: c_float, bottom: c_float, top: c_float, near: c_float, far: c_float) void {
    todo();
}

export fn sglFrustum(left: c_float, right: c_float, bottom: c_float, top: c_float, near: c_float, far: c_float) void {
    todo();
}

export fn sglViewport(x: c_int, y: c_int, width: c_int, height: c_int) void {
    todo();
}

export fn sglClearColor(r: c_float, g: c_float, b: c_float, alpha: c_float) void {
    todo();
}

export fn sglColor3f(r: c_float, g: c_float, b: c_float) void {
    todo();
}

export fn sglAreaMode(mode: c.sglEAreaMode) void {
    todo();
}

export fn sglPointSize(size: c_float) void {
    todo();
}

export fn sglEnable(capabilities: c.sglEEnableFlags) void {
    todo();
}

export fn sglDisable(capabilities: c.sglEEnableFlags) void {
    todo();
}

export fn sglBeginScene() void {
    todo();
}

export fn sglEndScene() void {
    todo();
}

export fn sglSphere(x: c_float, y: c_float, z: c_float, r: c_float) void {
    todo();
}

export fn sglMaterial(r: c_float, g: c_float, b: c_float, diffuse: c_float, specular: c_float, shine: c_float, transmittance: c_float, refraction: c_float) void {
    todo();
}

export fn sglPointLight(x: c_float, y: c_float, z: c_float, r: c_float, g: c_float, b: c_float) void {
    todo();
}

export fn sglRayTraceScene() void {
    todo();
}

export fn sglRasterizeScene() void {
    todo();
}

export fn sglEmissiveMaterial(r: c_float, g: c_float, b: c_float, att_0: c_float, att_1: c_float, att_2: c_float) void {
    todo();
}

export fn sglEnvironmentMap(width: c_int, height: c_int, texels: [*]c_float) void {
    todo();
}
