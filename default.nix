{ withRustBackend ? false, withZigBackend ? false }:

let
  sources = import ./nix/sources.nix;
  pkgs = import sources.nixpkgs { };
  gitignore = import sources."gitignore.nix" { };
  nixGL = (import sources.nixGL {
    inherit pkgs;
    # 32 bit support can be enabled after https://github.com/NixOS/nixpkgs/pull/136738
    enable32bits = false;
  }).auto.nixGLDefault;
  stdenv = pkgs.clang11Stdenv;
in
pkgs.callPackage ./release.nix {
  inherit stdenv withRustBackend withZigBackend nixGL;
  inherit (gitignore) gitignoreSource;
  llvmPackages = pkgs.llvmPackages_11;
}
