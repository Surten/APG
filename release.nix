{ stdenv
, lib
, nixGL
, cmake
, ninja
, makeWrapper
, freeglut
, libglvnd
, libGLU
, gitignoreSource
, withRustBackend ? false
, llvmPackages ? null
, rustPlatform ? null
, withZigBackend ? false
, zig ? null
}:

let
  pname = "testapp";
  version = "1.0";
  withCppBackend = !(withRustBackend || withZigBackend);
  backendString =
    if withRustBackend then "rust"
    else if withZigBackend then "zig"
    else "cpp";
  src = gitignoreSource ./.;
  cargoRoot = "sgl-rust";
in

assert with lib; length (filter id [ withCppBackend withRustBackend withZigBackend ]) == 1;
assert withRustBackend -> llvmPackages != null && rustPlatform != null;
assert withZigBackend -> zig != null;

stdenv.mkDerivation ({
  inherit pname version src;

  nativeBuildInputs = [ cmake ninja makeWrapper ]
    ++ lib.optionals withRustBackend (with rustPlatform; [ cargoSetupHook rust.cargo ])
    ++ lib.optional withZigBackend zig;
  buildInputs = [ freeglut libGLU libglvnd nixGL ];

  cmakeFlags = "-DSGL_BACKEND=${backendString}";

  postInstall = ''
    mv $out/bin/testapp $out/bin/.testapp-original
    echo "exec ${nixGL}/bin/nixGL $out/bin/.testapp-original" >> $out/bin/testapp
    chmod +x $out/bin/testapp
  '';
} // lib.optionalAttrs withRustBackend {
  # Vendor Rust dependencies (derivations are built without internet access)
  inherit cargoRoot;
  cargoDeps = rustPlatform.fetchCargoTarball {
    inherit src;
    sourceRoot = "source/${cargoRoot}";
    hash = "sha256:026isz1w9i1c57ray3rgk2h3hir4l2m0mhbq5pzk4yv715py1d88";
  };

  LIBCLANG_PATH = "${llvmPackages.libclang.lib}/lib";
})
